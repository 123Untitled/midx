#include "application.hpp"


// -- A P P L I C A T I O N ---------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* path constructor */
ml::application::application(const char* path)
: _running{true},
  _project{},
  _player{},
  _monitor{},
  _watcher{path, _monitor} {
}


// -- private methods ---------------------------------------------------------

/* run */
auto ml::application::_run(void) -> void {

	//_player.start();

	while (_running == true) {
		_monitor.wait(*this);
		_watcher.process_events(*this);
	}

	//_player.stop();
}


// -- public static methods ---------------------------------------------------

/* run */
auto ml::application::run(const char* path) -> void {
	ml::application{path}._run();
}
