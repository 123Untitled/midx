#ifndef language_char_class_hpp
#define language_char_class_hpp

#include "types.hpp"


// -- C C  N A M E S P A C E --------------------------------------------------

namespace cc {


	/* is lower */
	inline constexpr auto is_lower(const ml::u8 c) noexcept -> bool {
		return c >= 'a' && c <= 'z';
	}

	/* is upper */
	inline constexpr auto is_upper(const ml::u8 c) noexcept -> bool {
		return c >= 'A' && c <= 'Z';
	}

	/* is note */
	inline constexpr auto is_note(const ml::u8 c) noexcept -> bool {
		return c >= 'A' && c <= 'G';
	}

	/* is digit */
	inline constexpr auto is_digit(const ml::u8 c) noexcept -> bool {
		return c >= '0' && c <= '9';
	}

	/* is xdigit */
	inline constexpr auto is_hex(const ml::u8 c) noexcept -> bool {
		return (c >= '0' && c <= '9')
			|| (c >= 'a' && c <= 'f')
			|| (c >= 'A' && c <= 'F');
	}

	/* is bdigit */
	inline constexpr auto is_binary(const ml::u8 c) noexcept -> bool {
		return c == '0' || c == '1';
	}

	/* is odigit */
	inline constexpr auto is_octal(const ml::u8 c) noexcept -> bool {
		return c >= '0' && c <= '7';
	}

} // namespace cc

#endif // language_char_class_hpp
