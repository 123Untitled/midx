#ifndef language_to_integer_hpp
#define language_to_integer_hpp

#include "core/types.hpp"
#include "language/tokens/def.hpp"
#include "language/diagnostic.hpp"
#include "language/syntax/parameter.hpp"


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


	template <pa::id>
	struct overflow;

	template <>
	struct overflow<pa::trig> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 1;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<pa::note> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 127;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<pa::gate> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 100;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<pa::velo> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 127;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<pa::octa> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = +10;
		constexpr static mx::i8 min = -10;
		constexpr static bool   neg = true;
	}; // struct overflow

	template <>
	struct overflow<pa::semi> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = +127;
		constexpr static mx::i8 min = -128;
		constexpr static bool   neg = true;
	}; // struct overflow

	template <>
	struct overflow<pa::chan> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 15;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<pa::prob> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 100;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow

	template <>
	struct overflow<pa::dura> {
		non_instantiable_class(overflow);
		constexpr static mx::i8 max = 127;
		constexpr static mx::i8 min = 0;
		constexpr static bool   neg = false;
	}; // struct overflow


	/* convert integer */
	template <pa::id ID, typename BASE, bool NEG>
	auto convert(const tk::token_view& tv,
				 an::diagnostic& diag) noexcept -> mx::i8 {

		// get first chunk
		const auto& ck = tv.first_chunk();


		const mx::u8* it  = ck.lexeme.data;
		const mx::u8* end = ck.lexeme.data
						  + ck.lexeme.size;

		if constexpr (BASE::base != 10U) {
			// skip base prefix
			it += 2U;
		}

		mx::i8 num = 0U;
		constexpr mx::i8 max       = mx::overflow<ID>::max;
		constexpr mx::i8 min       = mx::overflow<ID>::min;
		constexpr mx::i8 mul_limit = NEG ? (min / BASE::base) : max / BASE::base;

		//std::cout << "min: " << mx::i32(min) << ", max: " << mx::i32(max) << "\n";


		// accumulate as negative to handle -128
		while (it < end) {

			// check multiplication overflow
			if constexpr (NEG == true) {
				if (num < mul_limit) {
					// push error
					num = min;
					diag.push("value overflow", ck.range);
					tv.make_invalid();
					break;
				}
			}
			if constexpr (NEG == false) {
				if (num > mul_limit) {
					// push error
					num = max;
					diag.push("value overflow", ck.range);
					tv.make_invalid();
					break;
				}
			}

			num *= BASE::base;
			const mx::i8 digit = BASE::to_digit(*it);

			// check subtraction overflow
			if constexpr (NEG == true) {
				if (num < (min + digit)) {
					// push error
					num = min;
					diag.push("value overflow", ck.range);
					tv.make_invalid();
					break;
				}
				num -= digit;
			}
			if constexpr (NEG == false) {
				if (num > (max - digit)) {
					// push error
					num = max;
					diag.push("value overflow", ck.range);
					tv.make_invalid();
					break;
				}
				num += digit;
			}

			++it;
		}
		return num;
	}

	using convert_fn = auto (*)(const tk::token_view&,
									an::diagnostic&) noexcept -> mx::i8;

	/* convert decimal */
	template <bool NEG>
	mx::convert_fn convert_dec[] {
		&mx::convert<pa::trig, mx::dec, NEG>,
		&mx::convert<pa::note, mx::dec, NEG>,
		&mx::convert<pa::gate, mx::dec, NEG>,
		&mx::convert<pa::velo, mx::dec, NEG>,
		&mx::convert<pa::octa, mx::dec, NEG>,
		&mx::convert<pa::semi, mx::dec, NEG>,
		&mx::convert<pa::chan, mx::dec, NEG>,
		&mx::convert<pa::prob, mx::dec, NEG>,
		&mx::convert<pa::dura, mx::dec, NEG>
	};

	/* convert binary */
	template <bool NEG>
	mx::convert_fn convert_bin[] {
		&mx::convert<pa::trig, mx::bin, NEG>,
		&mx::convert<pa::note, mx::bin, NEG>,
		&mx::convert<pa::gate, mx::bin, NEG>,
		&mx::convert<pa::velo, mx::bin, NEG>,
		&mx::convert<pa::octa, mx::bin, NEG>,
		&mx::convert<pa::semi, mx::bin, NEG>,
		&mx::convert<pa::chan, mx::bin, NEG>,
		&mx::convert<pa::prob, mx::bin, NEG>,
		&mx::convert<pa::dura, mx::bin, NEG>
	};

	/* convert octal */
	template <bool NEG>
	mx::convert_fn convert_oct[] {
		&mx::convert<pa::trig, mx::oct, NEG>,
		&mx::convert<pa::note, mx::oct, NEG>,
		&mx::convert<pa::gate, mx::oct, NEG>,
		&mx::convert<pa::velo, mx::oct, NEG>,
		&mx::convert<pa::octa, mx::oct, NEG>,
		&mx::convert<pa::semi, mx::oct, NEG>,
		&mx::convert<pa::chan, mx::oct, NEG>,
		&mx::convert<pa::prob, mx::oct, NEG>,
		&mx::convert<pa::dura, mx::oct, NEG>
	};

	/* convert hexadecimal */
	template <bool NEG>
	mx::convert_fn convert_hex[] {
		&mx::convert<pa::trig, mx::hex, NEG>,
		&mx::convert<pa::note, mx::hex, NEG>,
		&mx::convert<pa::gate, mx::hex, NEG>,
		&mx::convert<pa::velo, mx::hex, NEG>,
		&mx::convert<pa::octa, mx::hex, NEG>,
		&mx::convert<pa::semi, mx::hex, NEG>,
		&mx::convert<pa::chan, mx::hex, NEG>,
		&mx::convert<pa::prob, mx::hex, NEG>,
		&mx::convert<pa::dura, mx::hex, NEG>
	};





	//auto convert(const tk::token_view& tv,
	//			 an::diagnostic& diag) noexcept -> mx::i8 {
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
