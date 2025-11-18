#ifndef language_to_integer_hpp
#define language_to_integer_hpp

#include "types.hpp"
#include "language/tokens/def.hpp"
#include "language/diagnostic.hpp"
#include "data/sequence.hpp"
#include "language/syntax/specifier.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {

	struct dec final {
		non_instantiable_class(dec);
		static constexpr mx::u8 base = 10U;
		static auto to_digit(const mx::u8 c) noexcept -> mx::u8 {
			return c - '0';
		}
	};
	struct bin final {
		non_instantiable_class(bin);
		static constexpr mx::u8 base = 2U;
		static auto to_digit(const mx::u8 c) noexcept -> mx::u8 {
			return c - '0';
		}
	};
	struct oct final {
		non_instantiable_class(oct);
		static constexpr mx::u8 base = 8U;
		static auto to_digit(const mx::u8 c) noexcept -> mx::u8 {
			return c - '0';
		}
	};
	struct hex final {
		non_instantiable_class(hex);
		static constexpr mx::u8 base = 16U;
		static auto to_digit(const mx::u8 c) noexcept -> mx::u8 {
			return  (c >= '0' && c <= '9') ? (c - '0')
				 : ((c >= 'A' && c <= 'F') ? (c - 'A' + 10U)
										   : (c - 'a' + 10U));
		}
	};


	template <sp::id>
	struct overflow;

	template <>
	struct overflow<sp::id::trig> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 1;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<sp::id::note> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 127;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<sp::id::gate> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 100;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<sp::id::velo> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 127;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<sp::id::octa> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = +10;
		constexpr static mx::i8 min = -10;
		constexpr static bool   neg = true;
	}; // struct overflow

	template <>
	struct overflow<sp::id::semi> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = +127;
		constexpr static mx::i8 min = -128;
		constexpr static bool   neg = true;
	}; // struct overflow

	template <>
	struct overflow<sp::id::chan> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 15;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<sp::id::prob> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 100;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow


	/* convert integer */
	template <typename L, typename B, bool neg>
	auto convert(const tk::token& tk,
				 an::diagnostic& diag) noexcept -> mx::i8 {

		const mx::u8* it  = tk.lexeme.data;
		const mx::u8* end = tk.lexeme.data
						  + tk.lexeme.size;

		if constexpr (B::base != 10U) {
			// skip base prefix
			it += 2U;
		}

		mx::i8 num = 0U;
		constexpr mx::i8 max       = L::max;
		constexpr mx::i8 min       = L::min;
		constexpr mx::i8 mul_limit = neg ? (min / B::base) : max / B::base;

		//std::cout << "min: " << mx::i32(min) << ", max: " << mx::i32(max) << "\n";


		// accumulate as negative to handle -128
		while (it < end) {

			// check multiplication overflow
			if constexpr (neg == true) {
				if (num < mul_limit) {
					// push error
					num = min;
					diag.push("value overflow", tk);
					break;
				}
			}
			if constexpr (neg == false) {
				if (num > mul_limit) {
					// push error
					num = max;
					diag.push("value overflow", tk);
					break;
				}
			}

			num *= B::base;
			const mx::i8 digit = B::to_digit(*it);

			// check subtraction overflow
			if constexpr (neg == true) {
				if (num < (min + digit)) {
					// push error
					num = neg ? min : max;
					diag.push("value overflow", tk);
					break;
				}
				num -= digit;
			}
			if constexpr (neg == false) {
				if (num > (max - digit)) {
					// push error
					num = max;
					diag.push("value overflow", tk);
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
	auto convert_dec = &convert<mx::overflow<I>, mx::dec, neg>;

	/* convert binary */
	template <sp::id I, bool neg>
	auto convert_bin = &convert<mx::overflow<I>, mx::bin, neg>;

	/* convert octal */
	template <sp::id I, bool neg>
	auto convert_oct = &convert<mx::overflow<I>, mx::oct, neg>;

	/* convert hexadecimal */
	template <sp::id I, bool neg>
	auto convert_hex = &convert<mx::overflow<I>, mx::hex, neg>;




	//template <sp::id I, bool neg = false>
	//auto to_integer(tk::token& tk,
	//				mx::sequence& seq,
	//				an::diagnostic& diag) noexcept -> mx::i8 {
	//
	//	std::cout << "ID: " << static_cast<mx::usz>(I) << "\n";
	//	bool is_neg = false;
	//
	//	switch (tk.id) {
	//
	//		// decimal number
	//		case tk::decimal:
	//			return convert_dec<I, neg>(tk, diag);
	//
	//		// binary number
	//		case tk::binary:
	//			return convert_bin<I, neg>(tk, diag);
	//
	//		// octal number
	//		case tk::octal:
	//			return convert_oct<I, neg>(tk, diag);
	//
	//		// hexadecimal number
	//		case tk::hexadecimal:
	//			return convert_hex<I, neg>(tk, diag);
	//
	//		case tk::note:
	//			break;
	//
	//		default:
	//			diag.push_error("invalid value", tk);
	//			tk.id = tk::invalid;
	//			break;
	//	}
	//
	//
	//	return 0;
	//}

			//struct alteration final {
			//	const bool sharp;
			//	const bool flat;
			//};
			//
			//constexpr mx::u8 notes[] {
			//	9U, 11U, 0U, 2U, 4U, 5U, 7U
			//};
			//
			//constexpr alteration alts[] {
			//	{true,  true}, {false, true}, {true, false},
			//	{true,  true}, {false, true}, {true, false},
			//	{true,  true}
			//};
			//
			//const mx::u8* begin = _it;
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
			////constexpr mx::u8 note_limit = max / 12U;
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
