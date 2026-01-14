#ifndef core_string_strlen_hpp
#define core_string_strlen_hpp

#include "core/types.hpp"
#include "core/string/is_string.hpp"
#include <string.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* strlen */
	template <typename T>
	constexpr auto strlen(const T& str) noexcept -> mx::usz {

		// require string-like type
		static_assert(mx::is_string_like<T>, "mx::strlen: Unsupported type");

		// pointer of char
		if constexpr (mx::is_pointer_of<T, char>) {

			if consteval {
				mx::usz len = 0U;
				while (str[len] != '\0')
					++len;
				return len;
			}
			else {
				return ::strlen(str);
			}
		}

		// array of char
		if constexpr (mx::is_array_of<T, char>) {
			constexpr auto N = mx::extent<T>;
			return str[N - 1U] == '\0' ? N - 1U : N;
		}

		// string class
		if constexpr (mx::is_string_class<T>) {
			return str.size();
		}
	}

} // namespace mx

#endif // core_string_strlen_hpp
