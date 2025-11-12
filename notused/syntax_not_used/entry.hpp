#ifndef language_syntax_entry_hpp
#define language_syntax_entry_hpp

#include "types.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	using size_type = mx::u16;


	// -- E N T R Y -----------------------------------------------------------

	template <typename T, size_type NALIASES>
	struct entry final {

		T action;
		const char* aliases[NALIASES];


		entry(void) noexcept = delete;

		template <typename... Ts>
		constexpr entry(const T& a, const char* first, const Ts&... rest) noexcept
		: action{a}, aliases{first, rest...} {
		}

	}; // struct entry


	// -- deduction guides ----------------------------------------------------

	template <typename T, typename... Ts>
	entry(const T&, const char*, const Ts&...) -> entry<T, 1U + sizeof...(Ts)>;

} // namespace sx

#endif // language_syntax_entry_hpp
