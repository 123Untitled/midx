#include "application.hpp"


// -- A P P L I C A T I O N ---------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* constructor */
mx::application::application(void)
: _running{true},
  _monitor{},
  _signal{_monitor},
  _server{_monitor},
  _protocol{},
  _analyzers{},
  _active{0U},
  _player{_monitor} {
}


// -- private methods ---------------------------------------------------------

auto mx::application::reparse(std::string&& data) -> void {
	// clear screen
	::write(STDOUT_FILENO, "\x1b[H\x1b[0J", 7);

	auto& analyzer = _analyzers[_active];

	analyzer.analyze(std::move(data));
	_server.broadcast(analyzer.highlights());

	if (analyzer.has_errors() == true)
		return;

	//return;

	// switch tree
	_player.switch_tree(analyzer.tree());
	_active = (_active == 0U) ? 1U : 0U;
}

/* exit */
auto mx::application::exit(void) noexcept -> void {
	_running = false;
}

/* toggle play */
auto mx::application::toggle_play(void) noexcept -> void {
	if (_player.is_playing() == true) {
		_player.stop();
	}
	else {
		_player.start();
	}
}

/* run */
auto mx::application::_run(void) -> void {

	while (_running == true) {
		_monitor.wait(*this);
	}

	_player.stop();
}


// -- public static methods ---------------------------------------------------

/* run */
auto mx::application::run(void) -> void {
	mx::application{}._run();
}
