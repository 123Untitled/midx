#ifndef system_concurrency_condition_variable_hpp
#define system_concurrency_condition_variable_hpp

#include "diagnostics/system_error.hpp"
#include "system/concurrency/mutex.hpp"
#include <pthread.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- C O N D I T I O N  V A R I A B L E ----------------------------------

	class condition_variable final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::condition_variable;


			// -- private members ---------------------------------------------

			/* condition variable */
			::pthread_cond_t _cond;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			condition_variable(void) noexcept {
				_cond = PTHREAD_COND_INITIALIZER;
			}

			/* deleted copy constructor */
			condition_variable(const self&) = delete;

			/* deleted move constructor */
			condition_variable(self&&) = delete;

			/* destructor */
			~condition_variable(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public methods ----------------------------------------------

			/* signal */
			auto signal(void) noexcept -> void {

				// [EINVAL] The value specified by cond is invalid.
				// assume always success
				static_cast<void>(::pthread_cond_signal(&_cond));
			}

			/* broadcast */
			auto broadcast(void) noexcept -> void {

				// [EINVAL] The value specified by cond is invalid.
				// assume always success
				static_cast<void>(::pthread_cond_broadcast(&_cond));
			}

			/* wait */
			auto wait(mx::mutex& mtx) -> void {

				// [EINVAL] The value specified by cond or the value specified by mutex is invalid.

				if (::pthread_cond_wait(&_cond, &mtx._mutex) != 0)
					throw ml::system_error{"pthread_cond_wait"};
			}

	}; // class condition_variable

} // namespace mx

#endif // system_concurrency_condition_variable_hpp
