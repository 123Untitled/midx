#ifndef ml_monitoring_time_watcher_hpp
#define ml_monitoring_time_watcher_hpp

#include "monitoring/watcher.hpp"

#include <sys/event.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* application */
	class application;


	// -- T I M E  W A T C H E R ----------------------------------------------

	class time_watcher final : public ml::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::time_watcher;


			// -- private members ---------------------------------------------

			/* milliseconds */
			::intptr_t _ms;

			/* triggered */
			bool _triggered;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			time_watcher(void) noexcept
			: _ms{1000},
			  _triggered{false} {
			}

			/* milliseconds constructor */
			explicit time_watcher(const ::intptr_t ms) noexcept
			: _ms{ms},
			  _triggered{false} {
			}

			/* copy constructor */
			time_watcher(const self&) noexcept = default;

			/* move constructor */
			time_watcher(self&&) noexcept = default;

			/* destructor */
			~time_watcher(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(ml::application&, const struct ::kevent&) -> void override final {
				_triggered = true;
			}

			/* make add */
			auto make_add(void) noexcept -> struct ::kevent override final {
				return {
					.ident  = 0U,
					.filter = EVFILT_TIMER,
					.flags  = EV_ADD | EV_ONESHOT,
					.fflags = NOTE_USECONDS,
					.data   = _ms * 1'000,
					.udata  = static_cast<void*>(this)
				};
			}

			/* make del */
			auto make_del(void) const noexcept -> struct ::kevent override final {
				return {
					.ident  = 0U,
					.filter = EVFILT_TIMER,
					.flags  = EV_DELETE,
					.fflags = 0U,
					.data   = 0U,
					.udata  = nullptr
				};
			}

			// -- public accessors --------------------------------------------

			/* is triggered */
			auto is_triggered(void) const noexcept -> bool {
				return _triggered;
			}


			// -- public modifiers --------------------------------------------

			/* pop state */
			auto pop_state(void) noexcept -> bool {
				const auto ret = _triggered;
				_triggered = false;
				return ret;
			}

			/* reset */
			auto reset(void) noexcept -> void {
				_triggered = false;
			}

	}; // class time_watcher

} // namespace ml

#endif // ml_monitoring_time_watcher_hpp
