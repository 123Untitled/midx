#ifndef language_to_integer_hpp
#define language_to_integer_hpp

#include "types.hpp"
#include "language/tokens.hpp"
#include "language/diagnostic.hpp"
#include "data/sequence.hpp"
#include "language/syntax/specifier.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {

	struct dec final {
		static constexpr mx::u8 base = 10U;
		non_instantiable_class(dec);
	};
	struct bin final {
		static constexpr mx::u8 base = 2U;
		non_instantiable_class(bin);
	};
	struct oct final {
		static constexpr mx::u8 base = 8U;
		non_instantiable_class(oct);
	};
	struct hex final {
		static constexpr mx::u8 base = 16U;
		non_instantiable_class(hex);
	};


	template <sp::id>
	struct limits;

	template <>
	struct limits<sp::id::trig> {
		non_instantiable_class(limits);
		constexpr static mx::i8 max = 1;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct limits

	template <>
	struct limits<sp::id::note> {
		non_instantiable_class(limits);
		constexpr static mx::i8 max = 127;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct limits

	template <>
	struct limits<sp::id::gate> {
		non_instantiable_class(limits);
		constexpr static mx::i8 max = 100;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct limits

	template <>
	struct limits<sp::id::velo> {
		non_instantiable_class(limits);
		constexpr static mx::i8 max = 127;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct limits

	template <>
	struct limits<sp::id::octa> {
		non_instantiable_class(limits);
		constexpr static mx::i8 max = +10;
		constexpr static mx::i8 min = -10;
		constexpr static bool   neg = true;
	}; // struct limits

	template <>
	struct limits<sp::id::semi> {
		non_instantiable_class(limits);
		constexpr static mx::i8 max = +127;
		constexpr static mx::i8 min = -128;
		constexpr static bool   neg = true;
	}; // struct limits

	template <>
	struct limits<sp::id::chan> {
		non_instantiable_class(limits);
		constexpr static mx::i8 max = 15;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct limits

	template <>
	struct limits<sp::id::prob> {
		non_instantiable_class(limits);
		constexpr static mx::i8 max = 100;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct limits




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
	template <typename L, ml::u8 base, ml::to_digit tdfn, bool neg>
	auto convert(const tk::token& tk,
				 an::diagnostic& diag) noexcept -> ml::i8 {

		const ml::u8* it  = tk.lexeme.data;
		const ml::u8* end = tk.lexeme.data
						  + tk.lexeme.size;

		if constexpr (base != 10U) {
			// skip base prefix
			it += 2U;
		}

		ml::i8 num = 0U;
		constexpr ml::i8 max       = L::max;
		constexpr ml::i8 min       = L::min;
		constexpr ml::i8 mul_limit = neg ? (min / base) : max / base;

		//std::cout << "min: " << ml::i32(min) << ", max: " << ml::i32(max) << "\n";


		// accumulate as negative to handle -128
		while (it < end) {

			// check multiplication overflow
			if constexpr (neg == true) {
				if (num < mul_limit) {
					// push error
					num = min;
					diag.push_error("value overflow", tk);
					break;
				}
			}
			if constexpr (neg == false) {
				if (num > mul_limit) {
					// push error
					num = max;
					diag.push_error("value overflow", tk);
					break;
				}
			}

			num *= base;
			const ml::i8 digit = tdfn(*it);

			// check subtraction overflow
			if constexpr (neg == true) {
				if (num < (min + digit)) {
					// push error
					num = neg ? min : max;
					diag.push_error("value overflow", tk);
					break;
				}
				num -= digit;
			}
			if constexpr (neg == false) {
				if (num > (max - digit)) {
					// push error
					num = max;
					diag.push_error("value overflow", tk);
					break;
				}
				num += digit;
			}

			++it;
		}
		return num;
	}

	/* convert decimal */
	template <sp::id I, bool neg>
	auto convert_dec = &convert<ml::limits<I>, 10U, to_dec_digit, neg>;

	/* convert binary */
	template <sp::id I, bool neg>
	auto convert_bin = &convert<ml::limits<I>, 2U,  to_dec_digit, neg>;

	/* convert octal */
	template <sp::id I, bool neg>
	auto convert_oct = &convert<ml::limits<I>, 8U,  to_dec_digit, neg>;

	/* convert hexadecimal */
	template <sp::id I, bool neg>
	auto convert_hex = &convert<ml::limits<I>, 16U, to_hex_digit, neg>;




	template <sp::id I, bool neg = false>
	auto to_integer(tk::token& tk,
					ml::sequence& seq,
					an::diagnostic& diag) noexcept -> ml::i8 {

		std::cout << "ID: " << static_cast<ml::usz>(I) << "\n";
		bool is_neg = false;

		switch (tk.id) {

			// decimal number
			case tk::decimal:
				return convert_dec<I, neg>(tk, diag);

			// binary number
			case tk::binary:
				return convert_bin<I, neg>(tk, diag);

			// octal number
			case tk::octal:
				return convert_oct<I, neg>(tk, diag);

			// hexadecimal number
			case tk::hexadecimal:
				return convert_hex<I, neg>(tk, diag);

			case tk::note:
				break;

			default:
				diag.push_error("invalid value", tk);
				tk.id = tk::invalid;
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
