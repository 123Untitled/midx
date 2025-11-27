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

	const auto& a = node<as::atomic_values>(ctx.fr.node);

	const auto time = a.header.mod(ctx.fr.time * ctx.fr.speed);
	const auto step = a.count != 1U ? a.step_from_time(time) : 0U;

	bool edge = true;

	auto hash = ctx.hash_combine(ctx.fr.node, step);

	const auto it = ctx.hashes.find(hash);

	if (it != ctx.hashes.end())
		edge = time < it->second.time;

	ctx.hashes[hash] = as::edge{step, time};

	auto& ev = ctx.events.back();
	const auto value = value_at(a.value_start + step);


	//mx::usz ticks_to_next = 0U;

	if (a.param_id == pa::trig) {

		if (edge && value != 0) {
			ev.trig();
		}
	}
	else {
		if (a.param_id == pa::gate) {
			//mx::frac step_dur = a.header.dur / a.count;
			//mx::frac gate_frac{(mx::usz)value, 100};
			//mx::frac gate_dur = (step_dur * gate_frac).reduce();
			//mx::u64 gate_ticks = (gate_dur.num * MIDI_PPQN) / gate_dur.den;
		}
		//else {
			ev.add(a.param_id, value);
		//}
	}

	const auto tv = tokens->filtered_view(a.token_start + step);
	highlight(ctx.hi, tv, "IncSearch");
}

			//step = a.step_from_time(time);
			//edge = step != it->second.step;



auto as::tree::play_group(play_ctx& ctx) const -> void {

	const auto& g = node<as::group>(ctx.fr.node);

	auto time = g.header.mod(ctx.fr.time * ctx.fr.speed);

		  auto it  = g.range.start;
    const auto end = g.range.end();

    for (; it < end; ++it) {
		   mx::usz node = remap_index(it);
        const auto& dur = header(node).dur;

		if (time < dur) {
			// push node and local tick
			ctx.push(node, (time / ctx.fr.speed), ctx.fr.speed);
			return;
		}
		time -= dur;
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
			speed);
}

auto as::tree::play_modulo(play_ctx& ctx) const -> void {
	const auto& m = node<as::modulo>(ctx.fr.node);

	// push child node with modulated time
	ctx.push(m.child, ctx.fr.time, ctx.fr.speed);
}



auto as::tree::play_reference(play_ctx& ctx) const -> void {
	const auto& r = node<as::references>(ctx.fr.node);

		   auto it = r.ref_start;
		   auto tk = r.tok_start;
	const auto end = r.ref_start + r.count;

	auto time = r.header.mod(ctx.fr.time * ctx.fr.speed);

	// loop over referenced nodes
	for (; it < end; ++it, ++tk) {
		const auto node = ref_at(it);
		const auto& dur = header(node).dur;

		if (time < dur) {
			// push node and local tick
			ctx.push(node, (time / ctx.fr.speed), ctx.fr.speed);
			break;
		}
		time -= dur;
	}

	auto tv = tokens->filtered_view(tk);
	highlight(ctx.hi, tv, "Underlined");
}



auto as::tree::play_track(play_ctx& ctx) const -> void {

    const auto& tr = node<as::track>(ctx.fr.node);
	const auto& fr = ctx.fr;

	const auto time = tr.header.mod(fr.time * fr.speed);

	ctx.events.emplace_back();
	const auto mark = ctx.stack.size();

    for (mx::usz i = 0U; i < pa::max_params; ++i) {

        const auto n = tr.params[i];
        if (!n)
            continue;

		const auto& p = node<as::parameter>(n);

        auto pt  = p.header.mod(time);

        auto it  = p.range.start;
        auto end = p.range.end();

        for (; it < end; ++it) {
            const auto node = remap_index(it);
            const auto& dur = header(node).dur;

            if (pt < dur) {
				ctx.push(node, (pt / fr.speed), fr.speed);
				break;
			}
			pt -= dur;
        }
    }

	while (ctx.next_until(mark))
		dispatch_play(ctx);


	ctx.events.back().flush(ctx.engine);
	ctx.events.pop_back();
}

auto as::tree::play_program(play_ctx& ctx) const -> void {

	const auto& p = node<as::program>(ctx.fr.node);

		  auto it  = p.range.start;
	const auto end = p.range.end();

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = remap_index(it);
		const auto& h   = header(node);

		// push node and local tick
		ctx.push(node, h.mod(ctx.fr.time), mx::frac{1U, 1U});
	}
}


auto as::tree::play(std::stringstream& hi,
					mx::midi_engine& engine,
					const mx::frac& time) const -> void {

	as::play_ctx ctx{hi, engine};
	ctx.hashes.swap_now();
	ctx.stack.clear();
	ctx.events.clear();
	ctx.absolute = time;

	// push root node
	ctx.push(0U, time);

	while (ctx.next())
		dispatch_play(ctx);
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

		case as::type::crossfade:
			play_crossfade(ctx);
			break;

		case as::type::track:
			play_track(ctx);
			break;

		case as::type::references:
			play_reference(ctx);
			break;

		case as::type::tempo:
			play_tempo(ctx);
			break;

		case as::type::modulo:
			play_modulo(ctx);
			break;

		case as::type::permutation:
			// not implemented...
			break;

		case as::type::program:
			play_program(ctx);
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

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = remap_index(it);
		const auto& h   = header(node);

		// time inside this child's cycle
        const auto ti = h.mod(time);

		// push child node and local tick
		ctx.push(node, ti / ctx.fr.speed, ctx.fr.speed);
	}
}


auto as::tree::play_crossfade(play_ctx& ctx) const -> void {

	return;
	const auto& cf = node<as::crossfade>(ctx.fr.node);
	const auto& h  = cf.header;

	const auto time = cf.header.mod(ctx.fr.time * ctx.fr.speed);

	// get total duration in ticks
	const auto ticks = h.dur.num * MIDI_PPQN / h.dur.den;
	const auto current = time.num * MIDI_PPQN / time.den;
	//std::cout << "Crossfade total ticks: " << ticks << "\n";
	//std::cout << "Crossfade current tick: " << current << "\n";

	double accumulated = 0.0;

	for (mx::usz i = 0; i < ticks; ++i) {
        // Densité linéaire de 0 à 1
        double density = (ticks > 1) ? static_cast<double>(i) / (ticks - 1) : 0.0;

        // Accumuler la densité
        accumulated += density;

        
		// Placer un 1 si l'accumulateur dépasse 1.0
		if (accumulated >= 1.0) {

			accumulated -= 1.0;      // Réinitialiser l'erreur
			// Push right node
			if (i == current) {
				ctx.push(cf.right, (ctx.fr.time * ctx.fr.speed), ctx.fr.speed);
				break;
			}
		}
		else {
			// Push left node
			if (i == current) {
				ctx.push(cf.left, (ctx.fr.time * ctx.fr.speed), ctx.fr.speed);
				break;
			}
		}
    }


	//totalDuration.numerator * MIDI_PPQN / totalDuration.denominator;
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
