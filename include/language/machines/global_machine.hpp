#ifndef ml_language_machines_global_machine_hpp
#define ml_language_machines_global_machine_hpp

#include "language/state_machine.hpp"
#include "language/transition.hpp"
#include "language/keyword_library.hpp"

#include "literal.hpp"
#include <array>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {

	enum token_type : ml::u8 {
		TK_BLOCK = 0U,
		TK_IDENTIFIER,
		TK_LABEL,
		TK_COMMENT,
		TK_TYPE_MAX
	};

	class highlight final {
		public:
			ml::u32 line;
			ml::u32 start;
			ml::u32 end;
	};


	// -- forward declarations ------------------------------------------------

	/* parser context */
	class parser_context;


	// -- G L O B A L  M A C H I N E ------------------------------------------

	class global_machine final /*: public ml::state_machine*/ {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::global_machine;


			friend class ml::transition<self>;
			enum state_type : ml::u8;
			enum action_type : ml::u8;


			// -- private members ---------------------------------------------

			/* context */
			ml::parser_context* _ctx;

			/* transition */
			const ml::transition<self>* _tr;

			/* current byte */
			ml::u8 _byte;

			/* last state */
			state_type _last;


			/* buffer */
			//std::string _buffer;
			//
			///* count */
			//ml::usz _count;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			global_machine(void) noexcept;


			// -- public methods ----------------------------------------------

			auto parse(ml::parser_context& ctx) -> void /*override*/;


		private:

			template <action_type...>
			auto _call(void) -> void;

			auto _push_error(void) -> void;
			auto _push_state(void) noexcept -> void;
			auto _pop_state(void) noexcept -> void;
			auto _transition(void) -> void;
			auto _end_of_chunk(void) -> void;
			auto _end_of_file(void) -> void;
			auto _newline(void) noexcept -> void;
			auto _increment(void) noexcept -> void;
			auto _uncount(void) noexcept -> void;
			auto _reset_count(void) noexcept -> void;
			auto _search_block(void) -> void;
			auto _search_label(void) -> void;
			auto _handle_identifier(void) noexcept -> void;
			auto _append_in_buffer(void) noexcept -> void;




			auto _on_trigger(void) -> void{}
			auto _on_note(void) -> void{}
			auto _on_gate(void) -> void{}
			auto _on_track(void) -> void{}
			auto _on_pattern(void) -> void{}
			auto _on_track_note(void) -> void{}
			auto _on_track_gate(void) -> void{}
			auto _on_pattern_track(void) -> void{}


			enum action_type : ml::u8 {
				A_PUSH_ERROR,
				A_PUSH_STATE,
				A_POP_STATE,
				A_TRANSITION,
				A_END_OF_CHUNK,
				A_END_OF_FILE,
				A_NEWLINE,
				A_INCREMENT,
				A_UNCOUT,
				A_RESET_COUNT,
				A_SEARCH_BLOCK,
				A_SEARCH_LABEL,
				A_HANDLE_IDENTIFIER,
				A_APPEND_IN_BUFFER,
				A_TYPE_MAX
			};

			static constexpr
				auto (self::*_actions[A_TYPE_MAX])(void) -> void {
					&self::_push_error,
					&self::_push_state,
					&self::_pop_state,
					&self::_transition,
					&self::_end_of_chunk,
					&self::_end_of_file,
					&self::_newline,
					&self::_increment,
					&self::_uncount,
					&self::_reset_count,
					&self::_search_block,
					&self::_search_label,
					&self::_handle_identifier,
					&self::_append_in_buffer
				};




			// -- private constants -------------------------------------------

			enum char_type : ml::u8 {

				// control characters
				C_CONTROL = 0U,

				// invalid characters
				C_INVALID,

				// extended characters (utf-8, etc.)
				C_EXTENDED,

				// \n
				C_LINE_FEED,

				// \r
				C_CARRIAGE_RETURN,

				// [ \t]
				C_WHITESPACE,

				// [A-Za-z0-9]
				C_IDENTIFIER,

				// #
				C_NUMBER_SIGN,

				// '
				C_SINGLE_QUOTE,

				// :
				C_COLON,

				// [
				C_BRACKET_OPEN,

				/* \ */
				C_BACKSLASH,

				// ]
				C_BRACKET_CLOSE,

				// end of chunk
				C_EOC,

				// end of file
				C_EOF,

				// max character type
				C_TYPE_MAX
			};

			enum state_type : ml::u8 {

				// comment and whitespace states
				S_IN_COMMENT,
				S_EX_LINE_FEED,

				// block states
				S_EX_BLOCK,
				S_EX_BLOCK_NAME,
				S_IN_BLOCK_NAME,
				S_EX_BLOCK_END,

				// identifier states
				S_EX_IDENTIFIER,
				S_IN_IDENTIFIER,
				S_ON_IDENTIFIER_ESCAPE,

				// label states
				S_EX_LABEL_OR_BLOCK,
				S_EX_LABEL_NAME,
				S_IN_LABEL_NAME,

				// max state type
				S_TYPE_MAX
			};

			// -- private static members --------------------------------------


			/* keywords */
			static constexpr ml::keyword_library _keywords {

				kw::block_entry{"track",   &self::_on_pattern,
					kw::label_entry{"note", &self::_on_pattern_track}
				},

				kw::block_entry{"track",     &self::_on_track,
					kw::label_entry{"note",  &self::_on_track_note},
					kw::label_entry{"gate",  &self::_on_track_gate}
				},
				kw::block_entry{"pattern",   &self::_on_pattern,
					kw::label_entry{"track", &self::_on_pattern_track}
				}
			};


			/* state machine */
			static constexpr ml::transition<self> _machine[S_TYPE_MAX][C_TYPE_MAX] {


				// -- S_IN_COMMENT --------------------------------------------
				{
					{ // C_CONTROL
						S_IN_COMMENT,
						"unexpected control character in comment",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_EXTENDED
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_LINE_FEED
						S_IN_COMMENT,
						&self::_call<A_POP_STATE, A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						&self::_call<>
					},

					{ // C_WHITESPACE
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_IDENTIFIER
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_SINGLE_QUOTE
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_COLON
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_BRACKET_OPEN
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_BACKSLASH
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_BRACKET_CLOSE
						S_IN_COMMENT,
						&self::_call<>
					},

					{ // C_EOC
						S_IN_COMMENT, // maybe studies this case
						&self::_call<>
					},

					{ // C_EOF
						S_IN_COMMENT, // maybe studies this case
						&self::_call<>
					}
				},


				// -- S_EX_LINE_FEED ------------------------------------------
				{
					{ // C_CONTROL
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_INVALID
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_EXTENDED
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_LINE_FEED
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						&self::_call<A_NEWLINE>
					},

					{ // C_WHITESPACE
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_IDENTIFIER
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						&self::_call<A_NEWLINE>
					},

					{ // C_SINGLE_QUOTE
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_COLON
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_BRACKET_OPEN
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_BACKSLASH
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_BRACKET_CLOSE
						S_EX_LINE_FEED,
						&self::_call<A_POP_STATE, A_NEWLINE, A_TRANSITION>
					},

					{ // C_EOC
						S_EX_LINE_FEED, // warning !!
						&self::_call<>    // maybe store some bytes ??
					},

					{ // C_EOF
						S_EX_LINE_FEED, // need to call EOF on last state ??
						&self::_call<> // pop + eof
					}
				},

				// -- S_EX_BLOCK ----------------------------------------------
				{
					{ // C_CONTROL
						S_EX_BLOCK,
						"unexpected control character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_EX_BLOCK,
						"block expected",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EXTENDED
						S_EX_BLOCK,
						"block expected",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_LINE_FEED
						S_EX_BLOCK,
						&self::_call<A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_EX_BLOCK,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_WHITESPACE
						S_EX_BLOCK,
						&self::_call<>
					},

					{ // C_IDENTIFIER
						S_EX_BLOCK,
						"block expected",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						S_EX_BLOCK,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_SINGLE_QUOTE
						S_EX_BLOCK,
						"block expected",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_COLON
						S_EX_BLOCK,
						"block expected",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_BRACKET_OPEN
						S_EX_BLOCK_NAME,
						&self::_call<>
					},

					{ // C_BACKSLASH
						S_EX_BLOCK,
						"block expected",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_BRACKET_CLOSE
						S_EX_BLOCK,
						"block expected",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EOC
						S_EX_BLOCK,
						&self::_call<>
					},

					{ // C_EOF
						S_EX_BLOCK,
						&self::_call<>
					}
				},

				// -- S_EX_BLOCK_NAME -----------------------------------------
				{
					{ // C_CONTROL
						S_EX_BLOCK,
						"unexpected control character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_EX_BLOCK,                  // maybe non blocking error
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EXTENDED
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_LINE_FEED
						S_EX_BLOCK_NAME,
						&self::_call<A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_EX_BLOCK_NAME,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_WHITESPACE
						S_EX_BLOCK_NAME,
						&self::_call<>
					},

					{ // C_IDENTIFIER
						S_IN_BLOCK_NAME,
						&self::_call<A_RESET_COUNT, A_INCREMENT>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						S_EX_BLOCK_NAME,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_SINGLE_QUOTE
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_COLON
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_BRACKET_OPEN
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_BACKSLASH
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{
					// C_BRACKET_CLOSE
						S_EX_BLOCK,
						"empty block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EOC
						S_EX_BLOCK_NAME,
						&self::_call<>
					},

					{ // C_EOF
						S_EX_BLOCK_NAME,
						&self::_call<>
					}
				},

				// -- S_IN_BLOCK_NAME -----------------------------------------
				{
					{ // C_CONTROL
						S_EX_BLOCK,
						"unexpected control character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EXTENDED
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_LINE_FEED
						S_EX_BLOCK_END,
						&self::_call<A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_EX_BLOCK_END,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_WHITESPACE
						S_EX_BLOCK_END,
						&self::_call<>
					},

					{ // C_IDENTIFIER
						S_IN_BLOCK_NAME,
						&self::_call<A_INCREMENT>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						S_EX_BLOCK_END,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_SINGLE_QUOTE
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_COLON
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_BRACKET_OPEN
						S_EX_BLOCK_NAME,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BACKSLASH
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BRACKET_CLOSE
						S_EX_IDENTIFIER,
						&self::_call<A_SEARCH_BLOCK>
					},

					{
						// C_EOC
						S_IN_BLOCK_NAME, // maybe store some bytes ??
						&self::_call<>
					},

					{
						// C_EOF
						S_IN_BLOCK_NAME,
						"block not closed",
						&self::_call<A_PUSH_ERROR>
					}
				},


				// -- S_EX_BLOCK_END ------------------------------------------
				{
					{ // C_CONTROL
						S_EX_BLOCK,
						"unexpected control character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EXTENDED
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_LINE_FEED
						S_EX_BLOCK_END,
						&self::_call<A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_EX_BLOCK_END,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_WHITESPACE
						S_EX_BLOCK_END,
						&self::_call<>
					},

					{ // C_IDENTIFIER
						S_EX_BLOCK,
						"invalid block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						S_EX_BLOCK_END,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_SINGLE_QUOTE
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_COLON
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_BRACKET_OPEN
						S_EX_BLOCK_NAME,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BACKSLASH
						S_EX_BLOCK,
						"unexpected character in block name",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BRACKET_CLOSE
						S_EX_IDENTIFIER,
						&self::_call<A_SEARCH_BLOCK>
					},

					{
						// C_EOC
						S_EX_BLOCK_END, // maybe store some bytes ??
						&self::_call<>
					},

					{
						// C_EOF
						S_EX_BLOCK_END,
						"block not closed",
						&self::_call<A_PUSH_ERROR>
					}
				},


				// S_EX_IDENTIFIER
				{
					{ // C_CONTROL
						S_EX_IDENTIFIER,
						"unexpected control character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_EX_IDENTIFIER,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EXTENDED
						S_EX_IDENTIFIER,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_LINE_FEED
						S_EX_IDENTIFIER,
						&self::_call<A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_EX_IDENTIFIER,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_WHITESPACE
						S_EX_IDENTIFIER,
						&self::_call<>
					},

					{ // C_IDENTIFIER
						S_EX_IDENTIFIER,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						S_EX_IDENTIFIER,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_SINGLE_QUOTE
						S_IN_IDENTIFIER,
						&self::_call<A_RESET_COUNT/*, A_INCREMENT*/>
					},

					{ // C_COLON
						S_EX_IDENTIFIER,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_BRACKET_OPEN
						S_EX_IDENTIFIER,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BACKSLASH
						S_EX_IDENTIFIER,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BRACKET_CLOSE
						S_EX_IDENTIFIER,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_EOC
						S_EX_IDENTIFIER,
						&self::_call<>
					},

					{
						// C_EOF
						S_EX_IDENTIFIER,
						"block not identified",
						&self::_call<A_PUSH_ERROR>
					}
				},

				// S_IN_IDENTIFIER
				{
					{ // C_CONTROL
						S_IN_IDENTIFIER,
						"unexpected control character in identifier",
						&self::_call<A_PUSH_ERROR, A_INCREMENT>
					},

					{ // C_INVALID
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_EXTENDED
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_LINE_FEED
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT, A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT, A_PUSH_STATE> // here need to increment in ex_line_feed ??
																// because identifier can contains everything until '
					},

					{ // C_WHITESPACE
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_IDENTIFIER
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_NUMBER_SIGN
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_SINGLE_QUOTE
						S_EX_LABEL_OR_BLOCK,
						&self::_call<A_APPEND_IN_BUFFER, A_HANDLE_IDENTIFIER>
					},

					{ // C_COLON
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_BRACKET_OPEN
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{
						// C_BACKSLASH
						S_ON_IDENTIFIER_ESCAPE,
						&self::_call<A_INCREMENT> // maybe remove escape character
					},

					{
						// C_BRACKET_CLOSE
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{
						// C_EOC
						S_IN_IDENTIFIER,
						&self::_call<A_APPEND_IN_BUFFER, A_RESET_COUNT>
					},

					{
						// C_EOF
						S_IN_IDENTIFIER,
						"unterminated identifier",
						&self::_call<A_PUSH_ERROR>
					}
				},


				// S_ON_IDENTIFIER_ESCAPE
				{
					{ // C_CONTROL
						S_IN_IDENTIFIER,
						"unexpected control character in identifier",
						&self::_call<A_PUSH_ERROR, A_APPEND_IN_BUFFER, A_RESET_COUNT>
					},

					{ // C_INVALID
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_EXTENDED
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_LINE_FEED
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT, A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT, A_PUSH_STATE> // here need to increment in ex_line_feed ??
																// because identifier can contains everything until '
					},

					{ // C_WHITESPACE
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_IDENTIFIER
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_NUMBER_SIGN
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_SINGLE_QUOTE
						S_IN_IDENTIFIER,
						&self::_call<A_UNCOUT, A_APPEND_IN_BUFFER, A_RESET_COUNT>
					},

					{ // C_COLON
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{ // C_BRACKET_OPEN
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{
						// C_BACKSLASH
						S_ON_IDENTIFIER_ESCAPE,
						&self::_call<A_INCREMENT> // maybe remove escape character
					},

					{
						// C_BRACKET_CLOSE
						S_IN_IDENTIFIER,
						&self::_call<A_INCREMENT>
					},

					{
						// C_EOC
						S_ON_IDENTIFIER_ESCAPE, // HERE HANDLE CASE FOR NEXT BUFFER
						&self::_call<A_APPEND_IN_BUFFER, A_RESET_COUNT>
					},

					{
						// C_EOF
						S_IN_IDENTIFIER,
						"unterminated identifier",
						&self::_call<A_PUSH_ERROR>
					}
				},

				// S_EX_LABEL_OR_BLOCK
				{
					{ // C_CONTROL
						S_EX_LABEL_OR_BLOCK,
						"unexpected control character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EXTENDED
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_LINE_FEED
						S_EX_LABEL_OR_BLOCK,
						&self::_call<A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_EX_LABEL_OR_BLOCK,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_WHITESPACE
						S_EX_LABEL_OR_BLOCK,
						&self::_call<>
					},

					{ // C_IDENTIFIER
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						S_EX_LABEL_OR_BLOCK,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_SINGLE_QUOTE
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_COLON
						S_EX_LABEL_NAME,
						&self::_call<>
					},

					{ // C_BRACKET_OPEN
						S_EX_BLOCK_NAME,
						&self::_call<>
					},

					{
						// C_BACKSLASH
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BRACKET_CLOSE
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_EOC
						S_EX_LABEL_OR_BLOCK,
						&self::_call<>
					},

					{
						// C_EOF
						S_EX_LABEL_OR_BLOCK,
						&self::_call<>
					}
				},


				// S_EX_LABEL_NAME
				{
					{ // C_CONTROL
						S_EX_LABEL_NAME,
						"unexpected control character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_EX_LABEL_NAME,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EXTENDED
						S_EX_LABEL_NAME,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_LINE_FEED
						S_EX_LABEL_NAME,
						&self::_call<A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_EX_LABEL_NAME,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_WHITESPACE
						S_EX_LABEL_NAME,
						&self::_call<>
					},

					{ // C_IDENTIFIER
						S_IN_LABEL_NAME,
						&self::_call<A_RESET_COUNT, A_INCREMENT>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						S_EX_LABEL_NAME,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_SINGLE_QUOTE
						S_EX_LABEL_NAME,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_COLON
						S_EX_LABEL_NAME,
						&self::_call<>
					},

					{ // C_BRACKET_OPEN
						S_EX_BLOCK_NAME,
						"unamed label",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BACKSLASH
						S_EX_LABEL_NAME,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_BRACKET_CLOSE
						S_EX_LABEL_NAME,
						"unexpected character",
						&self::_call<A_PUSH_ERROR>
					},

					{
						// C_EOC
						S_EX_LABEL_NAME,
						&self::_call<>
					},

					{
						// C_EOF
						S_EX_LABEL_NAME,
						"unamed label",
						&self::_call<A_PUSH_ERROR>
					}
				},

				// -- S_IN_LABEL_NAME -----------------------------------------
				{
					{ // C_CONTROL
						S_EX_LABEL_OR_BLOCK,
						"unexpected control character in label name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_INVALID
						S_EX_LABEL_OR_BLOCK,
						"unexpected character in label name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_EXTENDED
						S_EX_LABEL_OR_BLOCK,
						"unexpected character in label name",
						&self::_call<A_PUSH_ERROR>
					},

					{ // C_LINE_FEED
						S_EX_LABEL_OR_BLOCK,
						&self::_call<A_SEARCH_LABEL, A_NEWLINE>
					},

					{ // C_CARRIAGE_RETURN
						S_EX_LINE_FEED,
						S_IN_LABEL_NAME,
						&self::_call<A_PUSH_STATE>
					},

					{ // C_WHITESPACE
						S_EX_LABEL_OR_BLOCK,
						&self::_call<A_SEARCH_LABEL>
					},

					{ // C_IDENTIFIER
						S_IN_LABEL_NAME,
						&self::_call<A_INCREMENT>
					},

					{ // C_NUMBER_SIGN
						S_IN_COMMENT,
						S_EX_LABEL_OR_BLOCK,
						&self::_call<A_PUSH_STATE, A_SEARCH_LABEL>
					},

					{ // C_SINGLE_QUOTE
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR, A_SEARCH_LABEL>
					},

					{ // C_COLON
						S_EX_LABEL_NAME,
						&self::_call<A_SEARCH_LABEL>
					},

					{ // C_BRACKET_OPEN
						S_EX_BLOCK_NAME,
						&self::_call<A_SEARCH_LABEL>
					},

					{
						// C_BACKSLASH
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR, A_SEARCH_LABEL>
					},

					{
						// C_BRACKET_CLOSE
						S_EX_LABEL_OR_BLOCK,
						"unexpected character",
						&self::_call<A_PUSH_ERROR, A_SEARCH_LABEL>
					},

					{
						// C_EOC
						S_IN_LABEL_NAME,
						&self::_call<A_APPEND_IN_BUFFER, A_RESET_COUNT>
					},

					{
						// C_EOF
						S_IN_LABEL_NAME,
						&self::_call<A_SEARCH_LABEL>
					}
				},

			};


		private:


			/* character table */
			static constexpr char_type _ctable[256U] {

				// 0x00 - 0x07 (control)
				C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,

				// 0x08 (backspace)
				C_CONTROL,

				// 0x09 (horizontal tab)
				C_WHITESPACE,

				// 0x0A (line feed)
				C_LINE_FEED,

				// 0x0B - 0x0C (vertical tab, form feed)
				C_CONTROL, C_CONTROL,

				// 0x0D (carriage return)
				C_CARRIAGE_RETURN,

				// 0x0E - 0x1F (control)
				C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
				C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
				C_CONTROL, C_CONTROL,

				// 0x20 (space)
				C_WHITESPACE,

				// 0x21 - 0x22 !"
				C_INVALID, C_INVALID,

				// 0x23 #
				C_NUMBER_SIGN,

				// 0x24 - 0x26 $%&
				C_INVALID, C_INVALID, C_INVALID,

				// 0x27 '
				C_SINGLE_QUOTE,

				// 0x28 - 0x2F ()*+,-./
				C_INVALID, C_INVALID, C_INVALID, C_INVALID, C_INVALID, C_INVALID, C_INVALID, C_INVALID,

				// 0x30 - 0x39 0123456789
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,

				// 0x3A :
				C_COLON,

				// 0x3B - 0x40 ;<=>?@
				C_INVALID, C_INVALID, C_INVALID, C_INVALID, C_INVALID, C_INVALID,

				// 0x41 - 0x5A ABCDEFGHIJKLMNOPQRSTUVWXYZ
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER,

				// 0x5B [
				C_BRACKET_OPEN,

				/* 0x5C \ */
				C_BACKSLASH,

				// 0x5D ]
				C_BRACKET_CLOSE,

				// 0x5E - 0x60 ^_`
				C_INVALID, C_INVALID, C_INVALID,

				// 0x61 - 0x7A abcdefghijklmnopqrstuvwxyz
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER, C_IDENTIFIER,
				C_IDENTIFIER, C_IDENTIFIER,

				// 0x7B - 0x7E {|}~
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,

				// 0x7F delete (invalid)
				C_CONTROL,

				// 0x80 - 0xFF (invalid)
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
			};

	}; // class global_machine

} // namespace ml

#endif //ml_language_machines_global_machine_hpp
