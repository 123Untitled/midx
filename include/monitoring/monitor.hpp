#ifndef ml_monitoring_monitor_hpp
#define ml_monitoring_monitor_hpp

#include "system/unix_descriptor.hpp"

#include <sys/event.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


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
			using self = ml::monitor;


			// -- private constants -------------------------------------------

			/* buffer size */
			static constexpr int BUFFER_SIZE = 4U;


			// -- private members ---------------------------------------------

			/* kqueue */
			ml::unix_descriptor _kqueue;

			/* events */
			struct ::kevent _events[BUFFER_SIZE];


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

			/* subscribe */
			auto subscribe(ml::watcher&) const -> void;

			/* unsubscribe */
			auto unsubscribe(ml::watcher&) const -> void;

			/* record */
			auto record(const struct ::kevent&) const -> void;


			/* wait */
			auto wait(ml::application&) -> void;

	}; // class monitor

} // namespace ml

#endif // ml_monitoring_monitor_hpp
