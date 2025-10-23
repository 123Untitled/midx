#ifndef ml_language_char_type_hpp
#define ml_language_char_type_hpp

#include "types.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {



	class global_char final {


		private:

			/* character type */
			enum : ml::u8 {

				// control characters
				C_CONTROL,

				// extended characters (utf-8, etc.)
				C_EXTENDED,

				// \n
				C_LINE_FEED,

				// \r
				C_CARRIAGE_RETURN,

				// [ \t]
				C_WHITESPACE,

				// [A-F] hex uppercase and notes
				C_UPPER_HEX,

				// [G] note
				C_UPPER_G,

				// [H-Z] uppercase other
				C_UPPER_OTHER,

				// [ac-f] hex lowercase (without b (binary))
				C_LOWER_HEX,

				// [x] hexadecimal prefix
				C_LOWER_X,

				// [b] binary prefix
				C_LOWER_B,

				// [o] octal prefix
				C_LOWER_O,

				// [g-np-wyz] lowercase other
				C_LOWER_OTHER,

				// 0
				C_ZERO_DIGIT,

				// 1
				C_ONE_DIGIT,

				// 2-7
				C_OCTAL_DIGIT,

				// 8-9
				C_DECIMAL_DIGIT,

				// !
				C_EXCLAMATION_MARK,

				// "
				C_DOUBLE_QUOTE,

				// #
				C_NUMBER_SIGN,

				// $
				C_DOLLAR_SIGN,

				// %
				C_PERCENT,

				// &
				C_AMPERSAND,

				// '
				C_SINGLE_QUOTE,

				// (
				C_PAREN_OPEN,

				// )
				C_PAREN_CLOSE,

				// *
				C_ASTERISK,

				// +
				C_PLUS,

				// ,
				C_COMMA,

				// -
				C_MINUS,

				// .
				C_DOT,

				// /
				C_SLASH,

				// :
				C_COLON,

				// ;
				C_SEMICOLON,

				// <
				C_LESS_THAN,

				// =
				C_EQUAL,

				// >
				C_GREATER_THAN,

				// ?
				C_QUESTION_MARK,

				// @
				C_AT_SIGN,

				// [
				C_BRACKET_OPEN,

				/* \ */
				C_BACKSLASH,

				// ]
				C_BRACKET_CLOSE,

				// ^
				C_CIRCUMFLEX,

				// _
				C_UNDERSCORE,

				// `
				C_ACCENT,

				// {
				C_BRACE_OPEN,

				// |
				C_VERTICAL_BAR,

				// }
				C_BRACE_CLOSE,

				// ~
				C_TILDE,

				// max character type
				C_TYPE_MAX
			};



		public:

			// -- public static members ---------------------------------------

			/* size */
			static constexpr ml::u8 size = C_TYPE_MAX;

			/* table */
			static constexpr ml::u8 table[256U] {

				// 0x00 - 0x07 (invalid)
				C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,

				// 0x08 backspace (invalid)
				C_CONTROL,

				// 0x09 (horizontal tab)
				C_WHITESPACE,

				// 0x0A (line feed)
				C_LINE_FEED,

				// 0x0B - 0x0C vertical tab, form feed (invalid)
				C_CONTROL, C_CONTROL,

				// 0x0D (carriage return)
				C_CARRIAGE_RETURN,

				// 0x0E - 0x1F (invalid)
				C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
				C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
				C_CONTROL, C_CONTROL,

				// 0x20 (space)
				C_WHITESPACE,

				// 0x21 ! (exclamation mark)
				C_EXCLAMATION_MARK,

				// 0x22 " (double quote)
				C_DOUBLE_QUOTE,

				// 0x23 # (number sign)
				C_NUMBER_SIGN,

				// 0x24 $ (dollar sign)
				C_DOLLAR_SIGN,

				// 0x25 % (percent)
				C_PERCENT,

				// 0x26 & (ampersand)
				C_AMPERSAND,

				// 0x27 ' (single quote)
				C_SINGLE_QUOTE,

				// 0x28 ( (parenthesis open)
				C_PAREN_OPEN,

				// 0x29 ) (parenthesis close)
				C_PAREN_CLOSE,

				// 0x2A * (asterisk)
				C_ASTERISK,

				// 0x2B + (plus)
				C_PLUS,

				// 0x2C , (comma)
				C_COMMA,

				// 0x2D - (minus)
				C_MINUS,

				// 0x2E . (dot)
				C_DOT,

				// 0x2F / (slash)
				C_SLASH,

				// 0x30 0 (zero digit)
				C_ZERO_DIGIT,

				// 0x31 1 (one digit)
				C_ONE_DIGIT,

				// 0x32 - 0x37 2 - 7 (octal digit)
				C_OCTAL_DIGIT, C_OCTAL_DIGIT, C_OCTAL_DIGIT, C_OCTAL_DIGIT, C_OCTAL_DIGIT, C_OCTAL_DIGIT,

				// 0x38 - 0x39 8 - 9 (decimal digit)
				C_DECIMAL_DIGIT, C_DECIMAL_DIGIT,

				// 0x3A : (colon)
				C_COLON,

				// 0x3B ; (semicolon)
				C_SEMICOLON,

				// 0x3C < (less than)
				C_LESS_THAN,

				// 0x3D = (equal)
				C_EQUAL,

				// 0x3E > (greater than)
				C_GREATER_THAN,

				// 0x3F ? (question mark)
				C_QUESTION_MARK,

				// 0x40 @ (at sign)
				C_AT_SIGN,

				// 0x41 - 0x46 A - F (hex uppercase)
				C_UPPER_HEX, C_UPPER_HEX, C_UPPER_HEX, C_UPPER_HEX, C_UPPER_HEX, C_UPPER_HEX,

				// 0x47 G (note)
				C_UPPER_G,

				// 0x48 - 0x5A H - Z (uppercase other)
				C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER,
				C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER,
				C_UPPER_OTHER, C_UPPER_OTHER, C_UPPER_OTHER,

				// 0x5B [ (open bracket)
				C_BRACKET_OPEN,

				// 0x5C \ (backslash)
				C_BACKSLASH,

				// 0x5D ] (close bracket)
				C_BRACKET_CLOSE,

				// 0x5E ^ (circumflex)
				C_CIRCUMFLEX,

				// 0x5F _ (underscore)
				C_UNDERSCORE,

				// 0x60 ` (accent)
				C_ACCENT,

				// 0x61 a (lowercase hex)
				C_LOWER_HEX,

				// 0x62 b (binary prefix)
				C_LOWER_B,

				// 0x63 - 0x66 c - f (lowercase hex)
				C_LOWER_HEX, C_LOWER_HEX, C_LOWER_HEX, C_LOWER_HEX,

				// 0x67 - 0x6E g - n (lowercase other)
				C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER,

				// 0x6F o (octal prefix)
				C_LOWER_O,

				// 0x70 - 0x77 p - w (lowercase other)
				C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER, C_LOWER_OTHER,

				// 0x78 x (hex prefix)
				C_LOWER_X,

				// 0x79 - 0x7A y - z (lowercase other)
				C_LOWER_OTHER, C_LOWER_OTHER,

				// 0x7B { (brace open)
				C_BRACE_OPEN,

				// 0x7C | (vertical bar)
				C_VERTICAL_BAR,

				// 0x7D } (brace close)
				C_BRACE_CLOSE,

				// 0x7E ~ (tilde)
				C_TILDE,

				// 0x7F delete (invalid)
				C_CONTROL,

				// 0x80 - 0xFF (invalid)
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
				C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED, C_EXTENDED,
			};

	}; // class global_char

} // namespace ml

#endif //ml_language_char_type_hpp
