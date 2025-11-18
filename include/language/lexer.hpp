#ifndef language_lexer_hpp
#define language_lexer_hpp

#include "language/tokens/def.hpp"


// -- forward declarations ----------------------------------------------------

namespace tk {
	class tokens;
} // namespace tk

namespace an {
	class diagnostic;
} // namespace an


// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {


	// -- L E X E R -----------------------------------------------------------

	class lexer final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = lx::lexer;


		private:

			// -- private members ---------------------------------------------

			/* iterator */
			const mx::u8* _it;

			/* end iterator */
			const mx::u8* _end;

			/* line */
			mx::uint _line;

			/* column */
			mx::uint _column;

			/* tokens */
			tk::tokens* _tokens;

			/* diagnostic */
			an::diagnostic* _diag;



			// -- private methods ---------------------------------------------

			/* lex */
			auto _lex(void) -> void;

			/* push error */
			auto error(const char*) -> void;
			auto error(const char*, const tk::chunk&) -> void;

			/* byte chunk */
			auto byte_chunk(void) noexcept -> tk::chunk;

			/* new chunk */
			auto new_chunk(const mx::u8*) noexcept -> tk::chunk;

			/* push byte token */
			auto push_byte_token(const tk::id) -> void;


			/* skip ignored */
			auto skip_ignored(void) -> void;


			/* lex identifier */
			auto lex_identifier(void) -> void;

			/* lex note */
			auto lex_note(void) -> void;

			/* lex number */
			auto lex_number(void) -> void;

			/* lex tempo */
			auto lex_tempo(const tk::id) -> void;

			/* lex parameter */
			auto lex_parameter(void) -> void;

			/* lex reference */
			auto lex_reference(void) -> void;


			/* invalid token */
			auto invalid_token(const tk::chunk&) -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			lexer(void) noexcept;


			// -- public methods ----------------------------------------------

			/* lex */
			auto lex(const std::string&, tk::tokens&, an::diagnostic&) -> void;

	}; // class lexer

} // namespace lx

#endif // language_lexer_hpp
