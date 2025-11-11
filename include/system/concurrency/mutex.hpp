#ifndef system_concurrency_mutex_hpp
#define system_concurrency_mutex_hpp

#include <pthread.h>
#include "diagnostics/system_error.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M U T E X -----------------------------------------------------------

	class mutex final {


		// -- friends ---------------------------------------------------------

		/* condition variable */
		friend class condition_variable;


		private:

			// -- public types ------------------------------------------------

			/* self type */
			using self = mx::mutex;


			// -- private members ---------------------------------------------

			/* mutex */
			::pthread_mutex_t _mutex;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr mutex(void) noexcept {
				_mutex = PTHREAD_MUTEX_INITIALIZER;
			}

			/* deleted copy constructor */
			mutex(const self&) = delete;

			/* deleted move constructor */
			mutex(self&&) = delete;

			/* destructor */
			~mutex(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public methods ----------------------------------------------

			/* lock */
			auto lock(void) -> void {

				if (::pthread_mutex_lock(&_mutex) != 0)
					throw ml::system_error{"pthread_mutex_lock"};

				// EINVAL The value specified by mutex is invalid.
				// EDEADLK A deadlock would occur if the thread blocked waiting for mutex.
			}

			/* try lock */
			auto try_lock(void) -> bool {

				const auto result = ::pthread_mutex_trylock(&_mutex);

				if (result == 0)
					return true;

				if (result == EBUSY)
					return false;

				throw ml::system_error{"pthread_mutex_trylock"};

				// EINVAL The value specified by mutex is invalid.
				// EBUSY Mutex is already locked.
			}

			/* unlock */
			auto unlock(void) -> void {

				if (::pthread_mutex_unlock(&_mutex) != 0)
					throw ml::system_error{"pthread_mutex_unlock"};

				// EINVAL The value specified by mutex is invalid.
				// EPERM The current thread does not hold a lock on mutex.
			}

			/* unlock noexcept */
			auto unlock_noexcept(void) noexcept -> void {

				// cannot throw in destructors
				static_cast<void>(::pthread_mutex_unlock(&_mutex));
			}

	}; // class mutex

} // namespace mx

#endif // system_concurrency_mutex_hpp
