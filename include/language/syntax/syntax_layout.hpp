#ifndef language_syntax_layout_hpp
#define language_syntax_layout_hpp

#include "language/syntax/block_list.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {



	static constexpr auto layout = sx::block_list {

		sx::block{
			sx::entry{&pr::parser::call<>, "track", "tr"},
			sx::param_list{
				sx::entry{&pr::parser::call<>, "trig", "tr", "trigger"},
				sx::entry{&pr::parser::call<>, "note", "nt", "no"},
				sx::entry{&pr::parser::call<>, "gate", "ga"},
				sx::entry{&pr::parser::call<>, "octave", "oc", "oct"},
				sx::entry{&pr::parser::call<>, "velocity", "vl", "vel"},
				sx::entry{&pr::parser::call<>, "prob", "pb", "probability"},

				sx::entry{&pr::parser::call<>, "semitone", "st", "semi"},
				sx::entry{&pr::parser::call<>, "swing", "sw"},

				sx::entry{&pr::parser::call<>, "condition", "cond", "co"}
			}
		}, sx::block{
			sx::entry{&pr::parser::call<>, "note", "nt", "no"},
			sx::param_list{
				sx::entry{&pr::parser::call<>, "sequence", "seq"},
				sx::entry{&pr::parser::call<>, "repeat", "rpt"},
				sx::entry{&pr::parser::call<>, "linked", "lnk"}
			}
		}, sx::block{
			sx::entry{&pr::parser::call<>, "pattern"},
			sx::param_list{
				sx::entry{&pr::parser::call<>, "section", "sec"},
				sx::entry{&pr::parser::call<>, "length", "len"}
			}
		}, sx::block{
			sx::entry{&pr::parser::call<>, "play"},
			sx::param_list{
				sx::entry{&pr::parser::call<>, "timeline", "tl"},
				sx::entry{&pr::parser::call<>, "start"},
				sx::entry{&pr::parser::call<>, "loop"}
			}
		}
	};


	// -- S Y N T A X  N A V I G A T O R --------------------------------------

	class syntax final {


		private:
			using self = sx::syntax;
			std::vector<size_type> _stack;

		public:





		template <ml::literal L>
		auto push_block(void) -> void {
			//constexpr auto bi = layout.block_index(L.data);
			//static_assert(bi != layout.block_not_found, "block not found");
			//_stack.push_back(bi);
		}


	}; // class syntax


} // namespace sx

#endif // language_syntax_syntax_layout_hpp


