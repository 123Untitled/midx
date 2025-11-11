#pragma once

#include "core/type_traits/traits.hpp"
#include "core/string/char_traits.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- B A S I C  S T R I N G  V I E W -------------------------------------

	template <typename T>
	class basic_string_view final {


		// -- assertions ------------------------------------------------------

		/* requires char type */
		static_assert(ms::is_character<T>,
					  "basic_string_view requires a character type (char, wchar_t, char8_t, char16_t, char32_t)");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self            = ms::basic_string_view<T>;

			/* traits type */
			using traits          = ms::char_traits<T>;


		public:

			// -- public types ------------------------------------------------

			/* char type */
			using char_type       = traits::char_type;

			/* const pointer type */
			using const_pointer   = traits::const_pointer;

			/* size type */
			using size_type       = traits::size_type;


		private:

			// -- private members ---------------------------------------------

			/* data */
			const_pointer _data;

			/* size */
			size_type _size;


		public:

			// -- public lifecycle ---------------------------------------------

			/* default constructor */
			constexpr basic_string_view(void) noexcept
			: _data{traits::empty}, _size{0U} {
			}

			/* buffer constructor */
			explicit constexpr basic_string_view(const_pointer data) noexcept
			: _data{data}, _size{traits::length(data)} {

				// do not check for better performance
				/*
				if (_data != nullptr)
					_size = traits::length(_data);
				else
					_data = traits::empty;
				*/
			}

			/* members constructor */
			constexpr basic_string_view(const_pointer data, const size_type size) noexcept
			: _data{data}, _size{size} {
			}

			/* copy constructor */
			constexpr basic_string_view(const self&) noexcept = default;

			/* move constructor */
			constexpr basic_string_view(self&&) noexcept = default;

			/* destructor */
			constexpr ~basic_string_view(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			constexpr auto operator=(self&&) noexcept -> self& = default;

			/* buffer assignment operator */
			constexpr auto operator=(const_pointer data) noexcept -> void {
				_size = traits::length(data);
				_data = data;
			}


			// -- public accessors --------------------------------------------

			/* data */
			constexpr auto data(void) const noexcept -> const_pointer {
				return _data;
			}

			/* size */
			constexpr auto size(void) const noexcept -> size_type {
				return _size;
			}

			/* empty */
			constexpr auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}


			// -- public element access ---------------------------------------

			/* operator[] */
			constexpr auto operator[](const size_type pos) const noexcept -> char_type {
				return _data[pos];
			}


			// -- public modifiers --------------------------------------------

			/* clear */
			constexpr auto clear(void) noexcept -> void {
				_data = traits::empty;
				_size = 0U;
			}

	}; // class basic_string_view



	// -- aliases -------------------------------------------------------------

	/* string view */
	using string_view    = ms::basic_string_view<char>;

	/* wide string view */
	using wstring_view   = ms::basic_string_view<wchar_t>;

	/* u8 string view */
	using u8string_view  = ms::basic_string_view<char8_t>;

	/* u16 string view */
	using u16string_view = ms::basic_string_view<char16_t>;

	/* u32 string view */
	using u32string_view = ms::basic_string_view<char32_t>;


	/* wstrv */
	using wstrv = ms::wstring_view;

	/* u8strv */
	using u8strv = ms::u8string_view;


} // namespace ms
