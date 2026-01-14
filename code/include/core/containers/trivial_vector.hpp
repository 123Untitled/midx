#pragma once

#include <type_traits>
#include <limits>
#include <iostream>

#include "core/type_traits/type_transformations/conditional.hpp"
#include "core/memory/malloc.hpp"
#include "core/memory/memcpy.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- T R I V I A L  V E C T O R ------------------------------------------

	template <typename T>
	class trivial_vector final {


		// -- assertions ------------------------------------------------------

		/* trivially copyable assertion */
		static_assert(std::is_trivially_copyable_v<T>,
				"trivial_vector<T>: T must be trivially copyable");

		/* trivially destructible assertion */
		static_assert(std::is_trivially_destructible_v<T>,
				"trivial_vector<T>: T must be trivially destructible");


		private:

			// -- forward declarations ----------------------------------------

			/* iterator */
			template <bool, bool>
			class iter;


		public:

			// -- public types ------------------------------------------------

			/* value type */
			using value_type             = T;

			/* reference type */
			using reference              = value_type&;

			/* const reference type */
			using const_reference        = const value_type&;

			/* pointer type */
			using pointer                = value_type*;

			/* const pointer type */
			using const_pointer          = const value_type*;

			/* size type */
			using size_type              = decltype(sizeof(0));

			/* iterator type */
			using iterator               = iter<false, false>;

			/* const iterator type */
			using const_iterator         = iter<true,  false>;

			/* reverse iterator type */
			using reverse_iterator       = iter<false, true>;

			/* const reverse iterator type */
			using const_reverse_iterator = iter<true,  true>;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::trivial_vector<T>;


			// -- private members ---------------------------------------------

			/* data */
			pointer _data;

			/* size */
			size_type _size;

			/* capacity */
			size_type _capacity;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			trivial_vector(void) noexcept
			: _data{nullptr}, _size{0U}, _capacity{0U} {
			}

			/* capacity constructor */
			trivial_vector(const size_type& size)
			: _data{ms::allocate<value_type>(size)}, _size{0U}, _capacity{size} {
			}

			/* copy constructor */
			trivial_vector(const self& other)
			: _data{nullptr}, _size{other._size}, _capacity{other._size} {

				if (_size == 0U)
					return;

				// allocate memory
				_data = ms::allocate<value_type>(_size);

				// copy data
				ms::memcpy(_data, other._data, _size);
			}

			/* move constructor */
			trivial_vector(self&& other) noexcept
			: _data{other._data}, _size{other._size}, _capacity{other._capacity} {

				// invalidate other
				other._init();
			}

			/* destructor */
			~trivial_vector(void) noexcept {

				// deallocate memory
				self::_deallocate();
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self& other) -> self& {

				// check for self-assignment
				if (this == &other)
					return *this;

				// expand capacity if necessary
				if (_capacity < other._size) {
					self::_reserve(other._size);
				}

				// copy members
				_size     = other._size;
				//_capacity = other._size;

				// copy data
				ms::memcpy(_data, other._data, _size);

				// done
				return *this;
			}

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				// check for self-assignment
				if (this == &other)
					return *this;

				// free memory
				self::_deallocate();

				// move members
				self::_copy_members(other);

				// invalidate other
				other._init();

				// done
				return *this;
			}


			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) noexcept -> iterator {
				return iterator{_data};
			}

			/* begin */
			auto begin(void) const noexcept -> const_iterator {
				return const_iterator{_data};
			}

			/* end */
			auto end(void) noexcept -> iterator {
				return iterator{_data + _size};
			}

			/* end */
			auto end(void) const noexcept -> const_iterator {
				return const_iterator{_data + _size};
			}

			/* rbegin */
			auto rbegin(void) noexcept -> reverse_iterator {
				return reverse_iterator{_data + _size - 1U};
			}

			/* rbegin */
			auto rbegin(void) const noexcept -> const_reverse_iterator {
				return const_reverse_iterator{_data + _size - 1U};
			}

			/* rend */
			auto rend(void) noexcept -> reverse_iterator {
				return reverse_iterator{_data - 1U};
			}

			/* rend */
			auto rend(void) const noexcept -> const_reverse_iterator {
				return const_reverse_iterator{_data - 1U};
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

			/* size */
			auto size(void) const noexcept -> const size_type& {
				return _size;
			}

			/* capacity */
			auto capacity(void) const noexcept -> const size_type& {
				return _capacity;
			}

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}


			// -- public element access ---------------------------------------

			/* operator [] */
			auto operator[](const size_type pos) noexcept -> reference {
				return _data[pos];
			}

			/* operator [] */
			auto operator[](const size_type pos) const noexcept -> const_reference {
				return _data[pos];
			}


			// -- public modifiers --------------------------------------------

			/* reserve */
			auto reserve(const size_type cap) -> void {

				if (cap > _capacity)
					self::_reserve(cap);
			}

			/* resize */
			auto resize(const size_type size) -> void {

				if (size > _capacity)
					self::_reserve(size);

				_size = size;
			}

			/* clear */
			auto clear(void) noexcept -> void {
				_size = 0U;
			}

			/* push back */
			auto push_back(const value_type& value) -> void {

				// check capacity
				if (_size == _capacity)
					self::_reserve(self::_expand_size());

				// copy construct
				_data[_size] = value;

				++_size;
			}

			/* emplace back */
			template <typename... Ts>
			auto emplace_back(Ts&&... args) -> void {

				// check capacity
				if (_size == _capacity)
					self::_reserve(self::_expand_size());

				// emplace construct
				::new(_data + _size) value_type{std::forward<Ts>(args)...};

				++_size;
			}

			/* pop back */
			auto pop_back(void) noexcept -> void {
				--_size;
			}

			/* erase */
			auto swap_and_pop(const size_type pos) noexcept -> bool {

				// decrease size
				--_size;

				// swap if not the last element
				return pos != _size ? _data[pos] = _data[_size], true : false;
			}


			auto debug(void) -> void {

				for (size_type i = 0U; i < _size; ++i) {
					std::cout << _data[i] << " ";
				}
				std::cout << "\r\n";
			}


		private:

			// -- private methods ---------------------------------------------

			/* init */
			auto _init(void) noexcept -> void {
				_data     = nullptr;
				_size     = 0U;
				_capacity = 0U;
			}

			/* copy members */
			auto _copy_members(self& other) noexcept -> void {
				_data     = other._data;
				_size     = other._size;
				_capacity = other._capacity;
			}

			/* available capacity */
			auto _available_capacity(void) const noexcept -> size_type {
				return _capacity - _size;
			}

			/* expand */
			auto _expand_size(void) const -> size_type {

				if (_capacity == 0U)
					return 1U;

				#undef max
				// get maximum capacity
				constexpr auto max = std::numeric_limits<size_type>::max();


				if (_capacity == max)
					throw ms::runtime_error{"trivial_vector<T>: capacity overflow"};

				constexpr auto limit = max / 2U;

				if (_capacity > limit)
					return max;

				return _capacity << 1U;
			}

			/* try expand */
			auto _try_expand(const size_type cap) const noexcept -> size_type {
				// check overflow
				const auto ncap = _capacity << 1U;

				return ncap < cap ? cap : ncap;
			}

			/* deallocate */
			auto _deallocate(void) noexcept -> void {
				if (_data == nullptr)
					return;
				ms::deallocate(_data);
			}

			/* reserve */
			auto _reserve(const size_type cap) -> void {

				// reallocate new memory
				auto ptr = ms::reallocate<value_type>(_data, cap);

				// update members
				_data     = ptr;
				_capacity = cap;
			}


			// -- private classes ---------------------------------------------

			/* iterator */
			template <bool C, bool R>
			class iter final {


				// -- friend classes ------------------------------------------

				/* trivial_vector as friend */
				friend class ms::trivial_vector<T>;


				private:

					// -- private types ---------------------------------------

					/* self type */
					using self = iter<C, R>;


					// -- private members -------------------------------------

					/* data */
					pointer _data;


					// -- private lifecycle -----------------------------------

					/* data constructor */
					iter(pointer data) noexcept
					: _data{data} {
					}


				public:

					// -- public lifecycle ------------------------------------

					/* default constructor */
					iter(void) noexcept
					: _data{nullptr} {
					}

					/* copy constructor */
					iter(const self&) noexcept = default;

					/* copy constructor (conversion) */
					iter(const iter<!C, R>& other) noexcept
					: _data{other._data} {

						static_assert(C,
							"cannot convert const iterator to non-const iterator");
					}

					/* destructor */
					~iter(void) noexcept = default;


					// -- public assignment operators -------------------------

					/* copy assignment operator */
					auto operator=(const self&) noexcept -> self& = default;

					/* copy assignment operator (conversion) */
					auto operator=(const iter<!C, R>& other) noexcept -> self& {

						static_assert(C,
							"cannot convert const iterator to non-const iterator");

						_data = other._data;

						return *this;
					}


					// -- public comparison operators -------------------------

					/* equality operator */
					auto operator==(const self& other) const noexcept -> bool {
						return _data == other._data;
					}

					/* inequality operator */
					auto operator!=(const self& other) const noexcept -> bool {
						return _data != other._data;
					}


					// -- public arithmetic operators -------------------------

					/* pre-increment operator */
					auto operator++(void) noexcept -> self& requires (!R) {
						++_data;
						return *this;
					}

					/* pre-increment operator (reverse) */
					auto operator++(void) noexcept -> self& requires (!!R) {
						--_data;
						return *this;
					}

					/* pre-decrement operator */
					auto operator--(void) noexcept -> self& requires (!R) {
						--_data;
						return *this;
					}

					/* pre-decrement operator (reverse) */
					auto operator--(void) noexcept -> self& requires (!!R) {
						++_data;
						return *this;
					}


					// -- public operator -------------------------------------

					/* arrow operator */
					auto operator->(void) const noexcept
						-> ms::conditional<C, const_pointer, pointer> {
						return _data;
					}

					/* dereference operator */
					auto operator*(void) const noexcept
						-> ms::conditional<C, const_reference, reference> {
						return *_data;
					}

			}; // class iter

	}; // class trivial_vector

} // namespace ms
