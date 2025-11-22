#include "language/ast/tree.hpp"

#include <sstream>
#include "time/signature.hpp"


auto as::tree::play_atomic(play_ctx& ctx) const -> void {
	const auto& atomic = node<as::atomic_values>(ctx.fr.node);

	mx::signature s{ctx.fr.num, ctx.fr.den};

	//if (s.is_time(ctx.fr.tick) == false)
	//if (s.is_time(ctx.absolute) == false)
		//return;

	//const auto count = s.count(ctx.absolute);
	//const auto count = s.count(ctx.fr.tick);

	//const auto step = count % atomic.header.duration;
	const auto step = ctx.fr.tick % atomic.header.duration;

	const mx::i8 value = value_at(atomic.value_start + step);

	auto tv = tokens->filtered_view(
		atomic.token_start + step);

	tv.for_each_chunk(
		[](const tk::chunk& ck, std::stringstream& ss) static -> void {
			const char* group = "IncSearch";
			//const char* group = "Underlined";
			const auto& r = ck.range;

			ss << "{\"l\":" << r.ln
					   << ",\"s\":" << r.cs
					   << ",\"e\":" << r.ce
					   << ",\"g\":\"" << group << "\"},";
		}, ctx.hi
	);

}

auto as::tree::play_reference(play_ctx& ctx) const -> void {
	const auto& refs = node<as::references>(ctx.fr.node);

		   auto it = refs.ref_start;
		   auto tk = refs.tok_start;
	const auto end = refs.ref_start + refs.count;
	auto tick = ctx.fr.tick;

	// loop over referenced nodes
	for (; it < end; ++it, ++tk) {
		const auto node = ref_at(it);
		const auto dur  = header(node).duration;

		if (tick < dur) {
			// push node and local tick
			ctx.push(node, tick);
			goto highlight;
			break;
		}
		// decrement tick
		tick -= dur;
	}
	return;

highlight:

	auto tv = tokens->filtered_view(tk);

	tv.for_each_chunk(
		[](const tk::chunk& ck, std::stringstream& ss) static -> void {
			//const char* group = "IncSearch";
			const char* group = "Underlined";
			const auto& r = ck.range;

			ss << "{\"l\":" << r.ln
					   << ",\"s\":" << r.cs
					   << ",\"e\":" << r.ce
					   << ",\"g\":\"" << group << "\"},";
		}, ctx.hi
	);
}


auto as::tree::play_group(play_ctx& ctx) const -> void {

	const auto& group = node<as::group>(ctx.fr.node);

		   auto it = group.range.start;
	const auto end = group.range.end();
	auto tick = ctx.fr.tick;

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = remap_index(it);
		const auto dur  = header(node).duration;

		if (tick < dur) {
			// push node and local tick
			ctx.push(node, tick);
			return;
		}
		else {
			// decrement tick
			tick -= dur;
		}
	}
}


auto as::tree::play_parameter(play_ctx& ctx) const -> void {
	const auto& param = node<as::parameter>(ctx.fr.node);
	// push child node with full duration
	ctx.push(param.child, ctx.fr.tick);
}

auto as::tree::play_tempo(play_ctx& ctx) const -> void {
	const auto& tempo = node<as::tempo>(ctx.fr.node);

	// push child node with full duration
	ctx.push(tempo.child,
			ctx.fr.tick, // * (tempo.den) / (tempo.num),
			ctx.fr.num * tempo.num,
			ctx.fr.den * tempo.den
			);
}



auto as::tree::play_parallel(play_ctx& ctx) const -> void {
	const auto& parallel = node<as::parallel>(ctx.fr.node);

		   auto it = parallel.range.start;
	const auto end = parallel.range.end();
	const auto pdur = header(ctx.fr.node).duration;


	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = remap_index(it);
		const auto dur  = header(node).duration;

		ctx.push(node, ctx.fr.tick % dur);
		continue;

	}
}

auto as::tree::play_track(play_ctx& ctx) const -> void {
    const auto& track = node<as::track>(ctx.fr.node);

    // tick local pour la track
    const auto track_tick = ctx.fr.tick;
	//ctx.absolute = track_tick; // JUST FOR TESTING !!!

    // loop over parameters
    for (mx::usz i = 0U; i < pa::max_params; ++i) {

        const auto& range = track.ranges[i];

        if (range.count == 0U)
            continue;

        // total duration of this param
        mx::usz total = 0;
        for (mx::usz j = 0; j < range.count; ++j)
            total += header(remap_index(range.start + j)).duration;

        // determine which event in this param plays at track_tick
        mx::usz tick = track_tick % total;

        auto it = range.start;
        const auto end = it + range.count;

        for (; it < end; ++it) {
            const auto node = remap_index(it);
            const auto dur  = header(node).duration;

            if (tick < dur) {
                ctx.push(node, tick);
                break;
            }
            tick -= dur;
        }
    }
}


auto as::tree::play(std::stringstream& hi, const mx::usz ticks) const -> void {

	play_ctx ctx{hi, ticks};

	// get root node
	const auto& root = node<as::program>(0U);

		   auto it = root.range.start;
	const auto end = root.range.end();

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = remap_index(it);
		const auto dur = header(node).duration;

		// push node and local tick
		ctx.push(node, ticks % dur, 1U, 1U);

		while (ctx.stack.empty() == false) {

			ctx.pop();

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
					//play_crossfade(ctx);
					break;

                case as::type::track:
                    play_track(ctx);
                    break;

                case as::type::parameter:
					play_parameter(ctx);
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
}



//auto as::tree::play_atomic(play_ctx& ctx) const -> void {
//	const auto& atomic = node<as::atomic_values>(ctx.fr.node);
//
//	mx::signature s{ctx.fr.num, ctx.fr.den};
//
//	//if (s.is_time(ctx.fr.tick) == false)
//	//if (s.is_time(ctx.absolute) == false)
//		//return;
//
//	//const auto count = s.count(ctx.absolute);
//	//const auto count = s.count(ctx.fr.tick);
//
//	//const auto step = count % atomic.header.duration;
//	const auto step = ctx.fr.tick % atomic.header.duration;
//
//	const mx::i8 value = value_at(atomic.value_start + step);
//
//	auto tv = tokens->filtered_view(
//		atomic.token_start + step);
//
//	tv.for_each_chunk(
//		[](const tk::chunk& ck, std::stringstream& ss) static -> void {
//			const char* group = "IncSearch";
//			//const char* group = "Underlined";
//			const auto& r = ck.range;
//
//			ss << "{\"l\":" << r.ln
//					   << ",\"s\":" << r.cs
//					   << ",\"e\":" << r.ce
//					   << ",\"g\":\"" << group << "\"},";
//		}, ctx.hi
//	);
//
//}
//
//auto as::tree::play_reference(play_ctx& ctx) const -> void {
//	const auto& refs = node<as::references>(ctx.fr.node);
//
//		   auto it = refs.ref_start;
//		   auto tk = refs.tok_start;
//	const auto end = refs.ref_start + refs.count;
//	auto tick = ctx.fr.tick;
//
//	// loop over referenced nodes
//	for (; it < end; ++it, ++tk) {
//		const auto node = ref_at(it);
//		const auto dur  = header(node).duration;
//
//		if (tick < dur) {
//			// push node and local tick
//			ctx.push(node, tick);
//			goto highlight;
//			break;
//		}
//		// decrement tick
//		tick -= dur;
//	}
//	return;
//
//highlight:
//
//	auto tv = tokens->filtered_view(tk);
//
//	tv.for_each_chunk(
//		[](const tk::chunk& ck, std::stringstream& ss) static -> void {
//			//const char* group = "IncSearch";
//			const char* group = "Underlined";
//			const auto& r = ck.range;
//
//			ss << "{\"l\":" << r.ln
//					   << ",\"s\":" << r.cs
//					   << ",\"e\":" << r.ce
//					   << ",\"g\":\"" << group << "\"},";
//		}, ctx.hi
//	);
//}
//
//
//auto as::tree::play_group(play_ctx& ctx) const -> void {
//
//	const auto& group = node<as::group>(ctx.fr.node);
//
//		   auto it = group.range.start;
//	const auto end = group.range.end();
//	auto tick = ctx.fr.tick;
//
//	// loop over child nodes
//	for (; it < end; ++it) {
//		const auto node = remap_index(it);
//		const auto dur  = header(node).duration;
//
//		if (tick < dur) {
//			// push node and local tick
//			ctx.push(node, tick);
//			return;
//		}
//		else {
//			// decrement tick
//			tick -= dur;
//		}
//	}
//}
//
//
//auto as::tree::play_parameter(play_ctx& ctx) const -> void {
//	const auto& param = node<as::parameter>(ctx.fr.node);
//	// push child node with full duration
//	ctx.push(param.child, ctx.fr.tick);
//}
//
//auto as::tree::play_tempo(play_ctx& ctx) const -> void {
//	const auto& tempo = node<as::tempo>(ctx.fr.node);
//
//	// push child node with full duration
//	ctx.push(tempo.child,
//			ctx.fr.tick, // * (tempo.den) / (tempo.num),
//			ctx.fr.num * tempo.num,
//			ctx.fr.den * tempo.den
//			);
//}
//
//
//
//auto as::tree::play_parallel(play_ctx& ctx) const -> void {
//	const auto& parallel = node<as::parallel>(ctx.fr.node);
//
//		   auto it = parallel.range.start;
//	const auto end = parallel.range.end();
//	const auto pdur = header(ctx.fr.node).duration;
//
//
//	// loop over child nodes
//	for (; it < end; ++it) {
//		const auto node = remap_index(it);
//		const auto dur  = header(node).duration;
//
//		ctx.push(node, ctx.fr.tick % dur);
//		continue;
//
//	}
//}
//
//auto as::tree::play_track(play_ctx& ctx) const -> void {
//    const auto& track = node<as::track>(ctx.fr.node);
//
//    // tick local pour la track
//    const auto track_tick = ctx.fr.tick;
//	//ctx.absolute = track_tick; // JUST FOR TESTING !!!
//
//    // loop over parameters
//    for (mx::usz i = 0U; i < pa::max_params; ++i) {
//
//        const auto& range = track.ranges[i];
//
//        if (range.count == 0U)
//            continue;
//
//        // total duration of this param
//        mx::usz total = 0;
//        for (mx::usz j = 0; j < range.count; ++j)
//            total += header(remap_index(range.start + j)).duration;
//
//        // determine which event in this param plays at track_tick
//        mx::usz tick = track_tick % total;
//
//        auto it = range.start;
//        const auto end = it + range.count;
//
//        for (; it < end; ++it) {
//            const auto node = remap_index(it);
//            const auto dur  = header(node).duration;
//
//            if (tick < dur) {
//                ctx.push(node, tick);
//                break;
//            }
//            tick -= dur;
//        }
//    }
//}
//
//
//auto as::tree::play(std::stringstream& hi, const mx::usz ticks) const -> void {
//
//	play_ctx ctx{hi, ticks};
//
//	// get root node
//	const auto& root = node<as::program>(0U);
//
//		   auto it = root.range.start;
//	const auto end = root.range.end();
//
//	// loop over child nodes
//	for (; it < end; ++it) {
//		const auto node = remap_index(it);
//		const auto dur = header(node).duration;
//
//		// push node and local tick
//		ctx.push(node, ticks % dur, 1U, 1U);
//
//		while (ctx.stack.empty() == false) {
//
//			ctx.pop();
//
//			// get type of node
//			const auto type = header(ctx.fr.node).type;
//
//			switch (type) {
//
//				case as::type::atomic_values:
//                    play_atomic(ctx);
//                    break;
//
//                case as::type::group:
//                    play_group(ctx);
//                    break;
//
//                case as::type::parallel:
//                    play_parallel(ctx);
//                    break;
//
//				case as::type::crossfade:
//					continue;
//					//play_crossfade(ctx);
//					break;
//
//                case as::type::track:
//                    play_track(ctx);
//                    break;
//
//                case as::type::parameter:
//					play_parameter(ctx);
//                    break;
//
//                case as::type::references:
//					play_reference(ctx);
//                    break;
//
//                case as::type::tempo:
//                    play_tempo(ctx);
//                    break;
//
//				default:
//					throw std::runtime_error("Unhandled node type in play");
//			}
//
//		}
//	}
//}
//



	// for PARALLEL INDEPENDANT TRACK BASED CLOCK
	//const auto step = ctx.fr.tick % pdur;
		// IN WHILE LOOP
		// smaller duration
		//if (dur != pdur) {
		//	ctx.push(node,
		//			//step % dur);
		//			ctx.absolute % dur
		//			); // here not good
		//	continue;
		//}
		//
		//// push node and local tick
		//ctx.push(node, step);
				//ctx.fr.tick % dur);
			//(dur == pdur ?
			// ctx.fr.tick
			// : ctx.absolute)






//auto as::tree::play_crossfade(play_ctx& ctx) const -> void {
//
//    const auto& cf = node<as::crossfade>(ctx.fr.node);
//
//    mx::usz t    = ctx.fr.tick;
//    mx::usz D    = cf.header.duration;
//
//    mx::usz L    = cf.left;
//    mx::usz R    = cf.right;
//
//    mx::usz Ll   = header(L).duration;
//    mx::usz Rl   = header(R).duration;
//
//    double ratio = double(t) / double(D);
//
//    mx::usz left_w  = mx::usz((1.0 - ratio) * D);
//    mx::usz right_w = D - left_w;
//
//    static thread_local std::vector<bool> PL, PR;
//    PL = euclid(left_w, D);
//    PR = euclid(right_w, D);
//
//    if (PL[t]) {
//        ctx.stack.emplace_back(L, t % Ll);
//    }
//    else {
//        ctx.stack.emplace_back(R, t % Rl);
//    }
//}

//std::vector<bool> bresenham(mx::usz left, mx::usz total)
//{
//    std::vector<bool> pattern(total);
//    long error = 2*left - total;
//
//    mx::usz y = 0;
//    for (mx::usz x = 0; x < total; ++x) {
//
//        // TRUE = LEFT, FALSE = RIGHT
//        pattern[x] = (error >= 0);
//
//        if (error >= 0)
//            error += 2*(left - total);
//        else
//            error += 2*left;
//    }
//    return pattern;
//}
//
//auto as::tree::play_crossfade(play_ctx& ctx) const -> void {
//
//    const auto& cf = node<as::crossfade>(ctx.fr.node);
//
//    mx::usz t    = ctx.fr.tick;
//    mx::usz D    = cf.header.duration;
//
//    mx::usz L    = cf.left;
//    mx::usz R    = cf.right;
//
//    mx::usz Ll   = header(L).duration;
//    mx::usz Rl   = header(R).duration;
//
//    // fraction du crossfade
//    double ratio = double(t) / double(D);
//
//    // combien de LEFT et RIGHT
//    mx::usz left_count  = mx::usz((1.0 - ratio) * D);
//    mx::usz right_count = D - left_count;
//
//    // créer le pattern
//    static thread_local std::vector<bool> pattern;
//    pattern = bresenham(left_count, D);
//
//    // choisir l’enfant
//    if (pattern[t]) {
//        ctx.stack.emplace_back(L, t % Ll);
//    } else {
//        ctx.stack.emplace_back(R, t % Rl);
//    }
//}

std::vector<bool> euclid(mx::usz k, mx::usz n)
{
    std::vector<bool> pattern(n, false);
    mx::usz bucket = 0;

    for (mx::usz i = 0; i < n; ++i) {
        bucket += k;
        if (bucket >= n) {
            bucket -= n;
            pattern[i] = true;
        }
    }

    return pattern;
}

