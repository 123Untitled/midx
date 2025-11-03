#ifndef language_parser_hpp
#define language_parser_hpp

#include "byte_range.hpp"
#include "types.hpp"
#include "literal.hpp"
#include <vector>

#include "language/syntax/syntax_layout.hpp"


namespace tk {
	class  token_list;
	struct token;
}


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- forward declarations ------------------------------------------------



	// -- P A R S E R ---------------------------------------------------------

	class parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = pr::parser;

			/* action type */
			using action_type = auto (pr::parser::*)(void) -> void;

		private:

			// -- private members ---------------------------------------------

			/* buffer */
			const ml::u8* _buffer;

			/* tokens */
			tk::token_list* _tokens;

			/* current token */
			tk::token* _current;

			/* state stack */
			std::vector<action_type> _stack;

			/* syntax stack */
			std::vector<sx::block_view_type> _context;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(const ml::byte_range&, tk::token_list&) -> void;

			/* switch state */
			template <action_type>
			auto switch_state(void) noexcept -> void;

			/* push state */
			template <action_type>
			auto push_state(void) -> void;

			/* pop state */
			auto pop_state(void) -> void;

			/* push error */
			template <ml::literal>
			auto push_error(void) -> void;


			/* mark invalid */
			auto mark_invalid(void) noexcept -> void;


			// -- main states -------------------------------------------------

			auto expect_block(void) -> void;
			auto expect_specifier(void) -> void;
			auto expect_identifier(void) -> void;
			auto expect_dot(void) -> void;
			auto expect_parameter(void) -> void;

			auto panic_block(void) -> void;
			auto panic_parameter(void) -> void;


			// -- context states ----------------------------------------------

			template <ml::literal L>
			auto on_nestable_param(void) -> void;

			// block trig
			auto on_block_trig(void) -> void;
			auto on_trig_sequence(void) -> void;
			auto on_trig_repeat(void) -> void;
			auto on_trig_linked(void) -> void;

			// block note
			auto on_block_note(void) -> void;
			auto on_note_sequence(void) -> void;
			auto on_note_repeat(void) -> void;
			auto on_note_linked(void) -> void;

			// block gate
			auto on_block_gate(void) -> void;
			auto on_gate_sequence(void) -> void;
			auto on_gate_repeat(void) -> void;
			auto on_gate_linked(void) -> void;

			// block track
			auto on_block_track(void) -> void;
			auto on_track_trig(void) -> void;
			auto on_track_note(void) -> void;
			auto on_track_gate(void) -> void;


			auto on_block_pattern(void) -> void;
			auto on_block_song(void) -> void;


			static constexpr action_type _actions[sx::max_action] {

				// block trigger
				&self::on_block_trig,
				&self::on_trig_sequence,
				&self::on_trig_repeat,
				&self::on_trig_linked,

				// block note
				&self::on_block_note,
				&self::on_note_sequence,
				&self::on_note_repeat,
				&self::on_note_linked,

				// block gate
				&self::on_block_gate,
				&self::on_gate_sequence,
				&self::on_gate_repeat,
				&self::on_gate_linked,

				// block track
				&self::on_block_track,
				&self::on_nestable_param<"trig">,
				&self::on_nestable_param<"note">,
				&self::on_nestable_param<"gate">,
				//&self::on_track_trig,
				//&self::on_track_note,
				//&self::on_track_gate,

				// block pattern
				&self::on_block_pattern,

				// block song
				&self::on_block_song

			};


	}; // class parser

} // namespace pr

#endif // language_parser_hpp
