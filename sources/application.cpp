#include "application.hpp"
#include "system/mapped_file.hpp"


// -- A P P L I C A T I O N ---------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* path constructor */
ml::application::application(const char* path)
: _running{true},
  //_project{},
  _player{},
  _monitor{},
  _signal{_monitor},
  //_watcher{path, _monitor},
  _server{_monitor},
  _protocol{},
  _analyzer{} {
}


// -- private methods ---------------------------------------------------------

auto ml::application::reparse(void) -> void {
	std::cout << "\x1b[2J\x1b[H";
	_analyzer.analyze(_protocol.data());
	_server.broadcast(_analyzer.highlights());
}

/* run */
auto ml::application::_run(void) -> void {

	//_player.start();

	while (_running == true) {
		_monitor.wait(*this);

		if (_protocol.require_update()) {
			reparse();
		}
		//if (_watcher.has_changes(_monitor)) {
		//	reparse();
		//}
	}

	//_player.stop();
}


// -- public static methods ---------------------------------------------------

/* run */
auto ml::application::run(const char* path) -> void {
	ml::application{path}._run();
}
