#ifndef core_string_strcmp_hpp
#define core_string_strcmp_hpp

#include "core/string/strlen.hpp"
#include "core/string/strptr.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* str equal */
	template <typename S1, typename S2>
	constexpr auto str_equal(const S1& s1, const S2& s2) noexcept -> bool {

		const auto l1 = mx::strlen(s1);
		const auto l2 = mx::strlen(s2);

		if (l1 != l2)
			return false;

		const char* p1 = mx::strptr(s1);
		const char* p2 = mx::strptr(s2);

		for (mx::usz i = 0U; i < l1; ++i) {

			if (p1[i] != p2[i])
				return false;
		}

		return true;
	}


	/* str lexicographical */
	template <typename S1, typename S2>
	constexpr auto str_lexicographical(const S1& s1, const S2& s2) noexcept -> int {

		const auto l1 = mx::strlen(s1);
		const auto l2 = mx::strlen(s2);
		const auto min_len = (l1 < l2) ? l1 : l2;

		const char* p1 = mx::strptr(s1);
		const char* p2 = mx::strptr(s2);

		for (mx::usz i = 0U; i < min_len; ++i) {

			if (p1[i] != p2[i])
				return (p1[i] < p2[i]) ? -1 : 1;
		}

		return (l1 == l2) ? 0 : (l1 < l2) ? -1 : 1;
	}




// overload comparisons operators for string-like types

/* equality operator */
template <mx::is_string_like S1, mx::is_string_like S2>
constexpr auto operator==(const S1& s1, const S2& s2) noexcept -> bool {
	return mx::str_equal(s1, s2);
}

/* inequality operator */
template <mx::is_string_like S1, mx::is_string_like S2>
constexpr auto operator!=(const S1& s1, const S2& s2) noexcept -> bool {
	return !mx::str_equal(s1, s2);
}

/* less than operator */
template <mx::is_string_like S1, mx::is_string_like S2>
constexpr auto operator<(const S1& s1, const S2& s2) noexcept -> bool {
	return mx::str_lexicographical(s1, s2) < 0;
}

/* greater than operator */
template <mx::is_string_like S1, mx::is_string_like S2>
constexpr auto operator>(const S1& s1, const S2& s2) noexcept -> bool {
	return mx::str_lexicographical(s1, s2) > 0;
}

/* less than or equal to operator */
template <mx::is_string_like S1, mx::is_string_like S2>
constexpr auto operator<=(const S1& s1, const S2& s2) noexcept -> bool {
	return mx::str_lexicographical(s1, s2) <= 0;
}

/* greater than or equal to operator */
template <mx::is_string_like S1, mx::is_string_like S2>
constexpr auto operator>=(const S1& s1, const S2& s2) noexcept -> bool {
	return mx::str_lexicographical(s1, s2) >= 0;
}

} // namespace mx

#endif // core_string_strcmp_hpp
