#include "language/ast/node.hpp"
#include "language/ast.hpp"


// -- A T O M I C  V A L U E S ------------------------------------------------

// -- A T O M I C  R E F S ----------------------------------------------------


auto duration_switch(as::ctx& ctx,
					 const as::node& node) noexcept -> mx::usz {

	const auto mark = ctx.stack.size();
	ctx.stack.push_back(&node);

	mx::usz dur = 0U;

	while (ctx.stack.size() > mark) {

		const as::node& n = *ctx.stack.back();
		ctx.stack.pop_back();

		switch (n.type) {

			// atomic values
			case as::type::atomic_values:
				dur += n.data.values.duration;
				break;

			// atomic refs
			case as::type::atomic_refs: {
					auto is = n.data.refs.value_start;
				const auto ic = is + n.data.refs.count;

				for (; is < ic; ++is) {
					const as::node& rn = ctx.arena.ref_at(is);
					ctx.stack.push_back(&rn);
				}
				break;
			}

			// other types....

			default:
				break;
		}

	}

	return dur;
}



auto compute_all_durations(as::arena& arena, mx::usz root) -> void {

    struct frame {
        mx::usz index;   // node index
        mx::usz child;   // next child to visit
    };

    std::vector<frame> stack;
    stack.reserve(128);

    stack.push_back({root, 0});

    while (!stack.empty()) {

        frame& f = stack.back();
        as::node& n = arena.node(f.index);

        // --- 1) Descente tant qu’il reste des enfants non visités ---
        if (f.child < n.count) {

            //mx::usz child_index = arena.remap(n.start + f.child);
            f.child++; // avancer le curseur

            //stack.push_back({child_index, 0});
            continue;
        }

        // --- 2) Tous les enfants sont visités → on calcule la durée ---
        mx::usz dur = 0;

        switch (n.type) {

            //case as::type::leaf_sequence: {
            //    dur = n.count;   // simple
            //    break;
            //}

            //case as::type::leaf_reference: {
            //    dur = arena.node(n.ref_index).duration;
            //    break;
            //}

            case as::type::sequence:
            case as::type::track:
            //case as::type::group:
            case as::type::permutation: {
                // somme des durées des enfants
                for (mx::usz i = 0; i < n.count; ++i) {
                    //mx::usz ci = arena.remap(n.start + i);
                    //dur += arena.node(ci).duration;
                }
                break;
            }

            case as::type::parallel: {
                // max des durées des deux enfants
                //dur = std::max(
                    //arena.node(arena.remap(n.start + 0)).duration,
                    //arena.node(arena.remap(n.start + 1)).duration
                //);
                break;
            }

            case as::type::crossfade: {
                // durée = min(left,right) ou selon ta règle
                //mx::usz a = arena.node(arena.remap(n.start + 0)).duration;
                //mx::usz b = arena.node(arena.remap(n.start + 1)).duration;
                //dur = std::min(a,b);
                break;
            }

            default:
                dur = 0;
                break;
        }

        // assignation finale
        //n.duration = dur;

        // et on remonte
        stack.pop_back();
    }
}
