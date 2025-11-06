#ifndef language_syntax_layout_hpp
#define language_syntax_layout_hpp

#include "language/syntax/block_list.hpp"
#include "language/syntax/block_view.hpp"

#include "language/ast/tree.hpp"

#include "time/signature.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {

	// NOTE, TRIG, GATE, OCTAVE, VELOCITY, PROB, SEMITONE, SWING, CONDITION, CHANNEL
	// sequence, repeat, linked

	// TRACK
	// trig, note, gate, octave, velocity, prob, semitone, swing, condition, channel

	// PATTERN
	// section, length

	// SONG
	// timeline, start, loop


	template <ml::u8 DEFAULT>
	class sequence {
		public:

			//A   (B  (C  D))  E  F _ _ _
			//100 (80 (29 11)) 20 28 100_ _ _

			struct slot final {
				public:
					ml::u8 value;
					ml::signature sign;
					tk::token* token;
					ml::usz depth;
			};

			std::vector<slot> _slots;

			auto parse(const as::param_view& pv) -> void {

				for (const auto& v : pv.values()) {


				}
			}
	};


	class builder {

		public:


			auto empty(void) const noexcept -> void {
			}


			static constexpr auto layout = sx::block_list {

				// block trigger
				sx::block{
					sx::entry{&sx::builder::empty, "trig", "tr", "trigger"},
					sx::param_list{
						sx::entry{&sx::builder::empty, "sequence", "seq"},
						sx::entry{&sx::builder::empty, "repeat", "rpt"},
						sx::entry{&sx::builder::empty, "linked", "lnk"}
					}
				},

				// block note
				sx::block{
					sx::entry{&sx::builder::empty, "note", "nt", "no"},
					sx::param_list{
						sx::entry{&sx::builder::empty, "sequence", "seq"},
						sx::entry{&sx::builder::empty, "repeat", "rpt"},
						sx::entry{&sx::builder::empty, "linked", "lnk"}
					}
				},


				// block gate
				sx::block{
					sx::entry{&sx::builder::empty, "gate", "ga"},
					sx::param_list{
						sx::entry{&sx::builder::empty, "sequence", "seq"},
						sx::entry{&sx::builder::empty, "repeat", "rpt"},
						sx::entry{&sx::builder::empty, "linked", "lnk"}
					}
				},

				// block track
				sx::block{
					sx::entry{&sx::builder::empty, "track"},
					sx::param_list{
						sx::entry{&sx::builder::empty, "trig", "tr", "trigger"},
						sx::entry{&sx::builder::empty, "note", "nt", "no"},
						sx::entry{&sx::builder::empty, "gate", "ga"},
					}
				},

				// block pattern
				sx::block{
					sx::entry{&sx::builder::empty, "pattern", "ptn"},
					sx::param_list{
						sx::entry{&sx::builder::empty, "sequence", "seq"},
						sx::entry{&sx::builder::empty, "repeat", "rpt"},
						sx::entry{&sx::builder::empty, "linked", "lnk"}
					}
				}
			};


			auto build(const as::tree& tree) -> void {

				for (const auto& b : tree.blocks()) {

					auto& spec = b.block().specifier();

					auto bv = layout.find_block(spec.lexeme);

					if (bv.block_not_found()) {
						std::cout << "block not \x1b[31mfound\x1b[0m: " << spec.lexeme << "\n";
						spec.id = tk::invalid;
						continue;
					}
					else {
						std::cout << "block \x1b[32mfound\x1b[0m: " << spec.lexeme << "\n";
					}


					for (const auto& p : b.params()) {

						auto& tk = p.param().token();

						auto pv = bv.find_param(tk.lexeme);

						if (pv.param_not_found()) {
							std::cout << "  param not \x1b[31mfound\x1b[0m: " << tk.lexeme << "\n";
							tk.id = tk::invalid;
							continue;
						}
						else {
							std::cout << "  param \x1b[32mfound\x1b[0m: " << tk.lexeme << "\n";
						}

						for (const auto& v : p.values()) {



						}
					}
				}
			}
	};


	//using layout_type = std::remove_cvref<decltype(layout)>::type;
	//using block_view_type = sx::block_view<sx::layout_type>;
	//
	//
	//template <ml::literal L>
	//consteval auto request_block(void) noexcept -> block_view_type {
	//	constexpr auto bv = sx::layout.template find_block<L>();
	//	//constexpr auto bv = sx::layout.find_block(l);
	//	static_assert(!bv.block_not_found(), "requested block not found in layout");
	//	return bv;
	//}

} // namespace sx

#endif // language_syntax_syntax_layout_hpp



