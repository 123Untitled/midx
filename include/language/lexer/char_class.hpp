#ifndef language_char_class_hpp
#define language_char_class_hpp

#include "types.hpp"


// -- C C  N A M E S P A C E --------------------------------------------------

namespace cc {


	/* is lower */
	inline constexpr auto is_lower(const mx::u8 c) noexcept -> bool {
		return c >= 'a' && c <= 'z';
	}

	/* is upper */
	inline constexpr auto is_upper(const mx::u8 c) noexcept -> bool {
		return c >= 'A' && c <= 'Z';
	}

	/* is alpha */
	inline constexpr auto is_alpha(const mx::u8 c) noexcept -> bool {
		return cc::is_lower(c) || cc::is_upper(c);
	}

	/* is note */
	inline constexpr auto is_note(const mx::u8 c) noexcept -> bool {
		return c >= 'A' && c <= 'G';
	}

	/* is digit */
	inline constexpr auto is_digit(const mx::u8 c) noexcept -> bool {
		return c >= '0' && c <= '9';
	}

	/* is alnum */
	inline constexpr auto is_alnum(const mx::u8 c) noexcept -> bool {
		return cc::is_alpha(c) || cc::is_digit(c);
	}

	/* is xdigit */
	inline constexpr auto is_hex(const mx::u8 c) noexcept -> bool {
		return (c >= '0' && c <= '9')
			|| (c >= 'a' && c <= 'f')
			|| (c >= 'A' && c <= 'F');
	}

	/* is bdigit */
	inline constexpr auto is_binary(const mx::u8 c) noexcept -> bool {
		return c == '0' || c == '1';
	}

	/* is odigit */
	inline constexpr auto is_octal(const mx::u8 c) noexcept -> bool {
		return c >= '0' && c <= '7';
	}

} // namespace cc

#endif // language_char_class_hpp
