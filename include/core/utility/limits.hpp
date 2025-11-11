#ifndef core_utility_limits_hpp
#define core_utility_limits_hpp

#include "core/types.hpp"
#include "core/type_traits/traits.hpp"
#include <type_traits>

//#include <intrin.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- L I M I T S ---------------------------------------------------------

	template <typename T>
	class limits final {


		// -- assertions ------------------------------------------------------

		/* requires scalar type */
		static_assert(std::is_scalar_v<T>, "limits requires a scalar type");


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = ms::limits<T>;


			// -- public static methods ---------------------------------------

			/* max bool */
			static consteval auto max(void) noexcept -> T requires ms::is_bool<T> {
				return true;
			}

			/* min bool */
			static consteval auto min(void) noexcept -> T requires ms::is_bool<T> {
				return false;
			}


			/* max integral */
			static consteval auto max(void) noexcept -> T requires (ms::is_integral<T>
																&& !ms::is_bool<T>) {

				// number of bits in type
				constexpr T bits = static_cast<T>((ms::bits<T>) - std::is_signed_v<T>);

				T count = static_cast<T>(0);

				// loop through bits
				for (T i = count; i < bits; ++i)
					// add shifted bit to max
					count |= static_cast<T>(static_cast<T>(1) << i);

				return count;
			}

			/* min integral */
			static consteval auto min(void) noexcept -> T requires (ms::is_integral<T>
																&& !ms::is_bool<T>) {
				T max = self::max();
				return ~max;
			}


			/* max floating point */
			static consteval auto max(void) noexcept -> T requires ms::is_floating_point<T> {

				if constexpr (sizeof(T) == 4U)
					// return IEEE 754 single precision max
					return 0x1.fffffep+127f;

				else if constexpr (sizeof(T) == 8U)
					// return IEEE 754 double precision max
					return 0x1.fffffffffffffp+1023;
			}

			/* min floating point */
			static consteval auto min(void) noexcept -> T requires ms::is_floating_point<T> {

				if constexpr (sizeof(T) == 4U)
					// return IEEE 754 single precision min
					return -0x1.fffffep+127f;

				else if constexpr (sizeof(T) == 8U)
					// return IEEE 754 double precision min
					return -0x1.fffffffffffffp+1023;
			}


			/* max number of digits */
			static consteval auto max_digits(void) -> unsigned {

				// max value of type
				T num = self::max();

				// number of digits
				unsigned digits = 0U;

				do { // increment digits
					++digits;
					// divide max by base
				} while (num /= 10U);

				return digits;
			}


	}; // class limits



	/* count leading zeros */
	template <typename T>
	constexpr auto countl_zero(const T num) noexcept -> ms::usz {

		static_assert(ms::is_integral<T>, "countl_zero only supports integral types");
		static_assert(sizeof(T) <= 8U, "countl_zero only supports integral types up to 64 bits");

		if (num == static_cast<T>(0))
			return ms::bits<T>;

		if !consteval {

			unsigned long index;

			if constexpr (sizeof(T) > 4U)
				static_cast<void>(::_BitScanReverse64(&index, static_cast<ms::u64>(num)));
			else
				static_cast<void>(::_BitScanReverse(&index, static_cast<unsigned long>(num)));

			return (ms::bits<T> - 1U) - index;
		}
		else {

			ms::usz count = 0;
			for (ms::usz i = ms::bits<T>; i > 0U; --i) {
				if (num & (T{1} << (i - 1U)))
					break;
				++count;
			}
			return count;
		}
	}


	/* count digits */
	template <ms::is_integral T>
	constexpr auto count_digits(const T value) noexcept -> ms::usz {


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
		const ms::usz bits_used = ms::bits<U> - ms::countl_zero(n);

		// approx log10
		const ms::usz log10 = (bits_used * 1233U) >> 12U;

		static constexpr ms::u64 powers_of_10[] {
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
