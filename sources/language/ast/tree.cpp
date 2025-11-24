#include "language/ast/tree.hpp"

#include <sstream>


auto highlight(std::stringstream& ss,
			   const tk::const_token_view& tv,
			   const char* group) -> void {

	tv.for_each_chunk(
		[](const tk::chunk& ck, std::stringstream& ss, const char* group) static -> void {
			//const char* group = "IncSearch";
			//const char* group = "Underlined";
			const auto& r = ck.range;

			ss << "{\"l\":" << r.ln
					   << ",\"s\":" << r.cs
					   << ",\"e\":" << r.ce
					   << ",\"g\":\"" << group << "\"},";
		}, ss, group
	);
}


auto as::tree::play_atomic(play_ctx& ctx) -> void {
	auto& atomic = node<as::atomic_values>(ctx.fr.node);

	const auto local = ctx.fr.time * ctx.fr.speed;

	// compute step
    mx::frac ratio = local / atomic.header.dur;
    mx::usz  step  = (ratio.num * atomic.header.steps) / ratio.den;


	if (step != atomic.step) {
		atomic.step = step;

		const auto value = value_at(atomic.value_start + step);
		//ctx.events.back().values[atomic.param_id] = value;
	}


	auto tv = tokens->filtered_view(
		atomic.token_start + step);

	highlight(ctx.hi, tv, "IncSearch");
}




auto as::tree::play_group(play_ctx& ctx) -> void {

	const auto& group = node<as::group>(ctx.fr.node);

	std::cout << "Before group frac_mod: ctx.fr.time=" << ctx.fr.time
			  << ", ctx.fr.speed=" << ctx.fr.speed << "\n";
	auto local = mx::frac_mod(ctx.fr.time * ctx.fr.speed,
			group.header.dur);
	std::cout << "After group frac_mod: local=" << local << "\n";

    auto it  = group.range.start;
    auto end = group.range.end();

	int i = 0;
    for (; it < end; ++it) {
        mx::usz child = remap_index(it);
        const auto& dur = header(child).dur;

        if (local < dur) {
            ctx.push(child, local / ctx.fr.speed, ctx.fr.speed);
            return;
        }
		local -= dur;
    }
}

auto as::tree::play_tempo(play_ctx& ctx) -> void {
	const auto& tempo = node<as::tempo>(ctx.fr.node);


	// push child node with full duration
	if (!tempo.runtime) {
		ctx.push(tempo.child, ctx.fr.time, ctx.fr.speed);
		return;
	}

	ctx.push(tempo.child,
			ctx.fr.time,
			ctx.fr.speed * tempo.factor);
}



auto as::tree::play_reference(play_ctx& ctx) -> void {
	const auto& refs = node<as::references>(ctx.fr.node);

		   auto it = refs.ref_start;
		   auto tk = refs.tok_start;
	const auto end = refs.ref_start + refs.count;

	std::cout << "Before reference frac_mod: ctx.fr.time=" << ctx.fr.time
			  << ", ctx.fr.speed=" << ctx.fr.speed << "\n";
	auto local = mx::frac_mod(ctx.fr.time * ctx.fr.speed,
								refs.header.dur);
	std::cout << "After reference frac_mod: local=" << local << "\n";


	// loop over referenced nodes
	for (; it < end; ++it, ++tk) {
		const auto node = ref_at(it);
		const auto& dur = header(node).dur;

		if (local < dur) {
			// push node and local tick
			ctx.push(node, local / ctx.fr.speed, ctx.fr.speed);
			break;
		}
		// decrement tick
		local -= dur;
	}

	return;
	auto tv = tokens->filtered_view(tk);

	highlight(ctx.hi, tv, "Underlined");
}

auto as::tree::play_parallel(play_ctx& ctx) -> void {

	const auto& parallel = node<as::parallel>(ctx.fr.node);

		   auto it = parallel.range.start;
	const auto end = parallel.range.end();

	std::cout << "Before parallel frac_mod: ctx.fr.time=" << ctx.fr.time
			  << ", ctx.fr.speed=" << ctx.fr.speed << "\n";
	auto local = mx::frac_mod(ctx.fr.time * ctx.fr.speed,
								parallel.header.dur);
	std::cout << "After parallel frac_mod: local=" << local << "\n";

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = remap_index(it);
		const auto& dur = header(node).dur;

		// time inside this child's cycle
		std::cout << "Before child frac_mod: local=" << local
				  << ", dur=" << dur << "\n";
        mx::frac child_time = mx::frac_mod(local, dur);
		std::cout << "After child frac_mod: child_time=" << child_time << "\n";
		// push child node and local tick
		ctx.push(node, child_time / ctx.fr.speed, ctx.fr.speed);
	}
}



auto as::tree::play_track(play_ctx& ctx) -> void {

	//ctx.mark = ctx.stack.size();
	//ctx.events.emplace_back();

    auto& track = node<as::track>(ctx.fr.node);

	std::cout << "Before track frac_mod: ctx.fr.time=" << ctx.fr.time
			  << ", ctx.fr.speed=" << ctx.fr.speed << "\n";
	auto local = mx::frac_mod(ctx.fr.time * ctx.fr.speed,
							  track.header.dur);
	std::cout << "After track frac_mod: local=" << local << "\n";


    for (mx::usz p = 0U; p < pa::max_params; ++p) {

        const auto pnode = track.params[p];
        if (!pnode)
            continue;

		const auto& param = node<as::parameter>(pnode);

        const auto& total = param.header.dur;

		std::cout << "Before parameter frac_mod: local=" << local
				  << ", total=" << total << "\n";
        auto time = mx::frac_mod(local, total);
		std::cout << "After parameter frac_mod: time=" << time << "\n";

        auto it  = param.range.start;
        auto end = param.range.end();

        for (; it < end; ++it) {
            mx::usz child = remap_index(it);
            const auto& dur = header(child).dur;

            if (time < dur) {
                ctx.push(child, time / ctx.fr.speed, ctx.fr.speed);
                break;
            }
            time -= dur;
        }
    }
}


auto as::tree::play(std::stringstream& hi, const mx::frac& time) -> void {

	play_ctx ctx{hi};

	// get root node
	const auto& root = node<as::program>(0U);

		   auto it = root.range.start;
	const auto end = root.range.end();


	// loop over child nodes
	for (; it < end; ++it) {
		const auto node  = remap_index(it);
		const auto& dur = header(node).dur;

		// push node and local tick

		std::cout << "before frac_mod play: time=" << time << ", dur=" << dur << "\n";
		ctx.push(node, mx::frac_mod(time, dur), {1U, 1U});
		std::cout << "after frac_mod play: ctx.fr.time=" << ctx.fr.time << "\n";

		while (ctx.stack.empty() == false) {

			ctx.pop();

			//if (ctx.mark == ctx.stack.size()) {
			//	std::cout << "At mark, checking events\n";
			//	if (!ctx.events.back().has_trig()) {
			//		//ctx.events.pop_back();
			//		std::cout << "No event triggered, popping event\n";
			//	}
			//	ctx.mark = (mx::usz)-1;
			//}


			// get type of node
			const auto type = header(ctx.fr.node).type;

			switch (type) {

				case as::type::atomic_values:
                    play_atomic(ctx);
                    break;

                case as::type::group:
                    play_group(ctx);
                    break;

                case as::type::parallel:
                    play_parallel(ctx);
                    break;

				case as::type::crossfade:
					continue;
					play_crossfade(ctx);
					break;

                case as::type::track:
                    play_track(ctx);
                    break;

                case as::type::parameter:
					//play_parameter(ctx);
					std::cout << "Parameter play is handled in track\n";
                    break;

                case as::type::references:
					play_reference(ctx);
                    break;

                case as::type::tempo:
                    play_tempo(ctx);
                    break;

				default:
					throw std::runtime_error("Unhandled node type in play");
			}

		}
	}

	if (ctx.events.empty())
		return;

	//for (auto& ev : ctx.events) {
	//	std::cout << "Event Triggered:\n";
	//}
	//
	//std::cout << "\n\n";
}
