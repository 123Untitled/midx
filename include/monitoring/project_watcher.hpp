#ifndef ml_monitoring_project_watcher_hpp
#define ml_monitoring_project_watcher_hpp

#include "monitoring/paths.hpp"
#include "monitoring/fs_watcher.hpp"
#include "monitoring/time_watcher.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* application */
	class application;

	/* monitor */
	class monitor;


	// -- P R O J E C T  W A T C H E R ----------------------------------------

	class project_watcher final {

		private:

			ml::paths _paths;

			ml::fs_watcher _target;
			ml::fs_watcher _parent;
			ml::time_watcher _timer;


		public:

			// -- public lifecycle --------------------------------------------

			project_watcher(const char*, ml::monitor&);


			// -- public methods ----------------------------------------------

			/* process events */
			auto process_events(ml::application&) -> void;

	}; // class file_watcher

} // namespace ml

#endif // ml_monitoring_project_watcher_hpp
