#ifndef ml_monitoring_watcher_hpp
#define ml_monitoring_watcher_hpp


// -- forward declarations ----------------------------------------------------

/* kevent struct */
struct kevent;


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	/* application */
	class application;


	// -- W A T C H E R -------------------------------------------------------

	class watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::watcher;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			watcher(void) noexcept = default;

			/* copy constructor */
			watcher(const self&) noexcept = default;

			/* move constructor */
			watcher(self&&) noexcept = default;

			/* destructor */
			virtual ~watcher(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public interface --------------------------------------------

			/* on event */
			virtual auto on_event(mx::application&, const struct ::kevent&) -> void = 0;

			/* ident */
			virtual auto ident(void) const noexcept -> int = 0;

			/* make add */
			//virtual auto make_add(void) noexcept -> struct ::kevent = 0;

			/* make del */
			//virtual auto make_del(void) const noexcept -> struct ::kevent = 0;

	}; // class watcher

} // namespace mx

#endif // ml_monitoring_watcher_hpp
