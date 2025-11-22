//#include "language/ast/node.hpp"
//#include "language/ast/tree.hpp"
//
//
//// -- A T O M I C  V A L U E S ------------------------------------------------
//
//// -- A T O M I C  R E F S ----------------------------------------------------
//
//
//auto child_count(const as::tree& tree, mx::usz idx) -> mx::usz {
//	const auto& h = tree.header(idx);
//
//	switch (h.type) {
//		case as::type::track: {
//			const auto& n = tree.node<as::track>(idx);
//		}
//
//		//case as::type::parameter: // special case, see below
//		case as::type::group:
//		case as::type::parallel:
//		case as::type::permutation: {
//			const auto& n = tree.node<as::group>(idx);
//			return n.range.count;
//		}
//		case as::type::crossfade: {
//			return 2; // left and right
//		}
//		default:
//			return 0;
//	}
//}
//
//auto child_at(const as::tree& tree, mx::usz node, mx::usz index) -> mx::usz {
//
//	const auto& h = tree.header(node);
//
//	switch (h.type) {
//		//case as::type::track: // special case, see below
//		//case as::type::parameter: // special case, see below
//		case as::type::group:
//		case as::type::parallel:
//		case as::type::permutation: {
//			const auto& n = tree.node<as::group>(node);
//			const mx::usz remap = n.range.start + index;
//			return tree.remap_index(remap);
//		}
//		case as::type::crossfade: {
//			const auto& n = tree.node<as::crossfade>(node);
//			return index == 0
//				? tree.remap_index(n.left)
//				: tree.remap_index(n.right);
//		}
//		default:
//			throw std::runtime_error{"child_at: node type has no children"};
//	}
//}
//
//mx::usz duration_of(const as::tree& tree, mx::usz idx) {
//    const auto& h = tree.header(idx);
//    switch (h.type) {
//        case as::type::atomic_values:
//            return tree.node<as::atomic_values>(idx).duration;
//        //case as::type::atomic_refs:
//        //    return node<as::atomic_refs>(idx).duration;
//        case as::type::group:
//            //return tree.node<as::group>(idx).duration;
//        case as::type::track:
//            //return tree.node<as::track>(idx).duration;
//        // etc.
//        default:
//            return 0;
//    }
//}
//
//
//void compute_duration(as::tree& tree, mx::usz root) {
//	struct frame {
//		mx::usz node;         // node index
//		mx::usz child_index;  // next child to process
//		mx::usz acc;          // accumulated duration
//	};
//
//    std::vector<frame> stack;
//    stack.push_back(frame{root, 0, 0});
//
//    while (!stack.empty()) {
//
//        frame& f = stack.back();
//        //auto idx = f.node;
//
//        const auto& h = tree.header(f.node);
//
//
//		// leaf nodes
//        if (h.type == as::type::atomic_values) {
//
//			//auto& av = tree.node<as::atomic_values>(f.node);
//			//
//			//av.duration = av.duration; // ou recompute proprement
//			stack.pop_back();
//			continue;
//		}
//		if (h.type == as::type::references) {
//			// references must be visited one time!
//			stack.pop_back();
//			continue;
//		}
//
//            //else if (h.type == type::atomic_refs) {
//            //    auto& ar = node<as::atomic_refs>(idx);
//            //    mx::usz dur = 0;
//            //
//            //    for (mx::usz i = 0; i < ar.count; ++i) {
//            //        mx::usz ref_idx = /* lire l'index référencé i */;
//            //        // On suppose que les références pointent vers des nodes
//            //        // qui auront leur duration calculée avant l'utilisation
//            //        dur += duration_of(ref_idx);
//            //    }
//            //    ar.duration = dur;
//				//stack.pop_back();
//				//continue;
//            //}
//
//
//		// get number of children
//        mx::usz childs = child_count(tree, f.node);
//
//		// push next child if any
//        if (f.child_index < childs) {
//            mx::usz child = child_at(tree, f.node, f.child_index);
//			f.child_index += 1;
//            stack.push_back(frame{child, 0, 0});
//            continue;
//        }
//
//		// all children processed, compute duration
//        mx::usz dur = 0U;
//
//        switch (h.type) {
//			case as::type::group:
//			//case as::type::track:
//			//case as::type::parameter:
//			case as::type::permutation: {
//                // somme des durations des enfants
//                for (mx::usz i = 0U; i < childs; ++i) {
//                    mx::usz child = child_at(tree, f.node, i);
//                    dur += duration_of(tree, child);
//                }
//                break;
//            }
//
//			case as::type::parallel: {
//                // max des durations
//                for (mx::usz i = 0U; i < childs; ++i) {
//                    mx::usz child = child_at(tree, f.node, i);
//                    dur = std::max(dur, duration_of(tree, child));
//                }
//                break;
//            }
//
//			case as::type::crossfade: {
//                // selon ta sémantique
//                break;
//            }
//
//            default:
//                break;
//        }
//
//        // Écrire le résultat dans le node
//        //set_duration(f.node, dur);
//
//        stack.pop_back();
//    }
//}
//
//
//
//
////bool is_leaf_type(as::type t) {
////    return t == as::type::atomic_values ||
////           t == as::type::atomic_refs;
////}
////
//
//
////void set_duration(mx::usz idx, mx::usz dur) {
////    auto& h = header(idx);
////    switch (h.type) {
////        case as::type::atomic_values:
////            node<as::atomic_values>(idx).duration = dur;
////            break;
////        case as::type::sequence:
////            node<as::sequence>(idx).duration = dur;
////            break;
////        // etc.
////        default:
////            break;
////    }
////}
