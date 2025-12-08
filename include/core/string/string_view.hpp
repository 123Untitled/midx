#ifndef core_string_string_view_hpp
#define core_string_string_view_hpp

#include "core/types.hpp"
#include "core/string/strlen.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- S T R I N G  V I E W ------------------------------------------------

	class string_view final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self          = mx::string_view;


			// -- private members ---------------------------------------------

			/* data */
			const char* _data;

			/* size */
			mx::usz _size;


			// -- private static members --------------------------------------

			/* empty string */
			static constexpr const char* _empty = "";


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr string_view(void) noexcept
			: _data{self::_empty}, _size{0U} {
			}

			/* buffer constructor */
			explicit constexpr string_view(const char* data) noexcept
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
			constexpr string_view(const char* data, const mx::usz size) noexcept
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
			constexpr auto operator=(const char* data) noexcept -> void {
				_size = mx::strlen(data);
				_data = data;
			}


			// -- public accessors --------------------------------------------

			/* data */
			constexpr auto data(void) const noexcept -> const char* {
				return _data;
			}

			/* size */
			constexpr auto size(void) const noexcept -> mx::usz {
				return _size;
			}

			/* empty */
			constexpr auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}


			// -- public element access ---------------------------------------

			/* operator[] */
			constexpr auto operator[](const mx::usz pos) const noexcept -> char {
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


// overload operator << for mx::string_view
#include <ostream>
inline auto operator<<(std::ostream& os, const mx::string_view& sv) -> std::ostream& {
	return os.write(sv.data(), static_cast<std::streamsize>(sv.size()));
}

#endif // core_string_string_view_hpp
