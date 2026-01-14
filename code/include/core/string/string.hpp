#ifndef core_string_hpp
#define core_string_hpp

#include "core/memory/memcpy.hpp"
#include "core/memory/memmove.hpp"
#include "core/memory/malloc.hpp"


//#include "core/string/inplace_string.hpp"

// fix limit and overflow in extend and try extend methods

#include "core/utility/exchange.hpp"
#include "core/string/chars_encoder.hpp"
#include "core/string/strcmp.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- S T R I N G ---------------------------------------------------------

	class string final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::string;


			// -- private members ---------------------------------------------

			/* capacity */
			mx::usz _capacity;

			/* data */
			char* _data;

			/* size */
			mx::usz _size;



			// -- private lifecycle -------------------------------------------

			/* encoder constructor */
			template <typename... Tp>
			explicit string(const mx::chars_encoder<Tp...>& encoder)
			: _capacity{encoder.size()},
			  _data{self::_allocate(_capacity)},
			  _size{encoder.encode(_data)} {

				// null terminate
				self::_null_terminate();
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			string(void) noexcept
			: _capacity{0U}, _data{self::_empty()}, _size{0U} {
			}

			template <typename... Tp>
			static constexpr bool any_encoder = (mx::is_chars_encoder<Tp> || ...);

			template <typename... Tp>
			static constexpr bool is_single_self =
				(sizeof...(Tp) == 1U) && (mx::is_same<self, mx::remove_cvr<Tp>> && ...);


			/* variadic constructor */
			template <typename... Tp> requires (not any_encoder<Tp...>)
											&& (not is_single_self<Tp...>)
											&& (sizeof...(Tp) > 0U)
			explicit string(Tp&&... args)
			: mx::string{mx::chars_encoder<Tp...>{mx::forward<Tp>(args)...}} {
			}

			/* copy constructor */
			string(const self& other)
			: _capacity{other._size}, _data{self::_empty()}, _size{_capacity} {

				// check size
				if (_size == 0U)
					return;

				// allocate memory
				_data = self::_allocate(other._size);

				// copy data
				mx::memcpy(_data, other._data, _size);

				// null terminate
				self::_null_terminate();
			}

			/* move constructor */
			string(self&& other) noexcept
			: _capacity{mx::exchange(other._capacity, 0U)},
				  _data{mx::exchange(other._data, self::_empty())},
				  _size{mx::exchange(other._size, 0U)} {
				// other is now empty
			}

			/* destructor */
			~string(void) noexcept {
				self::_deallocate();
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self& other) -> self& {

				if (this == &other)
					return *this;

				if (_capacity < other._size) {

					// allocate new memory
					char* ptr = self::_allocate(other._size);

					// deallocate memory
					self::_deallocate();

					// copy members
					_data     = ptr;
					_capacity = other._size;
				}

				// copy data
				mx::memcpy(_data, other._data, other._size);
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
				_capacity = mx::exchange(other._capacity, 0U);
				_data     = mx::exchange(other._data, self::_empty());
				_size     = mx::exchange(other._size, 0U);

				return *this;
			}


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> char* {
				return _data;
			}

			/* const data */
			auto data(void) const noexcept -> const char* {
				return _data;
			}

			/* size */
			auto size(void) const noexcept -> mx::usz {
				return _size;
			}

			/* capacity */
			auto capacity(void) const noexcept -> mx::usz {
				return _capacity;
			}

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}

			/* front */
			auto front(void) noexcept -> char& {
				return *_data;
			}

			/* front */
			auto front(void) const noexcept -> char {
				return *_data;
			}

			/* back */
			auto back(void) noexcept -> char& {
				return _data[_size - 1U];
			}

			/* back */
			auto back(void) const noexcept -> char {
				return _data[_size - 1U];
			}


			// -- public subscript operators ----------------------------------

			/* operator [] */
			auto operator[](const mx::usz pos) noexcept -> char& {
				return _data[pos];
			}

			/* operator [] */
			auto operator[](const mx::usz pos) const noexcept -> char {
				return _data[pos];
			}


			// -- public modifiers --------------------------------------------

			/* reserve */
			auto reserve(const mx::usz cap) -> void {
				// reallocate memory
				if (cap > _capacity)
					self::_reserve(cap);

				// does i need to null terminate?????
			}

			/* resize */
			auto resize(const mx::usz size) -> void {

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
			auto push_back(const char ch) -> void {

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
			auto pop_back(const mx::usz count = 1U) noexcept -> void {

				// reduce size
				_size = (_size < count) ? 0U : _size - count;

				// null terminate (maybe segfault)
				self::_null_terminate();
			}


		private:

			template <typename... Tp>
			auto _append_impl(const mx::chars_encoder<Tp...>& encoder) -> void {

				const mx::usz required = encoder.size();

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
			auto _assign_impl(const mx::chars_encoder<Tp...>& encoder) -> void {

				const mx::usz required = encoder.size();

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
						mx::chars_encoder<Ts...>{mx::forward<Ts>(args)...}
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
						mx::chars_encoder<Ts...>{mx::forward<Ts>(args)...}
					);

				// null terminate
				self::_null_terminate();
			}

			/* erase */
			auto erase(const mx::usz pos) noexcept -> void {

				// check position
				if (pos >= _size)
					return;

				// shift data
				mx::memmove(_data + pos, _data + pos + 1U, _size - pos);

				// update size
				--_size;

				// null terminate
				self::_null_terminate();
			}



			// -- public comparison operators ---------------------------------

			/* equal */
			auto operator==(const self& other) const noexcept -> bool {
				return mx::str_equal(*this, other);
			}

			/* not equal */
			auto operator!=(const self& other) const noexcept -> bool {
				return !mx::str_equal(*this, other);
			}

			/* less than */
			auto operator <(const self& other) const noexcept -> bool {
				return mx::str_lexicographical(*this, other) < 0;
			}

			/* greater than */
			auto operator >(const self& other) const noexcept -> bool {
				return mx::str_lexicographical(*this, other) > 0;
			}

			/* less than or equal to */
			auto operator<=(const self& other) const noexcept -> bool {
				return mx::str_lexicographical(*this, other) <= 0;
			}

			/* greater than or equal to */
			auto operator>=(const self& other) const noexcept -> bool {
				return mx::str_lexicographical(*this, other) >= 0;
			}


		private:

			// -- private static methods --------------------------------------

			/* allocate */
			static auto _allocate(const mx::usz size) -> char* {
				return mx::allocate<char>(size + 1U);
			}

			/* empty data pointer */
			static auto _empty(void) noexcept -> char* {
				static char str[1U] {'\0'};
				return str;
			}


			// -- private methods ---------------------------------------------

			/* reallocate */
			auto _reallocate(const mx::usz size) -> char* {
				return mx::reallocate<char>(_data, size + 1U);
			}

			/* null terminate */
			auto _null_terminate(void) noexcept -> void {
				_data[_size] = '\0';
			}

			/* available capacity */
			auto _available_capacity(void) const noexcept -> mx::usz {
				return _capacity - _size;
			}

			/* extend */
			auto _extend(void) const noexcept -> mx::usz {
				// check overflow
				return _capacity ? _capacity << 1U : 1U;
			}

			/* try extend */
			auto _try_extend(const mx::usz cap) const noexcept -> mx::usz {
				// check overflow
				const auto ncp = _capacity << 1U;
				return ncp < cap ? cap : ncp;
			}

			/* deallocate */
			auto _deallocate(void) noexcept -> void {
				if (_data == self::_empty())
					return;
				mx::deallocate(_data);
			}

			/* reserve */
			auto _reserve(const mx::usz& cap) -> void {

				_data = (_data != self::_empty())
					// reallocate new memory
					? self::_reallocate(cap)
					// allocate new memory
					: self::_allocate(cap);

				// update capacity
				_capacity = cap;
			}


	}; // class string

} // namespace mx


// overload operator << for mx::string
#include <ostream>
inline auto operator<<(std::ostream& os, const mx::string& str) -> std::ostream& {
	return os.write(str.data(), static_cast<std::streamsize>(str.size()));
}

#endif // core_string_hpp
