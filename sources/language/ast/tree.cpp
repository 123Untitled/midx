#include "language/ast/tree.hpp"

#include <sstream>

#include <numeric>

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

auto as::tree::play_atomic(play_ctx& ctx) const -> void {
	auto& atomic = node<as::atomic_values>(ctx.fr.node);

	const auto time = ctx.fr.time * ctx.fr.speed;
	const auto step = atomic.step_from_time(time);

	bool edge = true;

	if (!ctx.fr.diverged) {
		const auto prev      = ctx.fr.prev * ctx.fr.speed;
		const auto prev_step = atomic.step_from_time(prev);
		edge = (step != prev_step);
	}

	const auto value = value_at(atomic.value_start + step);

	if (atomic.param_id == pa::trig) {
		if (edge && value != 0) {
			ctx.events.back().edges[atomic.param_id]  = true;
			ctx.events.back().values[atomic.param_id] = value;
		}
	}
	else {
		ctx.events.back().edges[atomic.param_id]  = edge;
		ctx.events.back().values[atomic.param_id] = value;
	}

	//if (edge) {
	auto tv = tokens->filtered_view(
			atomic.token_start + step);
		highlight(ctx.hi, tv, "IncSearch");
	//}
}




auto as::tree::play_group(play_ctx& ctx) const -> void {

	const auto& g = node<as::group>(ctx.fr.node);

	auto time = g.header.mod(ctx.fr.time * ctx.fr.speed);
	auto prev = g.header.mod(ctx.fr.prev * ctx.fr.speed);

		  auto it  = g.range.start;
    const auto end = g.range.end();

	bool diverged = false;
    for (; it < end; ++it) {
		   mx::usz node = remap_index(it);
        const auto& dur = header(node).dur;

		if (time < dur) {
			// push node and local tick
			ctx.push(node,
					(time / ctx.fr.speed),
					(prev / ctx.fr.speed),
					ctx.fr.speed, diverged);
			return;
		}
		time -= dur;
		if (prev < dur) {
			diverged = true;
		}
		else {
			prev -= dur;
		}
	}
}


auto as::tree::play_tempo(play_ctx& ctx) const -> void {
	const auto& t = node<as::tempo>(ctx.fr.node);

	const auto speed = (t.runtime == true)
					 ? (ctx.fr.speed * t.factor)
					 :  ctx.fr.speed;

	// push child node with full duration
	ctx.push(t.child,
			ctx.fr.time,
			ctx.fr.prev,
			speed);
}



auto as::tree::play_reference(play_ctx& ctx) const -> void {
	const auto& r = node<as::references>(ctx.fr.node);

		   auto it = r.ref_start;
		   auto tk = r.tok_start;
	const auto end = r.ref_start + r.count;

	auto time = r.header.mod(ctx.fr.time * ctx.fr.speed);
	auto prev = r.header.mod(ctx.fr.prev * ctx.fr.speed);


	bool diverged = false;
	// loop over referenced nodes
	for (; it < end; ++it, ++tk) {
		const auto node = ref_at(it);
		const auto& dur = header(node).dur;

		if (time < dur) {
			// push node and local tick
			ctx.push(node,
					(time / ctx.fr.speed),
					(prev / ctx.fr.speed),
					ctx.fr.speed, diverged);
			break;
		}
		time -= dur;
		if (prev < dur) {
			diverged = true;
		}
		else {
			prev -= dur;
		}
	}

	auto tv = tokens->filtered_view(tk);
	highlight(ctx.hi, tv, "Underlined");
}


auto as::tree::play_track(play_ctx& ctx) const -> void {

    const auto& tr = node<as::track>(ctx.fr.node);
	const auto& fr = ctx.fr;

	const auto time = tr.header.mod(fr.time * fr.speed);
	const auto prev = tr.header.mod(fr.prev * fr.speed);

	ctx.events.emplace_back();
	const auto mark = ctx.stack.size();

    for (mx::usz i = 0U; i < pa::max_params; ++i) {

        const auto n = tr.params[i];
        if (!n)
            continue;

		const auto& p = node<as::parameter>(n);

        auto pt  = p.header.mod(time);
		auto pp  = p.header.mod(prev);

        auto it  = p.range.start;
        auto end = p.range.end();

		bool diverged = false;
        for (; it < end; ++it) {
            const auto node = remap_index(it);
            const auto& dur = header(node).dur;

            if (pt < dur) {
				ctx.push(node, (pt / fr.speed),
							   (pp / fr.speed), fr.speed, diverged);
				break;
			}

			pt -= dur;
			if (pp < dur) {
				diverged = true;
			}
			else {
				pp -= dur;
			}
        }
    }

	while (ctx.next_until(mark))
		dispatch_play(ctx);

	// finalize events
	if (!ctx.events.back().has_trig()) {
		ctx.events.pop_back();
		return;
	}
	else {
		mx::i8 ch   = ctx.events.back().values[pa::chan];
		mx::i8 note = ctx.events.back().values[pa::note];
		mx::i8 velo = ctx.events.back().values[pa::velo];

		ctx.engine.note_on(ch, note, velo, 10);
		//static mx::usz count = 0U;
		//std::cout << "Track Event: " << count++ << '\n';
	}
}



auto as::tree::play(std::stringstream& hi,
					mx::midi_engine& engine,
					const mx::frac& time,
						  mx::frac& prev) const -> void {

	as::play_ctx ctx{hi, engine};

	// get root node
	const auto& root = node<as::program>(0U);

	if (prev.num == 0 && prev.den == 0) {
		//prev = root.header.dur - mx::frac{1U, 1U};
		prev = mx::frac{root.header.dur.num
					  * root.header.dur.den - 1,
						root.header.dur.den }.reduce();

	}

		   auto it = root.range.start;
	const auto end = root.range.end();

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = remap_index(it);
		const auto& h   = header(node);

		// push node and local tick
		ctx.push(node, h.mod(time), h.mod(prev), mx::frac{1U, 1U});

		while (ctx.next())
			dispatch_play(ctx);
	}
}


auto as::tree::dispatch_play(play_ctx& ctx) const -> void {

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

		//case as::type::crossfade:
			// not implemented...
			//break;

		case as::type::track:
			play_track(ctx);
			break;

		case as::type::references:
			play_reference(ctx);
			break;

		case as::type::tempo:
			play_tempo(ctx);
			break;

		case as::type::permutation:
			// not implemented...
			break;

		default:
			throw std::runtime_error("Unhandled node type in dispatch_play");
	}
}

auto as::tree::play_parallel(play_ctx& ctx) const -> void {

	const auto& p = node<as::parallel>(ctx.fr.node);

		   auto it = p.range.start;
	const auto end = p.range.end();

	auto time = p.header.mod(ctx.fr.time * ctx.fr.speed);
	auto prev = p.header.mod(ctx.fr.prev * ctx.fr.speed);

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = remap_index(it);
		const auto& h   = header(node);

		// time inside this child's cycle
        const auto ti = h.mod(time);
		const auto pr = h.mod(prev);

		// push child node and local tick
		ctx.push(node,
				ti / ctx.fr.speed,
				pr / ctx.fr.speed,
				ctx.fr.speed);
	}
}



//auto as::tree::play_permutation(play_ctx& ctx) const -> void {
//
//	const auto& perm = node<as::permutation>(ctx.fr.node);
//
//	auto local = mx::frac_mod(ctx.fr.time * ctx.fr.speed,
//							  perm.local);
//							  //perm.header.dur);
//
//		  auto it  = perm.range.start;
//    const auto end = perm.range.end();
//
//    for (; it < end; ++it) {
//        mx::usz child   = remap_index(it);
//        const auto& dur = header(child).dur;
//
//        if (local < dur) {
//            ctx.push(child, (local / ctx.fr.speed), ctx.fr.speed);
//            return;
//        }
//		local -= dur;
//    }
//}















// GOOD IMPLEMENTATION FOR REFERENCE PLAYBACK


