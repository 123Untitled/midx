#ifndef ml_monitoring_monitor_hpp
#define ml_monitoring_monitor_hpp

#include "core/types.hpp"
#include "system/unix_descriptor.hpp"
#include "monitoring/watcher.hpp"

#include <sys/event.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	/* application */
	class application;

	/* watcher */
	class watcher;


	// -- M O N I T O R -------------------------------------------------------

	class monitor final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::monitor;


			// -- private constants -------------------------------------------

			/* buffer size */
			static constexpr int BUFFER_SIZE = 16U;


			// -- private members ---------------------------------------------

			/* kqueue */
			mx::unix_descriptor _kqueue;

			/* events */
			struct ::kevent _events[BUFFER_SIZE];


			// -- private methods ---------------------------------------------

			/* record */
			template <mx::i16 FILTER, mx::u16 FLAGS>
			auto record(const int ident, void* data = nullptr) const -> void {

				// make event
				const struct ::kevent ev {
					.ident  = static_cast<::uintptr_t>(ident),
					.filter = FILTER,
					.flags  = FLAGS,
					.fflags = 0U,
					.data   = 0,
					.udata  = data
				};

				// record event
				if (::kevent(_kqueue, &ev, 1, nullptr, 0, nullptr) == -1)
					throw mx::system_error{"kevent"};
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			monitor(void);

			/* deleted copy constructor */
			monitor(const self&) = delete;

			/* move constructor */
			monitor(self&&) noexcept = default;

			/* destructor */
			~monitor(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* add read */
			auto add_read(mx::watcher&) const -> void;

			/* add write */
			auto add_write(mx::watcher&) const -> void;

			/* del read */
			auto del_read(mx::watcher&) const -> void;

			/* del write */
			auto del_write(mx::watcher&) const -> void;


			/* add user */
			auto add_user(mx::watcher&) const -> void;


			/* wait */
			auto wait(mx::application&) -> void;


			// -- public accessors --------------------------------------------

			/* kqueue */
			auto kqueue(void) const noexcept -> const mx::unix_descriptor& {
				return _kqueue;
			}

	}; // class monitor

} // namespace mx

#endif // ml_monitoring_monitor_hpp
