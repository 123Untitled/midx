#ifndef print_hpp
#define print_hpp

#include <format>
#include <iostream>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {

	template <typename... Ts>
	auto print(const std::format_string<Ts...>& fmt, Ts&&... args) -> void {
		std::cout << std::format(fmt, std::forward<Ts>(args)...);
	}

} // namespace mx

#endif // print_hpp
