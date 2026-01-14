//#ifndef core_memory_alloc_guard_hpp
//#define core_memory_alloc_guard_hpp
//
//#include "core/memory/malloc.hpp"
//#include "core/utility/exchange.hpp"
//
//
//// -- M X  N A M E S P A C E --------------------------------------------------
//
//namespace mx {
//
//
//	// -- A L L O C  G U A R D ------------------------------------------------
//
//	template <typename T>
//	class alloc_guard final {
//
//
//		private:
//
//			// -- private types -----------------------------------------------
//
//			/* self type */
//			using self = mx::alloc_guard<T>;
//
//
//			// -- private members ---------------------------------------------
//
//			/* data */
//			T* _data;
//
//
//		public:
//
//			// -- public lifecycle --------------------------------------------
//
//			/* deleted default constructor */
//			alloc_guard(void) = delete;
//
//			/* data constructor */
//			alloc_guard(T* data) noexcept
//			: _data{data} {
//			}
//
//			/* deleted copy constructor */
//			alloc_guard(const self&) = delete;
//
//			/* deleted move constructor */
//			alloc_guard(self&&) = delete;
//
//			/* destructor */
//			~alloc_guard(void) noexcept {
//
//				// check for complete
//				if (_data == nullptr)
//					return;
//
//				// deallocate memory
//				mx::free(_data);
//			}
//
//
//			// -- public assignment operators ---------------------------------
//
//			/* deleted copy assignment operator */
//			auto operator=(const self&) -> self& = delete;
//
//			/* deleted move assignment operator */
//			auto operator=(self&&) noexcept -> self& = delete;
//
//
//			// -- public accessors --------------------------------------------
//
//			/* data */
//			auto data(void) const noexcept -> void* {
//				return _data;
//			}
//
//			/* get */
//			auto get(void) const noexcept -> T* {
//				return _data;
//			}
//
//
//			// -- public modifiers --------------------------------------------
//
//			/* complete */
//			auto complete(void) noexcept -> void {
//				_data = nullptr;
//			}
//
//			/* release */
//			auto release(void) noexcept -> T* {
//				return mx::exchange(_data, nullptr);
//			}
//
//	}; // class alloc_guard
//
//} // namespace mx
//
//#endif // core_memory_alloc_guard_hpp
