#ifndef language_syntax_layout_hpp
#define language_syntax_layout_hpp

#include "language/syntax/block_list.hpp"
#include "language/syntax/block_view.hpp"


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


	enum action : ml::u8 {

		// block trigger
		on_block_trig,
		on_trig_sequence,
		on_trig_repeat,
		on_trig_linked,

		// block note
		on_block_note,
		on_note_sequence,
		on_note_repeat,
		on_note_linked,

		// block gate
		on_block_gate,
		on_gate_sequence,
		on_gate_repeat,
		on_gate_linked,

		// block track
		on_block_track,
		on_track_trig,
		on_track_note,
		on_track_gate,


		// block pattern
		on_block_pattern,

		// block song
		on_block_song,





		max_action,
		unknown = max_action
	}; // enum action


	static constexpr auto layout = sx::block_list {

		// block trigger
		sx::block{
			sx::entry{sx::action::on_block_trig, "trig", "tr", "trigger"},
			sx::param_list{
				sx::entry{sx::action::on_trig_sequence, "sequence", "seq"},
				sx::entry{sx::action::on_trig_repeat, "repeat", "rpt"},
				sx::entry{sx::action::on_trig_linked, "linked", "lnk"}
			}
		},

		// block note
		sx::block{
			sx::entry{sx::action::on_block_note, "note", "nt", "no"},
			sx::param_list{
				sx::entry{sx::action::on_note_sequence, "sequence", "seq"},
				sx::entry{sx::action::on_note_repeat, "repeat", "rpt"},
				sx::entry{sx::action::on_note_linked, "linked", "lnk"}
			}
		},

		// block gate
		sx::block{
			sx::entry{sx::action::on_block_gate, "gate", "ga"},
			sx::param_list{
				sx::entry{sx::action::on_gate_sequence, "sequence", "seq"},
				sx::entry{sx::action::on_gate_repeat, "repeat", "rpt"},
				sx::entry{sx::action::on_gate_linked, "linked", "lnk"}
			}
		},

		// block track
		sx::block{
			sx::entry{sx::action::on_block_track, "track"},
			sx::param_list{
				sx::entry{sx::action::on_track_trig, "trig", "tr", "trigger"},
				sx::entry{sx::action::on_track_note, "note", "nt", "no"},
				sx::entry{sx::action::on_track_gate, "gate", "ga"},
			}
		}

		// block pattern
		//sx::block{
		//	sx::entry{sx::action::on_block_note, "pattern", "ptn"},
		//	sx::param_list{
		//		sx::entry{sx::action::on_param_sequence, "sequence", "seq"},
		//		sx::entry{sx::action::on_param_repeat, "repeat", "rpt"},
		//		sx::entry{sx::action::on_param_linked, "linked", "lnk"}
		//	}
		//}
	};

	using layout_type = std::remove_cvref<decltype(layout)>::type;
	using block_view_type = sx::block_view<sx::layout_type>;


	template <ml::literal L>
	consteval auto request_block(void) noexcept -> block_view_type {
		constexpr auto bv = sx::layout.template find_block<L>();
		//constexpr auto bv = sx::layout.find_block(l);
		static_assert(!bv.block_not_found(), "requested block not found in layout");
		return bv;
	}

} // namespace sx

#endif // language_syntax_syntax_layout_hpp



