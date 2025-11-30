#pragma once

#include "core/memory/memcpy.hpp"
#include "core/memory/memmove.hpp"
#include "core/memory/malloc.hpp"


//#include "core/string/inplace_string.hpp"


// fix limit and overflow in extend and try extend methods

#include "core/type_traits/traits.hpp"
#include "core/type_traits/index_sequence.hpp"
#include "core/utility/exchange.hpp"

#include "core/string/char_traits.hpp"
#include "core/string/chars_encoder.hpp"

#include <iostream>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- B A S I C  S T R I N G ----------------------------------------------

	template <typename C>
	class basic_string final {


		// -- assertions ------------------------------------------------------

		/* requires char type */
		static_assert(ms::is_character<C>,
					  "basic_string requires a character type (char, wchar_t, char8_t, char16_t, char32_t)");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self            = ms::basic_string<C>;

			/* traits type */
			using traits          = ms::char_traits<C>;


		public:

			// -- public types ------------------------------------------------

			/* char type */
			using char_type       = traits::char_type;

			/* reference type */
			using reference       = traits::reference;

			/* const reference type */
			using const_reference = traits::const_reference;

			/* pointer type */
			using pointer         = traits::pointer;

			/* const pointer type */
			using const_pointer   = traits::const_pointer;

			/* size type */
			using size_type       = traits::size_type;


		private:

			// -- private members ---------------------------------------------

			/* capacity */
			size_type _capacity;

			/* data */
			pointer _data;

			/* size */
			size_type _size;



			// -- private lifecycle -------------------------------------------

			/* encoder constructor */
			template <typename... Tp>
			explicit basic_string(const ms::chars_encoder<C, Tp...>& encoder)
			: _capacity{encoder.size()},
			  _data{self::_allocate(_capacity)},
			  _size{encoder.encode(_data)} {

				// null terminate
				self::_null_terminate();
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			basic_string(void) noexcept
			: _capacity{0U}, _data{self::_empty()}, _size{0U} {
			}

			template <typename... Tp>
			static constexpr bool any_encoder =
				(ms::is_chars_encoder<Tp> || ...);

			template <typename... Tp>
			static constexpr bool is_single_self =
				(sizeof...(Tp) == 1U) && (mx::is_same<self, ms::remove_cvr<Tp>> && ...);


			/* variadic constructor */
			template <typename... Tp> requires (not any_encoder<Tp...>)
											&& (not is_single_self<Tp...>)
											&& (sizeof...(Tp) > 0U)
			explicit basic_string(Tp&&... args)
			: basic_string{ms::chars_encoder<C, Tp...>{ms::forward<Tp>(args)...}} {
			}

			/* copy constructor */
			basic_string(const self& other)
			: _capacity{other._size}, _data{self::_empty()}, _size{_capacity} {

				// check size
				if (_size == 0U)
					return;

				// allocate memory
				_data = self::_allocate(other._size);

				// copy data
				ms::memcpy(_data, other._data, _size);

				// null terminate
				self::_null_terminate();
			}

			/* move constructor */
			basic_string(self&& other) noexcept
			: _capacity{ms::exchange(other._capacity, 0U)},
				  _data{ms::exchange(other._data, self::_empty())},
				  _size{ms::exchange(other._size, 0U)} {
				// other is now empty
			}

			/* destructor */
			~basic_string(void) noexcept {
				self::_deallocate();
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self& other) -> self& {

				if (this == &other)
					return *this;

				if (_capacity < other._size) {

					// allocate new memory
					pointer ptr = self::_allocate(other._size);

					// deallocate memory
					self::_deallocate();

					// copy members
					_data     = ptr;
					_capacity = other._size;
				}

				// copy data
				ms::memcpy(_data, other._data, other._size);
				_size = other._size;

				// null terminate
				self::_null_terminate();

				// done
				return *this;
			}

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				if (this == &other)
					return *this;

				// deallocate memory
				self::_deallocate();

				// move members
				_capacity = ms::exchange(other._capacity, 0U);
				_data     = ms::exchange(other._data, self::_empty());
				_size     = ms::exchange(other._size, 0U);

				return *this;
			}


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> pointer {
				return _data;
			}

			/* const data */
			auto data(void) const noexcept -> const_pointer {
				return _data;
			}

			/* cstr */
			auto cstr(void) noexcept -> pointer {
				return _data;
			}

			/* const cstr */
			auto cstr(void) const noexcept -> const_pointer {
				return _data;
			}

			/* size */
			auto size(void) const noexcept -> size_type {
				return _size;
			}

			/* capacity */
			auto capacity(void) const noexcept -> size_type {
				return _capacity;
			}

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}

			/* front */
			auto front(void) noexcept -> reference {
				return *_data;
			}

			/* front */
			auto front(void) const noexcept -> char_type {
				return *_data;
			}

			/* back */
			auto back(void) noexcept -> reference {
				return _data[_size - 1U];
			}

			/* back */
			auto back(void) const noexcept -> char_type {
				return _data[_size - 1U];
			}


			// -- public subscript operators ----------------------------------

			/* operator [] */
			auto operator[](const size_type pos) noexcept -> reference {
				return _data[pos];
			}

			/* operator [] */
			auto operator[](const size_type pos) const noexcept -> char_type {
				return _data[pos];
			}


			// -- public modifiers --------------------------------------------

			/* reserve */
			auto reserve(const size_type cap) -> void {
				// reallocate memory
				if (cap > _capacity)
					self::_reserve(cap);

				// does i need to null terminate?????
			}

			/* resize */
			auto resize(const size_type size) -> void {

				if (size > _capacity)
					self::_reserve(size);

				_size = size;

				// null terminate
				self::_null_terminate();
			}

			/* clear */
			auto clear(void) noexcept -> void {
				_size = 0U;
				self::_null_terminate();
			}

			/* push back */
			auto push_back(const char_type ch) -> void {

				// check capacity
				if (_size == _capacity)
					self::_reserve(self::_extend());

				// copy construct
				*(_data + _size) = ch;
				++_size;

				// null terminate
				self::_null_terminate();
			}

			/* pop back */
			auto pop_back(const size_type count = 1U) noexcept -> void {

				// reduce size
				_size = (_size < count) ? 0U : _size - count;

				// null terminate (maybe segfault)
				self::_null_terminate();
			}


		private:

			template <typename... Tp>
			auto _append_impl(const ms::chars_encoder<C, Tp...>& encoder) -> void {

				const size_type required = encoder.size();

				// check capacity
				if (required > self::_available_capacity()) {
					// reserve capacity
					self::_reserve(
						self::_try_extend(_size + required)
					);
				}

				// append each argument
				_size += encoder.encode(_data + _size);
			}

			template <typename... Tp>
			auto _assign_impl(const ms::chars_encoder<C, Tp...>& encoder) -> void {

				const size_type required = encoder.size();

				// check capacity
				if (required > self::_available_capacity()) {
					// reserve capacity
					self::_reserve(
						self::_try_extend(required)
					);
				}

				// append each argument
				_size = encoder.encode(_data);
			}


		public:

			/* append */
			template <typename... Ts>
			auto append(Ts&&... args) -> void {

				// check if any arguments
				if constexpr (sizeof...(args) > 0U) {
					self::_append_impl(
						ms::chars_encoder<C, Ts...>{ms::forward<Ts>(args)...}
					);

					// null terminate
					self::_null_terminate();
				}
			}

			/* assign */
			template <typename... Ts>
			auto assign(Ts&&... args) -> void {

				// clear current string
				_size = 0U;

				// check if any arguments
				if constexpr (sizeof...(args) > 0U)
					self::_append_impl(
						ms::chars_encoder<C, Ts...>{ms::forward<Ts>(args)...}
					);

				// null terminate
				self::_null_terminate();
			}

			/* erase */
			auto erase(const size_type pos) noexcept -> void {

				// check position
				if (pos >= _size)
					return;

				// shift data
				ms::memmove(_data + pos, _data + pos + 1U, _size - pos);

				// update size
				--_size;

				// null terminate
				self::_null_terminate();
			}



			// -- public comparison operators ---------------------------------

			/* equal */
			auto operator==(const self& other) const noexcept -> bool {
				return self::_compare(other) == 0;
			}

			/* not equal */
			auto operator!=(const self& other) const noexcept -> bool {
				return self::_compare(other) != 0;
			}

			/* less than */
			auto operator <(const self& other) const noexcept -> bool {
				return self::_compare(other) < 0;
			}

			/* greater than */
			auto operator >(const self& other) const noexcept -> bool {
				return self::_compare(other) > 0;
			}

			/* less than or equal to */
			auto operator<=(const self& other) const noexcept -> bool {
				return self::_compare(other) <= 0;
			}

			/* greater than or equal to */
			auto operator>=(const self& other) const noexcept -> bool {
				return self::_compare(other) >= 0;
			}


		private:

			// -- private static methods --------------------------------------

			/* allocate */
			static auto _allocate(const size_type size) -> pointer {
				return ms::allocate<char_type>(size + 1U);
			}


			// -- private methods ---------------------------------------------

			/* empty data pointer */
			auto _empty(void) noexcept -> pointer {
				static char_type _empty[1U] = { traits::null };
				return reinterpret_cast<pointer>(&_empty);
				//return reinterpret_cast<pointer>(&_capacity); // very bad idea (destructor will crash when moving)
			}

			/* reallocate */
			auto _reallocate(const size_type size) -> pointer {
				return ms::reallocate<char_type>(_data, size + 1U);
			}

			/* null terminate */
			auto _null_terminate(void) noexcept -> void {
				_data[_size] = traits::null;
			}

			/* available capacity */
			auto _available_capacity(void) const noexcept -> size_type {
				return _capacity - _size;
			}

			/* extend */
			auto _extend(void) const noexcept -> size_type {
				// check overflow
				return _capacity ? _capacity << 1U : 1U;
			}

			/* try extend */
			auto _try_extend(const size_type cap) const noexcept -> size_type {
				// check overflow
				const auto ncp = _capacity << 1U;
				return ncp < cap ? cap : ncp;
			}

			/* deallocate */
			auto _deallocate(void) noexcept -> void {
				if (_data == self::_empty())
					return;
				ms::deallocate(_data);
			}

			/* reserve */
			auto _reserve(const size_type& cap) -> void {

				_data = (_data != self::_empty())
					// reallocate new memory
					? self::_reallocate(cap)
					// allocate new memory
					: self::_allocate(cap);

				// update capacity
				_capacity = cap;
			}


			// -- private comparison methods ----------------------------------

			/* compare */
			auto _compare(const self& other) const noexcept -> int {

				// compare size
				if (_size != other._size)
					return _size < other._size ? -1 : 1;

				// compare data
				for (const_pointer s1 = _data, s2 = other._data;
						*s1 != traits::null; ++s1, ++s2) {

					// check inequality
					if (*s1 != *s2)
						return *s1 < *s2 ? -1 : 1;
				}

				// return equal
				return 0;
			}

	}; // class basic_string


			// C I R C U L A R  D E P E N D EN C Y
			/* append multi (inplace string) */
			//template <size_type N>
			//auto _append_multi(size_type& offset, const ms::inplace_wstring<N>& str) noexcept -> size_type {
			//	return self::_append_multi(offset, str.data(), str.size());
			//}


	// -- aliases -------------------------------------------------------------

	/* string */
	using string         = ms::basic_string<char>;

	/* wide string */
	using wstring        = ms::basic_string<wchar_t>;

	/* u8 string */
	using u8string       = ms::basic_string<char8_t>;

	/* u16 string */
	using u16string      = ms::basic_string<char16_t>;

	/* u32 string */
	using u32string      = ms::basic_string<char32_t>;




	/* cstr */
	using cstr = ms::string;

	/* widestr */
	using wstr = ms::wstring;

	/* str8 */
	using str8 = ms::u8string;

	/* str16 */
	using str16 = ms::u16string;

	/* str */
	using str32 = ms::u32string;




} // namespace ms





			/* insert multi */
			/*
			template <typename... ___params>
			auto insert(const size_type ___pos, const ___params&... ___args) -> self& {

				// check if any arguments
				if constexpr (sizeof...(___args) == 0U)
					return *this;

				// check position
				if (___pos > _size)
					return *this;

				// compute required size
				const auto ___req = (self::_size_multi(___args) + ...);

				// reserve capacity
				if (___req > self::_available_capacity())
					self::_reserve(self::_try_extend(_size + ___req));

				// shift data
				msh::memmove(_data + ___pos + ___req,
							 _data + ___pos, _size - ___pos);

				// offset
				size_type ___of = ___pos;

				// insert and update size
				_size += (self::_append_multi(___of, ___args) + ...);

				// null terminate
				_null_terminate();

				return *this;
			}
			*/

			/* append multi (integer) */
			/*
			template <typename ___type> requires (msh::is_integral<___type>
											   && not msh::is_char<___type>
											   && msh::is_unsigned<___type>)
			auto _append_multi(size_type& ___of, const ___type& ___val) -> size_type {

				// copy integer
				___type ___num = ___val;

				// get offset position
				const size_type ___st = ___of;

				// loop over all digits
				do {
					_data[___of] = static_cast<value_type>((___num % 10U) + '0');
					___num /= 10U;
					++___of;
				} while (___num != 0U);


				// reverse all integral digits
				for (size_type start = ___st,
								 end = ___of - 1U; start < end; ++start, --end) {

					// swap characters
					const char tmp = _data[start];
					_data[start]   = _data[end];
					_data[end]     = tmp;
				}

				// return number of characters
				return ___of - ___st;
			}
			*/
			/* size multi (integer) */
			/*
			template <typename ___type> requires (msh::is_integral<___type>
											&& not msh::is_char<___type>)
			static auto _size_multi(const ___type&) noexcept -> size_type {

				// return max number of digits
				return msh::limits<___type>::digits();
			}
			*/

			/* view constructor */
			//explicit string(const string_view& view)
			//: _data{_empty}, _size{view.size()}, _capacity{_size} {
			//
			//	// check view size
			//	if (_size == 0U)
			//		return;
			//
			//	// allocate memory
			//	_data = _allocate(_size);
			//
			//	// copy data
			//	msh::memcpy(_data, view.data(), _size);
			//
			//	// null terminate
			//	_null_terminate();
			//}

















	//
	//template <typename T>
	//class integral_traits final {
	//
	//	// -- assertions ------------------------------------------------------
	//
	//	/* requires integral type */
	//	static_assert(ms::is_integral<T>, "integral_traits requires an integral type");
	//
	//
	//	public:
	//
	//		static constexpr auto chars_length(const T value) noexcept -> ms::usz {
	//			const ms::usz digits = ms::count_digits(value);
	//			if constexpr (std::is_unsigned_v<T>) {
	//				return digits;
	//			}
	//			else if constexpr (std::is_signed_v<T>) {
	//				// get bit sign
	//				using U = std::make_unsigned_t<T>;
	//				//U bit = 
	//				// implement it
	//
	//
	//				return value < 0 ? digits + 1U : digits;
	//			}
	//		}
	//
	//}; // class integral_traits
	//
	//
	//template <typename T>
	//class floating_traits final {
	//
	//	// -- assertions ------------------------------------------------------
	//
	//	/* requires floating point type */
	//	static_assert(ms::is_floating_point<T>, "floating_traits requires a floating point type");
	//
	//
	//	public:
	//
	//		static constexpr ms::usz precision = 4U;
	//
	//		static constexpr auto chars_length(T value) noexcept -> ms::usz {
	//
	//
	//			static_assert(mx::is_same<T, float>,
	//						   "floating_traits only supports float type for now");
	//
	//
	//			if constexpr (std::is_same_v<T, float>) {
	//
	//				// aliasing rule is violated here
	//				ms::u32* ptr = reinterpret_cast<ms::u32*>(&value);
	//
	//				// get bit sign
	//				const unsigned bit = (*ptr & 0x80000000U) >> 31U;
	//
	//				// ensure positive value
	//				*ptr &= 0x7FFFFFFFU; // clear the sign bit
	//
	//				const ms::u32 int_part   = static_cast<ms::u32>(value);
	//				const ms::usz int_digits = ms::count_digits(int_part);
	//
	//				return int_digits + precision + 1U + bit;
	//			}
	//
	//		}
	//
	//}; // class floating_traits
	//
	//
	//template <typename C>
	//class append_traits final {
	//
	//
	//	private:
	//
	//		// LUT
	//		static constexpr char digits[200U] = {
	//			'0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
	//			'1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
	//			'2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
	//			'3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
	//			'4','0','4','1','4','2','4','3','4','4','4','5','4','6','4','7','4','8','4','9',
	//			'5','0','5','1','5','2','5','3','5','4','5','5','5','6','5','7','5','8','5','9',
	//			'6','0','6','1','6','2','6','3','6','4','6','5','6','6','6','7','6','8','6','9',
	//			'7','0','7','1','7','2','7','3','7','4','7','5','7','6','7','7','7','8','7','9',
	//			'8','0','8','1','8','2','8','3','8','4','8','5','8','6','8','7','8','8','8','9',
	//			'9','0','9','1','9','2','9','3','9','4','9','5','9','6','9','7','9','8','9','9'
	//		};
	//
	//
	//	public:
	//
	//		template <typename T> requires (ms::is_integral<T>)
	//		static constexpr auto to_chars(C* dest, T value, const ms::usz size) noexcept -> void {
	//
	//			if constexpr (std::is_unsigned_v<T>) {
	//				ms::usz o = size;
	//
	//				// more efficient version
	//				while (value >= 100U) {
	//					const T i = (value % 100U) * 2U;
	//					dest[--o] = static_cast<C>(digits[i + 1U]);
	//					dest[--o] = static_cast<C>(digits[i /**/]);
	//					value /= 100U;
	//				}
	//
	//				if (value >= 10U) {
	//					const T v = value * 2U;
	//					dest[--o] = static_cast<C>(digits[v + 1U]);
	//					dest[--o] = static_cast<C>(digits[v /**/]);
	//				}
	//				else
	//					dest[--o] = static_cast<C>(value + 0x30U);
	//
	//			}
	//			else if constexpr (std::is_signed_v<T>) {
	//
	//				// unsigned type
	//				using U = std::make_unsigned_t<T>;
	//
	//				if (value < 0) {
	//					*dest = static_cast<C>('-');
	//					to_chars<U>(dest, static_cast<U>(~value) + 1U, size);
	//				}
	//				else {
	//					to_chars<U>(dest, static_cast<U>(value), size);
	//				}
	//			}
	//		}
	//
	//		static constexpr auto pow10(const ms::usz exp) noexcept -> ms::usz {
	//			ms::usz result = 1U;
	//			for (ms::usz i = 0U; i < exp; ++i)
	//				result *= 10U;
	//			return result;
	//		}
	//
	//		template <typename T> requires (ms::is_floating_point<T>)
	//		static constexpr auto to_chars(C* dest, T value, const ms::usz size) noexcept -> void {
	//
	//			constexpr ms::usz precision = ms::floating_traits<T>::precision; // number of decimal places
	//			constexpr ms::usz scale = pow10(precision);
	//
	//			// always add negative sign
	//			// will be erased if value is positive
	//			*dest = static_cast<C>('-');
	//
	//			// ensure positive value
	//			value = ms::abs(value);
	//
	//			using fl_type = double;
	//
	//			ms::u32 int_part  = static_cast<ms::u32>(value);
	//			fl_type frac      = value - static_cast<fl_type>(int_part);
	//			ms::u32 frac_part = static_cast<ms::u32>((frac * scale) + 0.5f); // round to nearest integer
	//
	//
	//			if (frac_part >= scale) { // if scaled value is 1.0 or more
	//				frac_part -= scale; // adjust scaled value
	//				++int_part; // increment integer part
	//			}
	//
	//			to_chars<ms::u32>(dest, frac_part, size); // convert scaled value to characters
	//			dest[size - (precision + 1U)] = static_cast<C>('.'); // add decimal point
	//			to_chars<ms::u32>(dest, int_part, size - (precision + 1U)); // convert integer part to characters
	//			return;
	//
	//
	//			// ---------------------------------
	//			//// always add negative sign
	//			//// will be erased if value is positive
	//			//*dest = static_cast<C>('-');
	//			//
	//			//// ensure positive value
	//			//value = ms::abs(value);
	//			//
	//			//
	//			//ms::u32 int_part = static_cast<ms::u32>(value);
	//			//float  frac_part = value - static_cast<float>(int_part);
	//			//
	//			//
	//			//constexpr ms::usz scale = pow10(4U); // scale to 4 decimal places
	//			//float scaled = frac_part * scale;
	//			//
	//			//
	//			//
	//			//// fill the decimal part
	//			//constexpr ms::usz precision = 4U; // number of decimal places
	//			//ms::usz o = size - precision; // position for the decimal point
	//			//
	//			//for (ms::usz i = o; i < size; ++i) {
	//			//	frac_part *= 10.0f;
	//			//	ms::u32 digit = static_cast<ms::u32>(frac_part);
	//			//	dest[i] = static_cast<C>(digit + '0');
	//			//	frac_part -= static_cast<float>(digit);
	//			//}
	//			//
	//			//// add the decimal point
	//			//dest[--o] = static_cast<C>('.');
	//			//
	//			//to_chars<ms::u32>(dest, int_part, o); // convert integer part to characters
	//		}
	//};
	//
	//
			//// -- private constants -------------------------------------------
			//
			///* is pointer */
			////template <typename T>
			////static constexpr bool is_pointer =
			////	ms::is_pointer<clean<T>> &&
			////	mx::is_same<ms::remove_cvp<clean<T>>, C>;
			//
			///* is array */
			////template <typename T>
			////static constexpr bool is_array =
			////	ms::is_array<clean<T>> &&
			////	mx::is_same<ms::remove_cve<clean<T>>, C>;
			//
			///* is string */
			//template <typename T>
			//static constexpr bool is_string =
			//	mx::is_same<clean<T>, ms::basic_string<C>>;
			//
			///* is string view */
			//template <typename T>
			//static constexpr bool is_view =
			//	mx::is_same<clean<T>, ms::basic_string_view<C>>;
			//
			///* is char type */
			//template <typename T>
			//static constexpr bool is_char_type =
			//	mx::is_same<clean<T>, C>;
			//
			///* is integral */
			//template <typename T>
			//static constexpr bool is_integral =
			//	ms::is_integral<clean<T>> &&
			//	not ms::is_character<clean<T>>;
			//
			///* is floating point */
			//template <typename T>
			//static constexpr bool is_floating_point =
			//	ms::is_floating_point<clean<T>>;
			//
			///* is appendable */
			//template <typename T>
			//static constexpr bool is_appendable =
			//	   is_pointer  <T>
			//	|| is_array    <T>
			//	|| is_string   <T>
			//	|| is_view     <T>
			//	|| is_char_type<T>
			//	|| is_integral <T>
			//	|| is_floating_point<T>;
			///* size */
			//template <typename T>
			//static constexpr auto size_elem(const T& elem) noexcept -> size_type {
			//
			//	static_assert(self::is_appendable<T>,
			//			"T must be appendable to char_traits");
			//
			//	if constexpr (is_pointer<T>) {
			//		return self::length(elem);
			//	}
			//	else if constexpr (is_array<T>) {
			//		constexpr size_type N = ms::extent<T>;
			//		return elem[N - 1] == self::null ? N - 1 : N;
			//	}
			//	else if constexpr (is_string<T> || is_view<T>) {
			//		return elem.size();
			//	}
			//	else if constexpr (is_char_type<T>) {
			//		return 1U;
			//	}
			//	else if constexpr (is_integral<T>) {
			//		return ms::integral_traits<T>::chars_length(elem);
			//	}
			//	else if constexpr (is_floating_point<T>) {
			//		return ms::floating_traits<T>::chars_length(elem);
			//	}
			//}
			//
			///* append */
			//template <typename T>
			//static constexpr auto append_elem(pointer dst, const T& src, const size_type size) noexcept -> void {
			//
			//	static_assert(self::is_appendable<T>,
			//			"T must be appendable to char_traits");
			//
			//	if constexpr (is_pointer<T> || is_array<T>) {
			//		ms::memcpy(dst, src, size);
			//	}
			//	else if constexpr (is_string<T> || is_view<T>) {
			//		ms::memcpy(dst, src.data(), size);
			//	}
			//	else if constexpr (is_char_type<T>) {
			//		dst[0U] = src;
			//	}
			//	else if constexpr (is_integral<T>) {
			//		ms::append_traits<C>::to_chars(dst, src, size);
			//	}
			//	else if constexpr (is_floating_point<T>) {
			//		ms::append_traits<C>::to_chars(dst, src, size);
			//	}
			//}
			///* clean */
			//template <typename T>
			//using clean = ms::remove_cvr<T>;
