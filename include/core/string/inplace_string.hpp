#pragma once

#include "core/string/char_traits.hpp"
#include "core/string/chars_encoder.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {

	// string name ideas:


	// static string with bound (fixed capacity, null terminator)

	// str_stack
	// str_fixed
	// str_static
	// str_bound
	// str_inplace
	// str_inline
	// str_buffer
	// str_array
	// str_safe
	// str_hard
	// str_bloc

	//ms::str_fast<wchar_t, 64>;


	// str_heap
	// str_peek
	// str_fast


	// view string (non-owning, read-only, no null terminator)

	// str_link
	// str_range
	// str_slice
	// str_peek
	// str_view
	// str_ref
	// str_span
	// str_borrowed
	// str_unowned
	// str_const
	// str_readonly


	// dynamic string (with heap allocation, resizable, null terminator)

	// str_owned
	// str_flex
	// str_heap
	// str_alloc
	// str_dynamic
	// str_mutable
	// str_mut
	// str_growable
	// str_buffered


	// -- I N P L A C E  B A S I C  S T R I N G -------------------------------

	template <typename C, unsigned N>
	class inplace_basic_string final {


		// -- assertions ------------------------------------------------------

		/* assert that N is greater than 0 */
		static_assert(N > 0U, "Inplace string size must be greater than 0");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self            = ms::inplace_basic_string<C, N>;

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

			/* data */
			char_type _data[N];

			/* size */
			size_type _size;


			// -- private static members --------------------------------------

			/* capacity */
			static constexpr size_type _capacity = N - 1U; // reserve space for null terminator


			// -- private lifecycle -------------------------------------------

			/* encoder constructor */
			template <typename... Tp>
			constexpr explicit inplace_basic_string(const ms::chars_encoder<C, Tp...>& encoder) noexcept
			/* uninitialized */ {

				// initialize at compile time
				if consteval {
					for (size_type i = 0U; i < N; ++i)
						_data[i] = traits::null; }

				// encode
				_size = encoder.encode(_data, _data + _capacity);

				// null terminate
				_data[_size] = traits::null;
			}



			template <typename... Tp>
			static constexpr bool any_encoder =
				(ms::is_chars_encoder<Tp> || ...);

			template <typename... Tp>
			static constexpr bool is_single_self =
				(sizeof...(Tp) == 1U) && (ms::is_same<self, ms::remove_cvr<Tp>> && ...);


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr inplace_basic_string(void) noexcept
			: /* uninitialized */ _size{0U} {

				// initialize at compile time
				if consteval {
					for (size_type i = 0U; i < N; ++i)
						_data[i] = traits::null; }

				*_data = traits::null;
			}

			/* variadic constructor */
			template <typename... Tp> requires (not any_encoder<Tp...>)
											&& (not is_single_self<Tp...>)
											&& (sizeof...(Tp) > 0U)
			constexpr explicit inplace_basic_string(Tp&&... args) noexcept
			: inplace_basic_string{ms::chars_encoder<C, Tp...>{ms::forward<Tp>(args)...}} {
			}

			/* copy constructor */
			constexpr inplace_basic_string(const self& other) noexcept
			: /* uninitialized */ _size{other._size} {

				// initialize at compile time
				if consteval {
					for (size_type i = 0U; i < N; ++i)
						_data[i] = traits::null; }

				traits::copy(_data, other._data, _size);
				_data[_size] = traits::null;
			}

			/* move constructor */
			constexpr inplace_basic_string(self&& other) noexcept
			: inplace_basic_string{other} {
			}

			/* destructor */
			constexpr ~inplace_basic_string(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const self& other) noexcept -> self& {
				_size = other._size;
				traits::copy(_data, other._data, _size);
				_data[_size] = traits::null;
				return *this;
			}

			/* move assignment operator */
			constexpr auto operator=(self&& other) noexcept -> self& {
				return self::operator=(other);
			}


			// -- public accessors --------------------------------------------

			/* data */
			constexpr auto data(void) noexcept -> pointer {
				return _data;
			}

			/* const data */
			constexpr auto data(void) const noexcept -> const_pointer {
				return _data;
			}

			/* size */
			constexpr auto size(void) const noexcept -> size_type {
				return _size;
			}

			/* capacity */
			constexpr auto capacity(void) const noexcept -> size_type {
				return _capacity;
			}

			/* empty */
			constexpr auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}


			// -- public subscript operators ----------------------------------

			/* operator [] */
			constexpr auto operator[](const size_type pos) noexcept -> reference {
				return _data[pos];
			}

			/* operator [] */
			constexpr auto operator[](const size_type pos) const noexcept -> char_type {
				return _data[pos];
			}


			// -- public modifiers --------------------------------------------

			/* resize */
			constexpr auto resize(const size_type size) noexcept -> void {

				// set new size
				_size = (size < _capacity) ? size : _capacity;

				// null terminate
				_data[_size] = traits::null;
			}

			/* clear */
			constexpr auto clear(void) noexcept -> void {

				// clear current string
				_size = 0U;

				// null terminate
				_data[_size] = traits::null;
			}

			/* append */
			template <typename... Tp>
			constexpr auto append(Tp&&... args) noexcept -> void {

				// check if any arguments
				if constexpr (sizeof...(args) > 0U) {

					// create encoder
					const ms::chars_encoder<C, Tp...> encoder{ms::forward<Tp>(args)...};

					// encode
					_size += encoder.encode(_data + _size, _data + _capacity);

					// null terminate
					_data[_size] = traits::null;
				}
			}

			/* assign */
			template <typename... Tp>
			constexpr auto assign(Tp&&... args) noexcept -> void {

				// check if any arguments
				if constexpr (sizeof...(args) > 0U) {

					// create encoder
					const ms::chars_encoder<C, Tp...> encoder{ms::forward<Tp>(args)...};

					// encode
					_size = encoder.encode(_data, _data + _capacity);
				}
				else {
					// clear current string
					_size = 0U;
				}

				// null terminate
				_data[_size] = traits::null;
			}

	}; // class inplace_basic_string



	// -- aliases -------------------------------------------------------------

	/* inplace string */
	template <unsigned N>
	using inplace_string    = ms::inplace_basic_string<char, N>;

	/* inplace wstring */
	template <unsigned N>
	using inplace_wstring   = ms::inplace_basic_string<wchar_t, N>;

	/* inplace u8string */
	template <unsigned N>
	using inplace_u8string  = ms::inplace_basic_string<char8_t, N>;

	/* inplace u16string */
	template <unsigned N>
	using inplace_u16string = ms::inplace_basic_string<char16_t, N>;

	/* inplace u32string */
	template <unsigned N>
	using inplace_u32string = ms::inplace_basic_string<char32_t, N>;


} // namespace ms
