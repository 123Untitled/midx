#ifndef core_string_string_view_hpp
#define core_string_string_view_hpp

#include "core/type_traits/traits.hpp"
#include "core/string/char_traits.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- S T R I N G  V I E W ------------------------------------------------

	class string_view final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self          = mx::string_view;


		public:

			// -- public types ------------------------------------------------

			/* char type */
			using char_type     = char;

			/* const pointer type */
			using const_pointer = const char*;

			/* size type */
			using size_type     = mx::usz;


		private:

			// -- private members ---------------------------------------------

			/* data */
			const_pointer _data;

			/* size */
			size_type _size;


			static constexpr const char* _empty = "";


		public:

			// -- public lifecycle ---------------------------------------------

			/* default constructor */
			constexpr string_view(void) noexcept
			: _data{self::_empty}, _size{0U} {
			}

			/* buffer constructor */
			explicit constexpr string_view(const_pointer data) noexcept
			: _data{data}, _size{mx::strlen(data)} {

				// do not check for better performance
				/*
				if (_data != nullptr)
					_size = traits::length(_data);
				else
					_data = traits::empty;
				*/
			}

			/* members constructor */
			constexpr string_view(const_pointer data, const size_type size) noexcept
			: _data{data}, _size{size} {
			}

			/* copy constructor */
			constexpr string_view(const self&) noexcept = default;

			/* move constructor */
			constexpr string_view(self&&) noexcept = default;

			/* destructor */
			constexpr ~string_view(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			constexpr auto operator=(self&&) noexcept -> self& = default;

			/* buffer assignment operator */
			constexpr auto operator=(const_pointer data) noexcept -> void {
				_size = mx::strlen(data);
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
				_data = self::_empty;
				_size = 0U;
			}

	}; // class string_view

} // namespace mx

#endif // core_string_string_view_hpp
