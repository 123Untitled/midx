#ifndef language_to_integer_hpp
#define language_to_integer_hpp

#include "types.hpp"
#include "language/tokens.hpp"
#include "language/diagnostic.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	/* to digit */
	using to_digit = auto (const ml::u8) noexcept -> ml::u8;


	/* to decimal digit */
	inline constexpr auto to_dec_digit(const ml::u8 c) noexcept -> ml::u8 {
		return c - '0';
	}

	/* to hex digit */
	inline constexpr auto to_hex_digit(const ml::u8 c) noexcept -> ml::u8 {
		return  (c >= '0' && c <= '9') ? (c - '0')
			 : ((c >= 'A' && c <= 'F') ? (c - 'A' + 10U)
									   : (c - 'a' + 10U));
	}


	/* convert integer */
	template <ml::u8 base, ml::to_digit tdfn, bool neg>
	auto convert(const tk::token& tk,
				 an::diagnostic& diag) noexcept -> ml::i8 {

		const ml::u8* it  = tk.lexeme.data;
		const ml::u8* end = tk.lexeme.data
						  + tk.lexeme.size;

		ml::i8 num = 0U;
		constexpr ml::i8 max       = +127;
		constexpr ml::i8 min       = -128;
		constexpr ml::i8 mul_limit = min / base;

		// accumulate as negative to handle -128
		while (it < end) {

			// check -10 multiplication overflow
			if (num < mul_limit) {
				// push error
				num = neg ? min : max;
				diag.push_error("integer value overflow", tk);
				break;
			}
			num *= base;
			const ml::i8 digit = tdfn(*it);

			// check subtraction overflow
			if (num < (min + digit)) {
				// push error
				num = neg ? min : max;
				diag.push_error("integer value overflow", tk);
				break;
			}

			num -= digit;
			++it;
		}

		// finalize octave value
		if constexpr (neg == false) {
			if (num == min) {
				// push error
				num = max;
				diag.push_error("integer value overflow", tk);
			}
			else { num = -num; }
		}
		return num;
	}

	/* convert decimal */
	template <bool neg>
	auto convert_dec = &convert<10U, to_dec_digit, neg>;

	/* convert binary */
	template <bool neg>
	auto convert_bin = &convert<2U,  to_dec_digit, neg>;

	/* convert octal */
	template <bool neg>
	auto convert_oct = &convert<8U,  to_dec_digit, neg>;

	/* convert hexadecimal */
	template <bool neg>
	auto convert_hex = &convert<16U, to_hex_digit, neg>;




	template <bool neg = false>
	auto to_integer(tk::token& tk,
					an::diagnostic& diag) noexcept -> ml::i8 {


		switch (tk.id) {

			// decimal number
			case tk::decimal:
				return convert_dec<neg>(tk, diag);

			// binary number
			case tk::binary:
				return convert_bin<neg>(tk, diag);

			// octal number
			case tk::octal:
				return convert_oct<neg>(tk, diag);

			// hexadecimal number
			case tk::hexadecimal:
				return convert_hex<neg>(tk, diag);

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
