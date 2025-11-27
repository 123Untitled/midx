#ifndef language_to_integer_hpp
#define language_to_integer_hpp

#include "core/types.hpp"
#include "language/tokens/def.hpp"
#include "language/diagnostic.hpp"
#include "language/syntax/parameter.hpp"

#include "language/lexer/char_class.hpp"

#include "math.hpp"


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
	};


	template <typename T, T MIN, T MAX, typename B, bool NEG>
	auto convert2(const mx::i8* it,
				  const mx::i8* end,
				  T& num,
				 an::diagnostic& diag) noexcept -> bool {

		constexpr T mul_limit = NEG ? (MIN / B::base) : MAX / B::base;

		num = static_cast<T>(0);

		while (it < end) {

			// check multiplication overflow
			if constexpr (NEG == true) {
				if (num < mul_limit) {
					// push error
					num = MIN;
					return false;
				}
			}
			if constexpr (NEG == false) {
				if (num > mul_limit) {
					// push error
					num = MAX;
					return false;
				}
			}

			num *= B::base;
			const T digit = B::to_digit(*it);

			// check subtraction overflow
			if constexpr (NEG == true) {
				if (num < (MIN + digit)) {
					// push error
					num = MIN;
					return false;
				}
				num -= digit;
			}
			if constexpr (NEG == false) {
				if (num > (MAX - digit)) {
					// push error
					num = MAX;
					return false;
				}
				num += digit;
			}

			++it;
		}

		return true;
	}



	inline auto convert_note(const tk::token_view& tv,
								an::diagnostic& diag) noexcept -> mx::i8 {

		constexpr mx::u8 notes[] {
			9U, 11U, 0U, 2U, 4U, 5U, 7U
		};

		const auto& l = tv.first_chunk().lexeme;
		const auto* it  = l.data;
		const auto* end = l.data + l.size;

		auto value = notes[(*it - 'A')];
		++it;

		if (it < end) {
			if (*it == '#') {
				value = (value + 1U) % 12U;
			}
			else if (*it == 'b') {
				value = (value + 11U) % 12U;
			}
		}

		return 0;

		//constexpr mx::u8 note_limit = max / 12U;
		//
		//if (num > note_limit) {
		//	this->push_error<"note value overflow">();
		//}

		//num *= 12U;

		//if (num > (max - value)) {
		//	this->push_error<"note value overflow">();
		//}

		//value += num;
	}


	/* convert integer */
	inline auto to_fraction(const tk::chunk& ck,
							an::diagnostic& diag) noexcept -> mx::frac {

		const mx::u8* it  = ck.lexeme.data;
		const mx::u8* end = ck.lexeme.data
						  + ck.lexeme.size;

		constexpr mx::usz max       = std::numeric_limits<mx::usz>::max();
		constexpr mx::usz mul_limit = max / 10U;

		mx::usz num = 0U;
		mx::usz den = 0U;
		mx::usz pow = 1U;

		for (; it < end && cc::is_digit(*it) == true; ++it) {

			// check multiplication overflow
			if (num > mul_limit)
				goto overflow;

			num *= 10U;
			const mx::usz digit = (*it - '0');

			// check subtraction overflow
			if (num > (max - digit))
				goto overflow;

			num += digit;
		}

		if (it == end || *it != '.')
			return mx::frac{num, 1U};


		// check fractional part
		while (++it < end && cc::is_digit(*it) == true) {

			// check multiplication overflow
			if (den > mul_limit)
				goto overflow;

			den *= 10U;
			pow *= 10U;
			const mx::usz digit = (*it - '0');

			// check subtraction overflow
			if (den > (max - digit))
				goto overflow;

			den += digit;
		}

		// check final multiplication overflow
		if (num > (max / pow))
			goto overflow;

		num *= pow;

		// check final addition overflow
		if (num > (max - den))
			goto overflow;

		return mx::frac{num + den, pow};

		// overflow handler
		overflow:
			diag.push("value overflow", ck.range);
			return mx::frac{1U, 1U};
	}

}

#endif // language_to_integer_hpp
