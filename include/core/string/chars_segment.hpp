#ifndef core_string_char_segment_hpp
#define core_string_char_segment_hpp

#include "core/utility/limits.hpp"
#include "core/math/math.hpp"

#include "core/string/string_view.hpp"
#include "core/string/is_string.hpp"
#include "core/memory/memcpy.hpp"



// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* chars_segment
	 * This class is used to represent a segment of characters that can be converted
	 * to a character representation. It can handle single characters, strings, and
	 * other types that can be represented as characters.
	 */

	template <typename>
	class chars_segment;

	template <typename>
	class chars_segment {
		static_assert(false, "Unsupported chars_segment specialization");
		noexcept_default_class(chars_segment);
	};


	// -- C H A R A C T E R  S P E C I A L I Z A T I O N ----------------------

	template <>
	class chars_segment<char> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::chars_segment<char>;


			// -- private members ---------------------------------------------

			/* character */
			char _ch;


		public:

			// -- public lifecycle --------------------------------------------

			/* value constructor */
			constexpr explicit chars_segment(const char ch) noexcept
			: _ch{ch} {
			}

			constexpr chars_segment(void) noexcept = default;
			constexpr chars_segment(const self&) noexcept = default;
			constexpr ~chars_segment(void) noexcept = default;
			constexpr auto operator=(const self&) noexcept -> self& = default;
			constexpr auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* size */
			constexpr auto size(void) const noexcept -> mx::usz {
				return 1U;
			}

			/* encode */
			constexpr auto encode(char* dst) const noexcept -> mx::usz {
				*dst = _ch;
				return 1U;
			}

			/* encode (range) */
			constexpr auto encode(char* begin, const char* end) const noexcept -> mx::usz {
				if (begin == end)
					return 0U; // nothing to encode
				*begin = _ch;
				return 1U; // one character encoded
			}

	};


	// -- P O I N T E R  S P E C I A L I Z A T I O N --------------------------

	template <mx::is_pointer_of<char> T>
	class chars_segment<T> final {

		private:
			using self = mx::chars_segment<T>;

			const char* _ptr;
			mx::usz _size;


		public:

			constexpr explicit chars_segment(const T& ptr) noexcept
			: _ptr{ptr}, _size{mx::strlen(ptr)} {
			}

			constexpr auto size(void) const noexcept -> mx::usz {
				return _size;
			}

			constexpr auto encode(char* dst) const noexcept -> mx::usz {
				mx::memcpy(dst, _ptr, _size);
				return _size;
			}

			constexpr auto encode(char* begin, char* end) const noexcept -> mx::usz {
				return mx::memcpy(begin, end, _ptr, _size);
			}

	};


	// -- A R R A Y  S P E C I A L I Z A T I O N ------------------------------

	template <mx::is_array_of<char> T>
	class chars_segment<T> final {

		private:
			using self = mx::chars_segment<T>;

			constexpr static auto N = mx::extent<T>;

			const char* _ptr;
			mx::usz _size;


		public:

			constexpr explicit chars_segment(const T& arr) noexcept
			: _ptr{arr}, _size{arr[N - 1U] == '\0' ? N - 1U : N} {
			}

			constexpr auto size(void) const noexcept -> mx::usz {
				return _size;
			}

			constexpr auto encode(char* dst) const noexcept -> mx::usz {
				mx::memcpy(dst, _ptr, _size);
				return _size;
			}

			constexpr auto encode(char* begin, char* end) const noexcept -> mx::usz {
				return mx::memcpy(begin, end, _ptr, _size);
			}
	};




	// -- S T R I N G  S P E C I A L I Z A T I O N ----------------------------

	template <mx::is_string_class T>
	class chars_segment<T> final {


		private:
			using self = mx::chars_segment<T>;

			const char* _data;
			mx::usz _size;


		public:

			explicit chars_segment(const T& str) noexcept
			: _data{str.data()}, _size{str.size()} {
			}

			chars_segment(void) = delete;
			chars_segment(const self&) noexcept = default;

			auto operator=(const self&) noexcept -> self& = default;


			auto size(void) const noexcept -> mx::usz {
				return _size;
			}
			auto encode(char* dst) const noexcept -> mx::usz {
				mx::memcpy(dst, _data, _size);
				return _size;
			}
			auto encode(char* begin, char* end) const noexcept -> mx::usz {
				return mx::memcpy(begin, end, _data, _size);
			}
	};



	// -- B O O L  S P E C I A L I Z A T I O N --------------------------------

	template <>
	class chars_segment<bool> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::chars_segment<bool>;


			// -- private members ---------------------------------------------

			/* index */
			unsigned _index;


			// -- private static members --------------------------------------

			/* true string */
			static constexpr char _true[] {
				't', 'r', 'u', 'e'
			};

			/* false string */
			static constexpr char _false[] {
				'f', 'a', 'l', 's', 'e'
			};

			/* views */
			static constexpr mx::string_view _views[] {
				mx::string_view{self::_false, 5U},
				mx::string_view{self::_true,  4U}
			};


		public:

			constexpr explicit chars_segment(const bool value) noexcept
			: _index{!!value} {
			}

			constexpr chars_segment(const self&) noexcept = default;
			constexpr auto operator=(const self&) noexcept -> self& = default;

			constexpr auto size(void) const noexcept -> mx::usz {
				return _views[_index].size();
			}

			constexpr auto encode(char* dst) const noexcept -> mx::usz {
				const auto& view = _views[_index];
				mx::memcpy(dst, view.data(), view.size());
				return view.size();
			}

			constexpr auto encode(char* begin, char* end) const noexcept -> mx::usz {
				const auto& view = _views[_index];
				return mx::memcpy(begin, end, view.data(), view.size());
			}
	};


	// -- N U L L P T R  S P E C I A L I Z A T I O N --------------------------

	template <>
	class chars_segment<decltype(nullptr)> final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::chars_segment<decltype(nullptr)>;


			// -- private constants -------------------------------------------

			/* null pointer string */
			static constexpr char _str[] {
				'n', 'u', 'l', 'l', 'p', 't', 'r'
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
			constexpr auto size(void) const noexcept -> mx::usz {
				return 7U; // nullptr
			}

			/* encode */
			constexpr auto encode(char* dst) const noexcept -> mx::usz {
				mx::memcpy(dst, _str, 7U);
				return 7U;
			}

			/* encode (range) */
			constexpr auto encode(char* begin, char* end) const noexcept -> mx::usz {
				return mx::memcpy(begin, end, _str, 7U);
			}

	};


	// -- A R I T H M E T I C  S P E C I A L I Z A T I O N --------------------

	template <typename T> requires (mx::is_arithmetic<T>
							 && not mx::is_character<T>)
	class chars_segment<T> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::chars_segment<T>;


			// -- private members ---------------------------------------------

			/* value */
			T _value;

			/* size */
			mx::usz _size;


			// -- private static members --------------------------------------

			/* precision */
			static constexpr mx::usz precision = 4U;



			template <typename U> requires (mx::is_integral<U> && std::is_unsigned_v<U>)
			static constexpr auto _count_digits(const U value) noexcept -> mx::usz {
				return mx::count_digits(value);
			}

			template <typename S> requires (mx::is_integral<S> && std::is_signed_v<S>)
			static constexpr auto _count_digits(const S value) noexcept -> mx::usz {

				const mx::usz digits = mx::count_digits(value);

				// msb index
				constexpr auto index = mx::bits<S> - 1U;

				// return required size
				return digits + ((static_cast<std::make_unsigned_t<S>>(value) >> index) & 1U);
			}

			template <typename F> requires (mx::is_floating_point<F>)
			static constexpr auto _count_digits(F value) noexcept -> mx::usz {

				static_assert(mx::is_same<F, float>,
						"only supports float type for now");

				// aliasing rule is violated here
				mx::u32* ptr = reinterpret_cast<mx::u32*>(&value);

				// get bit sign
				const unsigned bit = (*ptr & 0x80000000U) >> 31U;

				// ensure positive value
				*ptr &= 0x7FFFFFFFU; // clear the sign bit

				const mx::u32 int_part   = static_cast<mx::u32>(value);
				const mx::usz int_digits = mx::count_digits(int_part);

				return int_digits + precision + 1U + bit;
			}



			template <typename U> requires (mx::is_integral<U> && std::is_unsigned_v<U>)
			static constexpr auto _from_unsigned(char* offset, U value) noexcept -> void {

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
					*(--offset) = digits_table[i + 1U];
					*(--offset) = digits_table[i     ];
					value /= 100U;
				}

				if (value >= 10U) {
					const U v = value * 2U;
					*(--offset) = digits_table[v + 1U];
					*(--offset) = digits_table[v     ];
				}
				else
					*(--offset) = value + 0x30U;
			}


			constexpr auto _from_integral(char* dest) const noexcept -> void requires (mx::is_integral<T>) {

				char* offset = dest + _size;

				if constexpr (std::is_signed_v<T>) {
					using U = std::make_unsigned_t<T>;

					if (_value < 0) {
						*dest = '-';
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




			template <typename F> requires (mx::is_floating_point<F>)
			constexpr auto _from_float(char* dest, F value) const noexcept -> void {

				constexpr mx::usz scale = mx::pow10(precision);

				// always add negative sign
				// will be erased if value is positive
				*dest = '-';

				// ensure positive value
				value = mx::abs(value);

				using fl_type = double;

				mx::u32 int_part  = static_cast<mx::u32>(value);
				fl_type frac      = value - static_cast<fl_type>(int_part);
				mx::u32 frac_part = static_cast<mx::u32>((frac * scale) + 0.5f); // round to nearest integer


				if (frac_part >= scale) { // if scaled value is 1.0 or more
					frac_part -= scale; // adjust scaled value
					++int_part; // increment integer part
				}

				dest += _size; // move pointer to the end

				self::_from_unsigned<mx::u32>(dest, frac_part); // convert scaled value to characters
				dest -= (precision + 1U);
				*dest = '.'; // add decimal point

				//dest[_digits - (precision + 1U)] = static_cast<C>('.'); // add decimal point
				self::_from_unsigned<mx::u32>(dest, int_part);//, size - (precision + 1U)); // convert integer part to characters
			}


		public:

			constexpr explicit chars_segment(const T& value) noexcept
			: _value{value}, _size{self::_count_digits(value)} {
			}

			constexpr auto size(void) const noexcept -> mx::usz {
				return _size;
			}

			constexpr auto encode(char* dest) const noexcept -> mx::usz {
				if constexpr (mx::is_integral<T>) {
					self::_from_integral(dest);
					return _size;
				}
				else if constexpr (mx::is_floating_point<T>) {
					self::_from_float(dest, _value);
					return _size;
				}
			}

			constexpr auto encode(char* begin, char* end) const noexcept -> mx::usz {
				return 0U; // not implemented for range encoding
				//const auto size = static_cast<traits::size_type>(end - begin);
				//_digits = (size < _digits) ? size : _digits; // handle negative value '-'
				//return self::encode(begin);
			}
	};


} // namespace mx

#endif // core_string_char_segment_hpp
