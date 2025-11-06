#ifndef language_ast_param_hpp
#define language_ast_param_hpp

#include "language/tokens.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- P A R A M -----------------------------------------------------------

	class param final {


		private:
		public:

			// -- private members ---------------------------------------------

			tk::token* _token;

			ml::usz _values_start;
			ml::usz _values_count;

		public:

			// -- public lifecycle --------------------------------------------

			param(void) noexcept
			:  _token{nullptr}, _values_start{0U}, _values_count{0U} {
			}

			explicit param(tk::token* tk, const ml::usz vs, const ml::usz vc = 0U) noexcept
			: _token{tk}, _values_start{vs}, _values_count{vc} {
			}


			auto token(void) const noexcept -> tk::token& {
				return *_token;
			}

			// -- public methods ----------------------------------------------

			auto debug(void) const -> void {
				std::cout << "    PARAM: ";
				if (_token == nullptr) {
					std::cout << "//////\n";
				} else {
					std::cout << *_token << "\n";
				}
			}

	}; // class param

} // namespace as

#endif // language_ast_param_hpp
