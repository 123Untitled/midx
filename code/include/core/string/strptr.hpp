#ifndef core_string_strptr_hpp
#define core_string_strptr_hpp

#include "core/string/is_string.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* strptr */
	template <typename T>
	constexpr auto strptr(const T& str) noexcept -> const char* {

		// require string-like type
		static_assert(mx::is_string_like<T>, "mx::strptr: Unsupported type");

		// pointer or array of char
		if constexpr (mx::is_pointer_of<T, char>
				   || mx::is_array_of<T, char>) {
			return str;
		}

		// string class
		if constexpr (mx::is_string_class<T>) {
			return str.data();
		}
	}

} // namespace mx

#endif // core_string_strptr_hpp
