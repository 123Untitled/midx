#ifndef language_lexer_hpp
#define language_lexer_hpp

#include "byte_range.hpp"
#include "literal.hpp"
#include "language/tokens.hpp"
#include <vector>


// -- forward declarations ----------------------------------------------------

namespace tk {
	class token_list;
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

			/* current iterator */
			const ml::u8* _head;

			/* limit iterator */
			const ml::u8* _limit;

			/* mark iterator */
			const ml::u8* _mark;

			/* line */
			ml::uint _line;

			/* column base */
			ml::uint _base;

			/* culumn cursor */
			ml::uint _cursor;

			/* tokens */
			tk::token_list* _tokens;

			/* diagnostic */
			an::diagnostic* _diag;



			// -- private methods ---------------------------------------------

			/* lex */
			auto _lex(void) -> void;

			/* push token */
			//template <bool, tk::is_token_class>
			template <bool, tk::id>
			auto push_token(void) -> void;

			/* push byte token */
			//template <tk::is_token_class>
			template <tk::id>
			auto push_byte_token(void) -> void;

			/* push error */
			template <ml::literal>
			auto push_error(void) -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			lexer(void) noexcept;


			// -- public methods ----------------------------------------------

			/* lex */
			auto lex(const ml::byte_range&, tk::token_list&, an::diagnostic&) -> void;

	}; // class lexer

} // namespace lx

#endif // language_lexer_hpp
