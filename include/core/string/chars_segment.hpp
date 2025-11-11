#pragma once

#include "core/string/char_traits.hpp"
#include "core/utility/limits.hpp"
#include "core/math/math.hpp"


#include "core/string/is_string.hpp"

#include <iostream>

// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	/* chars_segment
	 * This class is used to represent a segment of characters that can be converted
	 * to a character representation. It can handle single characters, strings, and
	 * other types that can be represented as characters.
	 */

	template <typename C, typename>
	class chars_segment;

	template <typename C, typename T>
	class chars_segment {
		static_assert(false, "Unsupported chars_segment specialization");
		noexcept_default_class(chars_segment);
	};


	// -- C H A R A C T E R  S P E C I A L I Z A T I O N ----------------------

	template <typename C>
	class chars_segment<C, C> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::chars_segment<C, C>;

			/* traits type */
			using traits = ms::char_traits<C>;


			// -- private members ---------------------------------------------

			/* character */
			traits::char_type _char;


		public:

			// -- public lifecycle --------------------------------------------

			/* value constructor */
			constexpr explicit chars_segment(const traits::char_type& ch) noexcept
			: _char{ch} {
			}

			constexpr chars_segment(void) noexcept = default;
			constexpr chars_segment(const self&) noexcept = default;
			constexpr ~chars_segment(void) noexcept = default;
			constexpr auto operator=(const self&) noexcept -> self& = default;
			constexpr auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* size */
			constexpr auto size(void) const noexcept -> traits::size_type {
				return 1U;
			}

			/* encode */
			constexpr auto encode(traits::pointer dst) const noexcept -> traits::size_type {
				*dst = _char;
				return 1U;
			}

			/* encode (range) */
			constexpr auto encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {
				if (begin == end)
					return 0U; // nothing to encode
				*begin = _char;
				return 1U; // one character encoded
			}

	};


	// -- P O I N T E R  S P E C I A L I Z A T I O N --------------------------

	template <typename C, ms::is_pointer_of<C> T>
	class chars_segment<C, T> final {

		private:
			using self = ms::chars_segment<C, T>;
			using traits = ms::char_traits<C>;

			traits::const_pointer _ptr;
			traits::size_type _size;


		public:

			constexpr explicit chars_segment(const T& ptr) noexcept
			: _ptr{ptr}, _size{traits::length(ptr)} {
			}

			constexpr auto size(void) const noexcept -> traits::size_type {
				return _size;
			}

			constexpr auto encode(traits::pointer dst) const noexcept -> traits::size_type {
				traits::copy(dst, _ptr, _size);
				return _size;
			}

			constexpr auto encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {
				return traits::copy_safe(begin, end, _ptr, _size);
			}

	};


	// -- A R R A Y  S P E C I A L I Z A T I O N ------------------------------

	template <typename C, ms::is_array_of<C> T>
	class chars_segment<C, T> final {

		private:
			using self = ms::chars_segment<C, T>;
			using traits = ms::char_traits<C>;

			constexpr static auto N = ms::extent<T>;

			traits::const_pointer _ptr;
			traits::size_type _size;


		public:

			constexpr explicit chars_segment(const T& arr) noexcept
			: _ptr{arr}, _size{arr[N - 1] == traits::null ? N - 1 : N} {
			}

			constexpr auto size(void) const noexcept -> traits::size_type {
				return _size;
			}

			constexpr auto encode(traits::pointer dst) const noexcept -> traits::size_type {
				traits::copy(dst, _ptr, _size);
				return _size;
			}

			constexpr auto encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {
				return traits::copy_safe(begin, end, _ptr, _size);
			}
	};




	// -- S T R I N G  S P E C I A L I Z A T I O N ----------------------------

	template <typename C, ms::is_string_like_of<C> T>
	class chars_segment<C, T> final {


		private:
			using self = ms::chars_segment<C, T>;
			using traits = ms::char_traits<C>;

			traits::const_pointer _data;
			traits::size_type _size;


		public:

			explicit chars_segment(const T& str) noexcept
			: _data{str.data()}, _size{str.size()} {
			}

			chars_segment(void) = delete;
			chars_segment(const self&) noexcept = default;

			auto operator=(const self&) noexcept -> self& = default;


			auto size(void) const noexcept -> traits::size_type {
				return _size;
			}
			auto encode(traits::pointer dst) const noexcept -> traits::size_type {
				traits::copy(dst, _data, _size);
				return _size;
			}
			auto encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {
				return traits::copy_safe(begin, end, _data, _size);
			}
	};



	// -- B O O L  S P E C I A L I Z A T I O N --------------------------------

	template <typename C>
	class chars_segment<C, bool> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::chars_segment<C, bool>;

			/* traits type */
			using traits = ms::char_traits<C>;


			// -- private members ---------------------------------------------

			/* index */
			unsigned _index;


			// -- private static members --------------------------------------

			/* true string */
			static constexpr traits::char_type _true[] {
				C{'t'}, C{'r'}, C{'u'}, C{'e'}
			};

			/* false string */
			static constexpr traits::char_type _false[] {
				C{'f'}, C{'a'}, C{'l'}, C{'s'}, C{'e'}
			};

			/* views */
			static constexpr ms::basic_string_view<C> _views[] {
				ms::basic_string_view<C>{self::_false, 5U},
				ms::basic_string_view<C>{self::_true,  4U}
			};


		public:

			constexpr explicit chars_segment(const bool value) noexcept
			: _index{!!value} {
			}

			constexpr chars_segment(const self&) noexcept = default;
			constexpr auto operator=(const self&) noexcept -> self& = default;

			constexpr auto size(void) const noexcept -> traits::size_type {
				return _views[_index].size();
			}

			constexpr auto encode(traits::pointer dst) const noexcept -> traits::size_type {
				const auto& view = _views[_index];
				traits::copy(dst, view.data(), view.size());
				return view.size();
			}

			constexpr auto encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {
				const auto& view = _views[_index];
				return traits::copy_safe(begin, end, view.data(), view.size());
			}
	};


	// -- N U L L P T R  S P E C I A L I Z A T I O N --------------------------

	template <typename C>
	class chars_segment<C, decltype(nullptr)> final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::chars_segment<C, decltype(nullptr)>;

			/* traits type */
			using traits = ms::char_traits<C>;


			// -- private constants -------------------------------------------

			/* null pointer string */
			static constexpr typename traits::char_type _str[] {
				C{'n'}, C{'u'}, C{'l'}, C{'l'}, C{'p'}, C{'t'}, C{'r'}
			};


		public:

			// -- public lifecycle --------------------------------------------

			constexpr chars_segment(void) noexcept = default;
			constexpr chars_segment(const decltype(nullptr)) noexcept {
			}
			constexpr chars_segment(const self&) noexcept = default;
			constexpr auto operator=(const self&) noexcept -> self& = default;

			// -- public methods ----------------------------------------------

			/* size */
			constexpr auto size(void) const noexcept -> traits::size_type {
				return 7U; // nullptr
			}

			/* encode */
			constexpr auto encode(traits::pointer dst) const noexcept -> traits::size_type {
				traits::copy(dst, _str, 7U);
				return 7U;
			}

			/* encode (range) */
			constexpr auto encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {
				return traits::copy_safe(begin, end, _str, 7U);
			}

	};


	// -- A R I T H M E T I C  S P E C I A L I Z A T I O N --------------------

	template <typename C, typename T> requires (ms::is_arithmetic<T>
											 && not ms::is_character<T>)
	class chars_segment<C, T> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::chars_segment<C, T>;

			/* traits type */
			using traits = ms::char_traits<C>;


			// -- private members ---------------------------------------------

			/* value */
			T _value;

			/* size */
			ms::usz _size;


			// -- private static members --------------------------------------

			/* precision */
			static constexpr ms::usz precision = 4U;



			template <typename U> requires (ms::is_integral<U> && std::is_unsigned_v<U>)
			static constexpr auto _count_digits(const U value) noexcept -> traits::size_type {
				return ms::count_digits(value);
			}

			template <typename S> requires (ms::is_integral<S> && std::is_signed_v<S>)
			static constexpr auto _count_digits(const S value) noexcept -> traits::size_type {

				const ms::usz digits = ms::count_digits(value);

				// msb index
				constexpr auto index = ms::bits<S> - 1U;

				// return required size
				return digits + ((static_cast<std::make_unsigned_t<S>>(value) >> index) & 1U);
			}

			template <typename F> requires (ms::is_floating_point<F>)
			static constexpr auto _count_digits(F value) noexcept -> traits::size_type {

				static_assert(ms::is_same<F, float>,
						"only supports float type for now");

				// aliasing rule is violated here
				ms::u32* ptr = reinterpret_cast<ms::u32*>(&value);

				// get bit sign
				const unsigned bit = (*ptr & 0x80000000U) >> 31U;

				// ensure positive value
				*ptr &= 0x7FFFFFFFU; // clear the sign bit

				const ms::u32 int_part   = static_cast<ms::u32>(value);
				const ms::usz int_digits = ms::count_digits(int_part);

				return int_digits + precision + 1U + bit;
			}



			template <typename U> requires (ms::is_integral<U> && std::is_unsigned_v<U>)
			static constexpr auto _from_unsigned(traits::pointer offset, U value) noexcept -> void {

				// LUT
				static constexpr char digits_table[200U] = {
					'0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
					'1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
					'2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
					'3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
					'4','0','4','1','4','2','4','3','4','4','4','5','4','6','4','7','4','8','4','9',
					'5','0','5','1','5','2','5','3','5','4','5','5','5','6','5','7','5','8','5','9',
					'6','0','6','1','6','2','6','3','6','4','6','5','6','6','6','7','6','8','6','9',
					'7','0','7','1','7','2','7','3','7','4','7','5','7','6','7','7','7','8','7','9',
					'8','0','8','1','8','2','8','3','8','4','8','5','8','6','8','7','8','8','8','9',
					'9','0','9','1','9','2','9','3','9','4','9','5','9','6','9','7','9','8','9','9'
				};

				// more efficient version
				while (value >= 100U) {
					const U i = (value % 100U) * 2U;
					*(--offset) = static_cast<C>(digits_table[i + 1U]);
					*(--offset) = static_cast<C>(digits_table[i /**/]);
					value /= 100U;
				}

				if (value >= 10U) {
					const U v = value * 2U;
					*(--offset) = static_cast<C>(digits_table[v + 1U]);
					*(--offset) = static_cast<C>(digits_table[v /**/]);
				}
				else
					*(--offset) = static_cast<C>(value + 0x30U);
			}


			constexpr auto _from_integral(traits::pointer dest) const noexcept -> void requires (ms::is_integral<T>) {

				const typename traits::pointer offset = dest + _size;

				if constexpr (std::is_signed_v<T>) {
					using U = std::make_unsigned_t<T>;

					if (_value < 0) {
						*dest = static_cast<C>('-');
						self::_from_unsigned<U>(offset, static_cast<U>(~_value) + 1U);
					}
					else {
						self::_from_unsigned<U>(offset, static_cast<U>(_value));
					}
				}
				else if constexpr (std::is_unsigned_v<T>) {
					self::_from_unsigned(offset, _value);
				}
			}




			template <typename F> requires (ms::is_floating_point<F>)
			constexpr auto _from_float(traits::pointer dest, F value) const noexcept -> void {

				constexpr ms::usz scale = ms::pow10(precision);

				// always add negative sign
				// will be erased if value is positive
				*dest = static_cast<C>('-');

				// ensure positive value
				value = ms::abs(value);

				using fl_type = double;

				ms::u32 int_part  = static_cast<ms::u32>(value);
				fl_type frac      = value - static_cast<fl_type>(int_part);
				ms::u32 frac_part = static_cast<ms::u32>((frac * scale) + 0.5f); // round to nearest integer


				if (frac_part >= scale) { // if scaled value is 1.0 or more
					frac_part -= scale; // adjust scaled value
					++int_part; // increment integer part
				}

				dest += _size; // move pointer to the end

				self::_from_unsigned<ms::u32>(dest, frac_part); // convert scaled value to characters
				dest -= (precision + 1U);
				*dest = static_cast<C>('.'); // add decimal point

				//dest[_digits - (precision + 1U)] = static_cast<C>('.'); // add decimal point
				self::_from_unsigned<ms::u32>(dest, int_part);//, size - (precision + 1U)); // convert integer part to characters
			}


		public:

			constexpr explicit chars_segment(const T& value) noexcept
			: _value{value}, _size{self::_count_digits(value)} {
			}

			constexpr auto size(void) const noexcept -> traits::size_type {
				return _size;
			}

			constexpr auto encode(traits::pointer dest) const noexcept -> traits::size_type {
				if constexpr (ms::is_integral<T>) {
					self::_from_integral(dest);
					return _size;
				}
				else if constexpr (ms::is_floating_point<T>) {
					self::_from_float(dest, _value);
					return _size;
				}
			}

			constexpr auto encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {
				return 0U; // not implemented for range encoding
				//const auto size = static_cast<traits::size_type>(end - begin);
				//_digits = (size < _digits) ? size : _digits; // handle negative value '-'
				//return self::encode(begin);
			}
	};


} // namespace ms


//namespace ms {
//
//
//
//	template <ms::is_integral U, U D> requires (std::is_unsigned_v<U>)
//	class magic_division final {
//
//		//private:
//		//	static constexpr unsigned shift = ms::bits<U> - 1U;
//		//	static constexpr auto multiplier = ((__uint128_t(1) << shift) + (D - 1U)) / D;
//		//
//		//public:
//		//
//		//	static constexpr auto divide(const U value) noexcept -> U {
//		//		return static_cast<U>((static_cast<__uint128_t>(value) * multiplier) >> shift);
//		//	}
//		//	static constexpr auto modulo(const U value) noexcept -> U {
//		//		return value - (divide(value) * D);
//		//	}
//		//
//		//	static constexpr auto divide_and_modulo(U& value) noexcept -> U {
//		//		const U q = divide(value);
//		//		const U r = value - (q * D);
//		//		value = q;
//		//		return r;
//		//	}
//
//	};
//
//
//
//
//
//
//
//	class u128 final {
//
//		private:
//
//			// -- private types -----------------------------------------------
//
//			/* self type */
//			using self = ms::u128;
//
//
//			// -- private members ---------------------------------------------
//
//			/* low part */
//			ms::uint_of<64> _low;
//
//			/* high part */
//			ms::uint_of<64> _high;
//
//
//		public:
//
//			auto operator*(const self& other) const noexcept -> self {
//				self result;
//				// use intrinsics for 128-bit multiplication
//				result._low = ::_umul128(_low, other._low, &result._high);
//				return result;
//			}
//
//
//	}; // class u128
//	static constexpr char digits_table[200U] = {
//		'0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
//		'1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
//		'2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
//		'3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
//		'4','0','4','1','4','2','4','3','4','4','4','5','4','6','4','7','4','8','4','9',
//		'5','0','5','1','5','2','5','3','5','4','5','5','5','6','5','7','5','8','5','9',
//		'6','0','6','1','6','2','6','3','6','4','6','5','6','6','6','7','6','8','6','9',
//		'7','0','7','1','7','2','7','3','7','4','7','5','7','6','7','7','7','8','7','9',
//		'8','0','8','1','8','2','8','3','8','4','8','5','8','6','8','7','8','8','8','9',
//		'9','0','9','1','9','2','9','3','9','4','9','5','9','6','9','7','9','8','9','9'
//	};
//
//	ms::u32 value = 12345678;
//	const auto size = ms::count_digits(value);
//	std::cout << "Start value: " << value << " (" << size << " digits)\r\n";
//	char* buffer = (char*)::operator new(size + 1U); // +1 for null terminator
//	buffer[size] = '\0'; // null terminate the string
//
//	char* ptr = buffer + size;
//
//	// Division par 100 via multiplicateur magique (approximation exacte pour 32 bits)
//	while (value >= 100) {
//		std::cout << ">= 100: ";
//
//
//		constexpr uint64_t mul_inv_100 = 0x51EB851F; // ≈ 2^37 / 100
//		uint32_t q = (uint64_t(value) * mul_inv_100) >> 37;
//		uint32_t r = value - q * 100;
//		std::cout << q << " - " << r << "\r\n";
//		value = q;
//		r *= 2U; // r * 2U for first digit and second digit
//		*(--ptr) = digits_table[r + 1U]; // r + 1U for second digit
//		*(--ptr) = digits_table[r /**/]; // r for first digit
//	}
//	if (value >= 10) {
//		std::cout << ">= 10: ";
//		const uint32_t v = value * 2U; // multiply by 2U for first digit and second digit
//		*(--ptr) = digits_table[v + 1U]; // v + 1U for second digit
//		*(--ptr) = digits_table[v /**/]; // v for first digit
//		std::cout << v/2 << "\r\n";
//	}
//	else {
//		std::cout << "< 10: ";
//		*(--ptr) = static_cast<char>(value + 0x30U); // convert to character
//		std::cout << value << "\r\n";
//	}
//
//	std::cout << "Encoded value: " << ptr << "\r\n";
//
//	::operator delete(buffer); // free the buffer
//
//}
