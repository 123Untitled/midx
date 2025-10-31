#ifndef language_syntax_entry_hpp
#define language_syntax_entry_hpp

#include "language/parser.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	using size_type = ::uint16_t;


	// -- E N T R Y -----------------------------------------------------------

	template <size_type NALIASES>
	struct entry final {

		pr::action action;
		const char* aliases[NALIASES];


		entry(void) noexcept = delete;

		template <typename... Ts>
		constexpr entry(const pr::action& a, const char* first, const Ts&... rest) noexcept
		: action{a}, aliases{first, rest...} {
		}

	}; // struct entry


	// -- deduction guides ----------------------------------------------------

	template <typename... Ts>
	entry(const pr::action&, const char*, const Ts&...) -> entry<1U + sizeof...(Ts)>;

} // namespace sx

#endif // language_syntax_entry_hpp
