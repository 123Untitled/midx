//#pragma once
//
//#include "memory/malloc.hpp"
//#include "core/types.hpp"
//
//
//// -- M X  N A M E S P A C E --------------------------------------------------
//
//namespace mx {
//
//
//	// -- A L L O C A T I O N -------------------------------------------------
//
//	template <typename T>
//	class allocation final {
//
//
//		public:
//
//			// -- public types ------------------------------------------------
//
//			/* value type */
//			using value_type = T;
//
//			/* pointer type */
//			using pointer = T*;
//
//			/* const pointer type */
//			using const_pointer = const T*;
//
//			/* reference type */
//			using reference = T&;
//
//			/* const reference type */
//			using const_reference = const T&;
//
//			/* size type */
//			using size_type = mx::size_t;
//
//
//		private:
//
//			// -- private types -----------------------------------------------
//
//			/* self type */
//			using self = mx::allocation<T>;
//
//
//			// -- private members ---------------------------------------------
//
//			/* data */
//			pointer _data;
//
//			/* size */
//			size_type _size;
//
//
//		public:
//
//			// -- public lifecycle --------------------------------------------
//
//			/* default constructor */
//			allocation(void) noexcept
//			: _data{nullptr}, _size{0U} {
//			}
//
//			/* size constructor */
//			allocation(const size_type& size)
//			: _data{size ? ms::malloc<T>(size) : nullptr}, _size{size} {
//			}
//
//			/* copy constructor */
//			allocation(const self& other)
//			: _data{other._size ? ms::malloc<T>(other._size) : nullptr}, _size{other._size} {
//			}
//
//			/* move constructor */
//			allocation(self&& other) noexcept
//			: _data{other._data}, _size{other._size} {
//				other._data = nullptr;
//				other._size = 0U;
//			}
//
//			/* destructor */
//			~allocation(void) {
//
//				if (_data == nullptr)
//					return;
//				ms::free(_data);
//			}
//
//
//			// -- public assignment operators ---------------------------------
//
//			/* copy assignment operator */
//			auto operator=(const self& other) -> self& {
//
//				if (this == &other)
//					return *this;
//
//				if (_data != nullptr)
//					ms::free(_data);
//
//				_data = other._size ? ms::malloc<T>(other._size) : nullptr;
//				_size = other._size;
//
//				return *this;
//			}
//
//			/* move assignment operator */
//			auto operator=(self&& other) noexcept -> self& {
//
//				if (this == &other)
//					return *this;
//
//				if (_data != nullptr)
//					ms::free(_data);
//
//				_data = other._data;
//				_size = other._size;
//
//				other._data = nullptr;
//				other._size = 0U;
//
//				return *this;
//			}
//
//
//			// -- public accessors --------------------------------------------
//
//			/* data */
//			auto data(void) noexcept -> pointer {
//				return _data;
//			}
//
//			/* const data */
//			auto data(void) const noexcept -> const_pointer {
//				return _data;
//			}
//
//			/* size */
//			auto size(void) const noexcept -> size_type {
//				return _size;
//			}
//
//			/* is allocated */
//			auto is_allocated(void) const noexcept -> bool {
//				return _data != nullptr;
//			}
//
//
//			// -- public operators --------------------------------------------
//
//			/* [] operator */
//			auto operator[](const size_type& index) noexcept -> reference {
//				return _data[index];
//			}
//
//			/* const [] operator */
//			auto operator[](const size_type& index) const noexcept -> const_reference {
//				return _data[index];
//			}
//
//
//			// -- public modifiers --------------------------------------------
//
//			/* resize */
//			auto resize(const size_type& size) -> void {
//
//				if (size == 0U) {
//
//					if (_data != nullptr)
//						ms::free(_data);
//
//					_data = nullptr;
//					_size = 0U;
//					return;
//				}
//
//				if (_size == size)
//					return;
//
//				_data = ms::realloc(_data, size);
//				_size = size;
//			}
//
//	}; // class allocation
//
//} // namespace mx
