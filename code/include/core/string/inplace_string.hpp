#ifndef core_string_inplace_string_hpp
#define core_string_inplace_string_hpp

#include "core/string/chars_encoder.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {

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

	template <unsigned N>
	class inplace_string final {


		// -- assertions ------------------------------------------------------

		/* assert that N is greater than 0 */
		static_assert(N > 0U, "Inplace string size must be greater than 0");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::inplace_string<N>;


			// -- private members ---------------------------------------------

			/* data */
			char _data[N];

			/* size */
			mx::usz _size;


			// -- private static members --------------------------------------

			/* capacity */
			static constexpr mx::usz _capacity = N - 1U; // reserve space for null terminator


			// -- private lifecycle -------------------------------------------

			/* encoder constructor */
			template <typename... Tp>
			constexpr explicit inplace_string(const mx::chars_encoder<Tp...>& encoder) noexcept
			/* uninitialized */ {

				// initialize at compile time
				if consteval {
					for (mx::usz i = 0U; i < N; ++i)
						_data[i] = '\0';
				}

				// encode
				_size = encoder.encode(_data, _data + _capacity);

				// null terminate
				_data[_size] = '\0';
			}



			template <typename... Tp>
			static constexpr bool any_encoder = (mx::is_chars_encoder<Tp> || ...);

			template <typename... Tp>
			static constexpr bool is_single_self =
				(sizeof...(Tp) == 1U) && (mx::is_same<self, mx::remove_cvr<Tp>> && ...);


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr inplace_string(void) noexcept
			: /* uninitialized */ _size{0U} {

				// initialize at compile time
				if consteval {
					for (mx::usz i = 0U; i < N; ++i)
						_data[i] = '\0';
				}

				*_data = '\0';
			}

			/* variadic constructor */
			template <typename... Tp> requires (not any_encoder<Tp...>)
											&& (not is_single_self<Tp...>)
											&& (sizeof...(Tp) > 0U)
			constexpr explicit inplace_string(Tp&&... args) noexcept
			: inplace_string{mx::chars_encoder<Tp...>{mx::forward<Tp>(args)...}} {
			}

			/* copy constructor */
			constexpr inplace_string(const self& other) noexcept
			: /* uninitialized */ _size{other._size} {

				// initialize at compile time
				if consteval {
					for (mx::usz i = 0U; i < N; ++i)
						_data[i] = '\0';
				}

				mx::memcpy(_data, other._data, _size);
				_data[_size] = '\0';
			}

			/* move constructor */
			constexpr inplace_string(self&& other) noexcept
			: inplace_string{other} {
			}

			/* destructor */
			constexpr ~inplace_string(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const self& other) noexcept -> self& {
				_size = other._size;
				mx::memcpy(_data, other._data, _size);
				_data[_size] = '\0';
				return *this;
			}

			/* move assignment operator */
			constexpr auto operator=(self&& other) noexcept -> self& {
				return self::operator=(other);
			}


			// -- public accessors --------------------------------------------

			/* data */
			constexpr auto data(void) noexcept -> char* {
				return _data;
			}

			/* const data */
			constexpr auto data(void) const noexcept -> const char* {
				return _data;
			}

			/* size */
			constexpr auto size(void) const noexcept -> mx::usz {
				return _size;
			}

			/* capacity */
			constexpr auto capacity(void) const noexcept -> mx::usz {
				return _capacity;
			}

			/* empty */
			constexpr auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}


			// -- public subscript operators ----------------------------------

			/* operator [] */
			constexpr auto operator[](const mx::usz pos) noexcept -> char& {
				return _data[pos];
			}

			/* operator [] */
			constexpr auto operator[](const mx::usz pos) const noexcept -> const char& {
				return _data[pos];
			}


			// -- public modifiers --------------------------------------------

			/* resize */
			constexpr auto resize(const mx::usz size) noexcept -> void {

				// set new size
				_size = (size < _capacity) ? size : _capacity;

				// null terminate
				_data[_size] = '\0';
			}

			/* clear */
			constexpr auto clear(void) noexcept -> void {

				// clear current string
				_size = 0U;

				// null terminate
				_data[_size] = '\0';
			}

			/* append */
			template <typename... Tp>
			constexpr auto append(Tp&&... args) noexcept -> void {

				// check if any arguments
				if constexpr (sizeof...(args) > 0U) {

					// create encoder
					const mx::chars_encoder<Tp...> encoder{mx::forward<Tp>(args)...};

					// encode
					_size += encoder.encode(_data + _size, _data + _capacity);

					// null terminate
					_data[_size] = '\0';
				}
			}

			/* assign */
			template <typename... Tp>
			constexpr auto assign(Tp&&... args) noexcept -> void {

				// check if any arguments
				if constexpr (sizeof...(args) > 0U) {

					// create encoder
					const mx::chars_encoder<Tp...> encoder{mx::forward<Tp>(args)...};

					// encode
					_size = encoder.encode(_data, _data + _capacity);
				}
				else {
					// clear current string
					_size = 0U;
				}

				// null terminate
				_data[_size] = '\0';
			}

	}; // class inplace_string

} // namespace mx

#endif // core_string_inplace_string_hpp
