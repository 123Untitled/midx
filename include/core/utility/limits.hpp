#ifndef core_utility_limits_hpp
#define core_utility_limits_hpp

#include "core/types.hpp"
#include "core/type_traits/traits.hpp"
#include <type_traits>

#include <iostream>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- L I M I T S ---------------------------------------------------------

	template <typename T>
	class limits final {


		// -- assertions ------------------------------------------------------

		/* requires scalar type */
		static_assert(std::is_scalar_v<T>, "limits requires a scalar type");


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = mx::limits<T>;


			// -- public static methods ---------------------------------------

			/* max bool */
			static consteval auto max(void) noexcept -> T requires mx::is_bool<T> {
				return true;
			}

			/* min bool */
			static consteval auto min(void) noexcept -> T requires mx::is_bool<T> {
				return false;
			}


			/* max integral */
			static consteval auto max(void) noexcept -> T requires (mx::is_integral<T>
																&& !mx::is_bool<T>) {

				// number of bits in type
				constexpr T bits = static_cast<T>((mx::bits<T>) - std::is_signed_v<T>);

				T count = static_cast<T>(0);

				// loop through bits
				for (T i = count; i < bits; ++i)
					// add shifted bit to max
					count |= static_cast<T>(static_cast<T>(1) << i);

				return count;
			}

			/* min integral */
			static consteval auto min(void) noexcept -> T requires (mx::is_integral<T>
																&& !mx::is_bool<T>) {
				T max = self::max();
				return ~max;
			}


			/* max floating point */
			static consteval auto max(void) noexcept -> T requires mx::is_floating_point<T> {

				if constexpr (sizeof(T) == 4U)
					// return IEEE 754 single precision max
					return 0x1.fffffep+127f;

				else if constexpr (sizeof(T) == 8U)
					// return IEEE 754 double precision max
					return 0x1.fffffffffffffp+1023;
			}

			/* min floating point */
			static consteval auto min(void) noexcept -> T requires mx::is_floating_point<T> {

				if constexpr (sizeof(T) == 4U)
					// return IEEE 754 single precision min
					return -0x1.fffffep+127f;

				else if constexpr (sizeof(T) == 8U)
					// return IEEE 754 double precision min
					return -0x1.fffffffffffffp+1023;
			}


			/* digits */
			static consteval auto digits(void) noexcept -> mx::usz {
				return mx::bits<T>;
			}


	}; // class limits


	constexpr auto clz(const unsigned v) noexcept -> int {
		return __builtin_clz(v);
	}

	constexpr auto clz(const unsigned long v) noexcept -> int {
		return __builtin_clzl(v);
	}

	constexpr auto clz(const unsigned long long v) noexcept -> int {
		return __builtin_clzll(v);
	}


	/* count leading zeros */
	template <typename T>
	constexpr auto countl_zero(const T num) noexcept -> mx::usz {

		static_assert(mx::is_integral<T>, "countl_zero only supports integral types");

		if (num == static_cast<T>(0))
			return mx::value_bits<T>;

		if constexpr (sizeof(T) <= sizeof(unsigned int)) {
			return mx::clz(static_cast<unsigned int>(num))
				- (mx::value_bits<unsigned int> - mx::value_bits<T>);
		}
		else if constexpr (sizeof(T) <= sizeof(unsigned long)) {
			return mx::clz(static_cast<unsigned long>(num))
				- (mx::value_bits<unsigned long> - mx::value_bits<T>);
		}
		else if constexpr (sizeof(T) <= sizeof(unsigned long long)) {
			return mx::clz(static_cast<unsigned long long>(num))
				- (mx::value_bits<unsigned long long> - mx::value_bits<T>);
		}
		else {
			static_assert(false, "countl_zero only supports integral types up to 64 bits");
		}
	}


	/* count digits */
	template <mx::is_integral T>
	constexpr auto count_digits(const T value) noexcept -> mx::usz {


		using U = std::make_unsigned_t<T>;

		U n;

		if constexpr (std::is_signed_v<T>) {
			n = value < 0 ? static_cast<U>(~value) + 1U
						  : static_cast<U>( value);
		}
		else {
			n = static_cast<U>(value);
		}

		if (n == 0) return 1;

		// bits used in the number
		const mx::usz bits_used = mx::bits<U> - mx::countl_zero(n);

		// approx log10
		const mx::usz log10 = (bits_used * 1233U) >> 12U;

		static constexpr mx::u64 powers_of_10[] {
			1ULL,
			10ULL,
			100ULL,
			1000ULL,
			10000ULL,
			100000ULL,
			1000000ULL,
			10000000ULL,
			100000000ULL,
			1000000000ULL,
			10000000000ULL,
			100000000000ULL,
			1000000000000ULL,
			10000000000000ULL,
			100000000000000ULL,
			1000000000000000ULL,
			10000000000000000ULL,
			100000000000000000ULL,
			1000000000000000000ULL,
			10000000000000000000ULL
		};

		return n < powers_of_10[log10] ? log10 : log10 + 1U;
	}


} // namespace ms

#endif // core_utility_limits_hpp
