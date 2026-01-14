//#ifndef core_memory_unique_ptr_hpp
//#define core_memory_unique_ptr_hpp
//
//#include "core/memory/malloc.hpp"
//#include "core/memory/alloc_guard.hpp"
//#include "core/memory/lifecycle.hpp"
//
//#include "midx/diagnostics/exception.hpp"
//#include "midx/type_traits/forward.hpp"
//#include "midx/type_traits/is_base_of.hpp"
//
//#include <iostream>
//
//
//// -- M X H  N A M E S P A C E ------------------------------------------------
//
//namespace msh {
//
//
//	// -- U N I Q U E  P T R --------------------------------------------------
//
//	template <typename T>
//	class unique_ptr final {
//
//
//		// -- friends ---------------------------------------------------------
//
//		/* unique pointer as friend */
//		template <typename>
//		friend class unique_ptr;
//
//		/* make unique as friend */
//		//template <typename U, typename... Ts>
//		//friend auto make_unique(Ts&&...) -> msh::unique_ptr<U>;
//
//
//		private:
//
//			// -- private types -----------------------------------------------
//
//			/* self type */
//			using self = msh::unique_ptr<T>;
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
//			using pointer = value_type*;
//
//			/* const pointer type */
//			using const_pointer = const value_type*;
//
//			/* reference type */
//			using reference = value_type&;
//
//			/* const reference type */
//			using const_reference = const value_type&;
//
//
//		private:
//
//			// -- private members ---------------------------------------------
//
//			/* data */
//			pointer _data;
//
//
//		public:
//
//			// -- public lifecycle --------------------------------------------
//
//			/* default constructor */
//			constexpr unique_ptr(void) noexcept
//			: _data{nullptr} {
//			}
//
//			/* nullptr constructor */
//			constexpr unique_ptr(decltype(nullptr)) noexcept
//			: _data{nullptr} {
//			}
//
//			/* data constructor */
//			template <typename U>
//			explicit constexpr unique_ptr(U* data) noexcept
//			: _data{data} {
//			}
//
//			/* deleted copy constructor */
//			unique_ptr(const self&) = delete;
//
//			/* move constructor */
//			constexpr unique_ptr(self&& other) noexcept
//			: _data{other._data} {
//
//				// invalidate other
//				other._data = nullptr;
//			}
//
//			/* move constructor (derived) */
//			template <typename U>
//			constexpr unique_ptr(msh::unique_ptr<U>&& other) noexcept
//			: _data{other._data} {
//
//				// invalidate other
//				other._data = nullptr;
//
//				// check for derived type
//				static_assert(tt::is_base_of<T, U>,
//						"unique_ptr: invalid pointer type");
//			}
//
//			/* destructor */
//			constexpr ~unique_ptr(void) noexcept {
//
//				// destroy and deallocate
//				self::_free();
//			}
//
//
//			// -- public assignment operators ---------------------------------
//
//			/* deleted copy assignment operator */
//			auto operator=(const self&) -> self& = delete;
//
//			/* move assignment operator */
//			constexpr auto operator=(self&& other) noexcept -> self& {
//
//				// check for self assignment
//				if (_data == other._data)
//					return *this;
//
//				self::_free();
//
//				_data = other._data;
//				other._data = nullptr;
//
//				return *this;
//			}
//
//			/* move assignment operator (derived) */
//			template <typename U>
//			constexpr auto operator=(msh::unique_ptr<U>&& other) noexcept -> self& {
//
//				// check for self assignment
//				if (_data == other._data)
//					return *this;
//
//				self::_free();
//
//				_data = other._data;
//				other._data = nullptr;
//
//				// check for derived type
//				static_assert(tt::is_base_of<T, U>,
//						"unique_ptr: invalid pointer type");
//
//				return *this;
//			}
//
//			/* nullptr assignment operator */
//			constexpr auto operator=(decltype(nullptr)) noexcept -> self& {
//
//				self::_free();
//
//				_data = nullptr;
//
//				return *this;
//			}
//
//
//			// -- public modifiers --------------------------------------------
//
//			/* reset */
//			constexpr auto reset(void) noexcept -> void {
//
//				// destroy and deallocate
//				self::_free();
//
//				// reset data
//				_data = nullptr;
//			}
//
//			/* release */
//			constexpr auto release(void) noexcept -> pointer {
//
//				// get data
//				pointer data = _data;
//
//				// reset data
//				_data = nullptr;
//
//				// return data
//				return data;
//			}
//
//
//			// -- public accessors --------------------------------------------
//
//			/* dereference */
//			constexpr auto operator*(void) noexcept -> reference {
//				return *_data;
//			}
//
//			/* const dereference */
//			constexpr auto operator*(void) const noexcept -> const_reference {
//				return *_data;
//			}
//
//			/* arrow */
//			constexpr auto operator->(void) noexcept -> pointer {
//				return _data;
//			}
//
//			/* const arrow */
//			constexpr auto operator->(void) const noexcept -> const_pointer {
//				return _data;
//			}
//
//			/* get */
//			constexpr auto get(void) noexcept -> pointer {
//				return _data;
//			}
//
//			/* const get */
//			constexpr auto get(void) const noexcept -> const_pointer {
//				return _data;
//			}
//
//
//			// -- public conversion operators ---------------------------------
//
//			/* bool conversion operator */
//			explicit operator bool(void) const noexcept {
//				return _data != nullptr;
//			}
//
//
//			// -- public comparison operators ---------------------------------
//
//			/* nullptr equality operator */
//			auto operator==(decltype(nullptr)) const noexcept -> bool {
//				return _data == nullptr;
//			}
//
//			/* nullptr inequality operator */
//			auto operator!=(decltype(nullptr)) const noexcept -> bool {
//				return _data != nullptr;
//			}
//
//			/* equality operator */
//			template <typename U>
//			auto operator==(const msh::unique_ptr<U>& other) const noexcept -> bool {
//				return static_cast<void*>(_data) == static_cast<void*>(other._data);
//			}
//
//			/* inequality operator */
//			template <typename U>
//			auto operator!=(const msh::unique_ptr<U>& other) const noexcept -> bool {
//				return static_cast<void*>(_data) != static_cast<void*>(other._data);
//			}
//
//			/* less than operator */
//			template <typename U>
//			auto operator<(const msh::unique_ptr<U>& other) const noexcept -> bool {
//				return static_cast<void*>(_data) < static_cast<void*>(other._data);
//			}
//
//			/* greater than operator */
//			template <typename U>
//			auto operator>(const msh::unique_ptr<U>& other) const noexcept -> bool {
//				return static_cast<void*>(_data) > static_cast<void*>(other._data);
//			}
//
//			/* less than or equal operator */
//			template <typename U>
//			auto operator<=(const msh::unique_ptr<U>& other) const noexcept -> bool {
//				return static_cast<void*>(_data) <= static_cast<void*>(other._data);
//			}
//
//			/* greater than or equal operator */
//			template <typename U>
//			auto operator>=(const msh::unique_ptr<U>& other) const noexcept -> bool {
//				return static_cast<void*>(_data) >= static_cast<void*>(other._data);
//			}
//
//
//		private:
//
//
//			// -- private methods ---------------------------------------------
//
//			/* free */
//			auto _free(void) noexcept -> void {
//
//				// check allocated memory
//				if (_data == nullptr)
//					return;
//
//				// destruct object
//				_data->~T();
//
//				// free memory
//				msh::free(_data);
//			}
//
//	}; // class unique_ptr
//
//
//	// -- non-member functions ------------------------------------------------
//
//	/* make unique */
//	template <typename T, typename... Ts>
//	auto make_unique(Ts&&... args) -> msh::unique_ptr<T> {
//
//		// allocate memory
//		auto* ptr = msh::malloc<T>();
//
//		// create guard
//		msh::alloc_guard guard{ptr};
//
//		// construct object
//		msh::lifecycle<T>::construct(ptr, tt::forward<Ts>(args)...);
//
//		// release guard
//		guard.complete();
//
//		// return unique pointer
//		return msh::unique_ptr<T>{ptr};
//	}
//
//} // namespace msh
