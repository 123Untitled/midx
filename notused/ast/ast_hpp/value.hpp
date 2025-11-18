#ifndef language_ast_value_hpp
#define language_ast_value_hpp

#include "language/tokens/def.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- V A L U E -----------------------------------------------------------

	class value final {


		private:

			// -- private members ---------------------------------------------

			/* value */
			tk::token* _value;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			value(void) noexcept
			: _value{nullptr} {
			}

			/* token constructor */
			explicit value(tk::token& tk) noexcept
			: _value{&tk} {
			}


			// -- public accessors --------------------------------------------

			/* token */
			auto token(void) const noexcept -> tk::token& {
				return *_value;
			}


			// -- public methods ----------------------------------------------

			auto debug(void) const -> void {
				std::cout << "        VALUE: " << *_value << '\n';
			}

	}; // class value

} // namespace as

#endif // language_ast_value_hpp
