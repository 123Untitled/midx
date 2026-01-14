#ifndef monitoring_event_fd_hpp
#define monitoring_event_fd_hpp

#include "core/types.hpp"
#include "monitoring/watcher.hpp"

#include <sys/event.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	class event_fd final : public mx::watcher {


		private:

			// -- private members ---------------------------------------------

			int _fd;


			// -- private lifecycle -------------------------------------------

			/* fd constructor */
			event_fd(const int fd) noexcept
			: _fd{fd} {
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			event_fd(void) noexcept
			: mx::watcher{}, _fd{-1} {
			}

			/* destructor */
			~event_fd(void) noexcept = default;



			// -- public interface --------------------------------------------

			/* on event */
			auto on_event(mx::application&, const struct ::kevent&) -> void override {
			}

			/* make add */
			auto make_add(void) noexcept -> struct ::kevent override {
				struct ::kevent ev {
					.ident  = 0U,
					.filter = EVFILT_USER,
					.flags  = EV_ADD | EV_ONESHOT,
					.fflags = 0U,
					.data   = 0,
					.udata  = nullptr
				};
				return ev;
			}

			/* make del */
			auto make_del(void) const noexcept -> struct ::kevent override {
				struct ::kevent ev {
					.ident  = 0U,
					.filter = EVFILT_USER,
					.flags  = EV_DELETE,
					.fflags = 0U,
					.data   = 0,
					.udata  = nullptr
				};
				return ev;
			}


	}; // class event_fd

} // namespace mx

#endif // monitoring_event_fd_hpp
