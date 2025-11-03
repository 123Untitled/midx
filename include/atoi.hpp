#ifndef language_to_integer_hpp
#define language_to_integer_hpp

#include "types.hpp"
#include "macros.hpp"
#include "byte_range.hpp"
#include "language/tokens.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {



	template <ml::u8 base, auto (to_digit)(const ml::u8) -> ml::u8>
	auto convert(const ml::u8* it, const ml::u8* end) noexcept -> ml::i8 {

		ml::i8 num = 0U;
		constexpr ml::i8 max       = +127;
		constexpr ml::i8 min       = -128;
		constexpr ml::i8 mul_limit = min / base;

		bool neg = false;

		// accumulate as negative to handle -128
		while (it < end) {

			// check -10 multiplication overflow
			if (num < mul_limit) {
				// push error
				num = neg ? min : max;
				break;
			}
			num *= base;
			const ml::i8 digit = to_digit(*it);

			// check subtraction overflow
			if (num < (min + digit)) {
				// push error
				num = neg ? min : max;
				break;
			}

			num -= digit;
			++it;
		}

		// finalize octave value
		if (neg == false) {
			if (num == min) {
				// push error
				num = max;
			}
			else {
				num = -num;
			}
		}
	}


	auto to_integer(tk::token& tk) noexcept -> ml::i8 {

		const ml::u8* it  = tk.lexeme.data;
		const ml::u8* end = tk.lexeme.data
						  + tk.lexeme.size;


		switch (tk.id) {

			// decimal number
			case tk::decimal:
				return convert<10U, [](const ml::u8 c) static constexpr noexcept -> ml::u8 {
					return c - '0';
				}>(it, end);

			// binary number
			case tk::binary:
				return convert<2U, [](const ml::u8 c) static constexpr noexcept -> ml::u8 {
					return c - '0';
				}>(it + 2U, end);

			// octal number
			case tk::octal:
				return convert<8U, [](const ml::u8 c) static constexpr noexcept -> ml::u8 {
					return c - '0';
				}>(it + 2U, end);

			// hexadecimal number
			case tk::hexadecimal:
				return convert<16U, [](const ml::u8 c) static constexpr noexcept -> ml::u8 {
					return  (c >= '0' && c <= '9') ? (c - '0')
						 : ((c >= 'A' && c <= 'F') ? (c - 'A' + 10U)
												   : (c - 'a' + 10U));
				}>(it + 2U, end);

			case tk::note:
				break;
			default:
				break;
		}


		return 0;
	}

			//struct alteration final {
			//	const bool sharp;
			//	const bool flat;
			//};
			//
			//constexpr ml::u8 notes[] {
			//	9U, 11U, 0U, 2U, 4U, 5U, 7U
			//};
			//
			//constexpr alteration alts[] {
			//	{true,  true}, {false, true}, {true, false},
			//	{true,  true}, {false, true}, {true, false},
			//	{true,  true}
			//};
			//
			//const ml::u8* begin = _it;
			//auto value = notes[(c - 'A')];
			//++_it;
			//
			//
			//// -- alteration --------------------------------------------------
			//
			//if (_it < _end) {
			//
			//	if (*_it == '#' && alts[(c - 'A')].sharp == true) {
			//		++_it;
			//		value = (value + 1U) % 12U;
			//	}
			//	else if (*_it == 'b' && alts[(c - 'A')].flat == true) {
			//		++_it;
			//		value = (value + 11U) % 12U;
			//	}
			//}
			//
			//// -- negative octave ---------------------------------------------
			//
			//
			//
			//
			////constexpr ml::u8 note_limit = max / 12U;
			////
			////if (num > note_limit) {
			////	this->push_error<"note value overflow">();
			////}
			//
			////num *= 12U;
			//
			////if (num > (max - value)) {
			////	this->push_error<"note value overflow">();
			////}
			//
			////value += num;
}

#endif // language_to_integer_hpp
