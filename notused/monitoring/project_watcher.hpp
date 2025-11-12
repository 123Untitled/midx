#ifndef ml_monitoring_project_watcher_hpp
#define ml_monitoring_project_watcher_hpp

#include "monitoring/paths.hpp"
#include "monitoring/fs_watcher.hpp"
#include "monitoring/time_watcher.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	/* monitor */
	class monitor;


	// -- P R O J E C T  W A T C H E R ----------------------------------------

	class project_watcher final {

		private:

			mx::paths _paths;

			mx::fs_watcher _target;
			mx::fs_watcher _parent;
			mx::time_watcher _timer;


		public:

			// -- public lifecycle --------------------------------------------

			project_watcher(const char*, const mx::monitor&);


			// -- public methods ----------------------------------------------

			/* has_changes */
			auto has_changes(mx::monitor&) -> bool;

			/* target path */
			auto target_fd(void) const noexcept -> const mx::unix_descriptor& {
				return _target;
			}

	}; // class file_watcher

} // namespace mx

#endif // ml_monitoring_project_watcher_hpp
