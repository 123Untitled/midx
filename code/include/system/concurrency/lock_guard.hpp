#ifndef system_concurrency_lock_guard_hpp
#define system_concurrency_lock_guard_hpp


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- L O C K  G U A R D --------------------------------------------------

	template <typename T>
	class lock_guard final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::lock_guard<T>;


			// -- private members ---------------------------------------------

			/* mutex */
			T& _mtx;


		public:

			// -- public types ------------------------------------------------

			/* mutex type */
			using mutex_type = T;


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			lock_guard(void) = delete;

			/* mutex constructor */
			lock_guard(mutex_type& mtx)
			: _mtx{mtx} {

				// lock mutex
				_mtx.lock();
			}

			/* deleted copy constructor */
			lock_guard(const self&) = delete;

			/* deleted move constructor */
			lock_guard(self&&) = delete;

			/* destructor */
			~lock_guard(void) noexcept {

				// unlock mutex
				_mtx.unlock_noexcept();
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;

	}; // class lock_guard

} // namespace mx

#endif // system_concurrency_lock_guard_hpp
