#ifndef language_lexer_hpp
#define language_lexer_hpp

#include "literal.hpp"
#include "language/tokens.hpp"
//#include "language/char_map.hpp"
#include <vector>


// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {


	// -- forward declarations ------------------------------------------------

	class table;
	class lexer;
	class context;
	using action = auto (lexer::*)(void) -> void;




	// -- L E X E R -----------------------------------------------------------

	class lexer final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = lx::lexer;


		private:

			// -- private members ---------------------------------------------


			const ml::u8* _it;

			const ml::u8* _end;

			const ml::u8* _checkpoint;

			ml::uint _line;

			ml::uint _column;

			/* state */
			const lx::table* _state;

			std::vector<tk::token> _tokens;

			std::vector<std::string> _errors;


			auto _atoi(void) noexcept -> ml::i8;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			lexer(void) noexcept;


			// -- public methods ----------------------------------------------

			auto lex(lx::context&) -> void;
			auto lex2(lx::context&) -> void;


			/* call actions */
			template <lx::action...>
			auto call(void) -> void;


			/* switch state */
			template <typename>
			auto switch_state(void) noexcept -> void;

			/* push state */
			template <typename>
			auto push_state(void) -> void;

			/* pop state */
			auto pop_state(void) -> void;


			/* checkpoint */
			auto checkpoint(void) noexcept -> void;

			/* newline */
			auto newline(void) noexcept -> void;

			/* count */
			auto count(void) noexcept -> void;

			/* reset count */
			auto reset_count(void) noexcept -> void;


			/* push token */
			template <tk::is_token_class, ml::uint = 0U>
			auto push_token(void) -> void;

			template <tk::is_token_class>
			auto push_token2(void) -> void;

			template <tk::is_token_class, typename... Tp>
			auto push_token(const Tp&...) -> void;

			template <tk::is_token_class>
			auto push_token_byte(void) -> void;

			template <tk::is_token_class>
			auto push_byte_token(void) -> void;

			/* push error */
			template <ml::literal>
			auto push_error(void) -> void;


			auto end_of_file(void) -> void;


	}; // class lexer

} // namespace lx

#endif // language_lexer_hpp



			/* context */
			//lx::context* _ctx;


