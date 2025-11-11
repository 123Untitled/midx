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
  _analyzers{},
  _active{0U} {

	_player.server(_server);
}


// -- private methods ---------------------------------------------------------

auto ml::application::reparse(void) -> void {

	auto& analyzer = _analyzers[_active];

	analyzer.analyze(_protocol.data());
	_server.broadcast(analyzer.highlights());
	std::cout << "\x1b[31mANALYZER: " << _active << "\x1b[0m\n";

	if (analyzer.has_errors() == false) {


		_player.model(analyzer.model());
		_active = (_active + 1U) % 2U;

		if (_player.is_playing() == false)
			_player.start();
	}
}

/* run */
auto ml::application::_run(void) -> void {

	//_player.start();

	while (_running == true) {
		_monitor.wait(*this);

		if (_protocol.require_update()) {
			reparse();
			_protocol.reset();
		}
		//if (_watcher.has_changes(_monitor)) {
		//	reparse();
		//}
	}

	_player.stop();
}


// -- public static methods ---------------------------------------------------

/* run */
auto ml::application::run(const char* path) -> void {
	ml::application{path}._run();
}
