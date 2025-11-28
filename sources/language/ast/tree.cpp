#include "language/ast/tree.hpp"

#include <sstream>

#include <numeric>

auto as::highlight(std::stringstream& ss,
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




auto as::tree::play_tempo(play_ctx& ctx) const -> void {

	const auto& t = node<as::tempo>(ctx.fr.node);

	auto time = t.header.mod(ctx.fr.time * ctx.fr.speed);

		  auto  it = t.frac_start;
		  auto  tk = t.token_start;
	const auto end = t.frac_start + t.count;

	for (; it < end; ++it, ++tk) {
		const auto& tempo_frac = frac_at(it);

		// duration of child at this tempo
		const auto child_dur = header(t.child).dur / tempo_frac;

		if (time < child_dur) {
			// push child node with tempo-adjusted speed
			ctx.push(t.child,
					(time / ctx.fr.speed),
					 ctx.fr.speed * tempo_frac);
			break;
		}
		time -= child_dur;
	}

	// highlight
	auto tv = tokens->filtered_view(tk);
	highlight(ctx.hi, tv, "Underlined");
}

auto as::tree::play_modulo(play_ctx& ctx) const -> void {

	const auto& m = node<as::modulo>(ctx.fr.node);


	auto time = m.header.mod(ctx.fr.time * ctx.fr.speed);

		  auto  it = m.frac_start;
		  auto  tk = m.token_start;
	const auto end = m.frac_start + m.count;

	for (; it < end; ++it, ++tk) {
		const auto& frac = frac_at(it);

		if (time < frac) {
			// push child node with modulated time
			ctx.push(m.child,
					(time / ctx.fr.speed),
					 ctx.fr.speed);
			break;
		}
		time -= frac;
	}

	// highlight
	auto tv = tokens->filtered_view(tk);
	highlight(ctx.hi, tv, "Underlined");
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

	as::play_ctx ctx{*this, *tokens, hi, engine};
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

		case as::type::atomics:
			node<as::atomics>(ctx.fr.node).play(ctx);
			break;

		case as::type::group:
			node<as::group>(ctx.fr.node).play(ctx);
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

	  const auto& cf = node<as::crossfade>(ctx.fr.node);

      const auto time = cf.header.mod(ctx.fr.time * ctx.fr.speed);
      const auto progress = time / cf.header.dur;  // 0.0 → 1.0

      const auto mark = ctx.stack.size();

      // Évaluer left
      ctx.events.emplace_back();
      ctx.push(cf.left, time);
      while (ctx.next_until(mark))
          dispatch_play(ctx);
      auto ev_left = std::move(ctx.events.back());
      ctx.events.pop_back();

      // Évaluer right
      ctx.events.emplace_back();
      ctx.push(cf.right, time);
      while (ctx.next_until(mark))
          dispatch_play(ctx);
      auto ev_right = std::move(ctx.events.back());
      ctx.events.pop_back();

      // Mixer et push résultat
      //ctx.events.push_back(mix_events(ev_left, ev_right, progress));
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
