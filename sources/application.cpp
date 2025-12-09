#include "application.hpp"
#include "core/utility/type_operations.hpp"


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

auto mx::application::reparse(mx::string&& data) -> void {

	auto& analyzer = _analyzers[_active];

	analyzer.analyze(mx::move(data));
	_server.broadcast(analyzer.highlights());

	if (analyzer.has_errors() == true) {
		std::cout << "\x1b[31mErrors detected, playback aborted.\x1b[0m" << std::endl;
		return;
	}

	// switch tree
	_player.switch_tree(analyzer.tree(), analyzer.tokens());
	_active = (_active == 0U) ? 1U : 0U;
}

/* exit */
auto mx::application::exit(void) noexcept -> void {
	_running = false;
}

/* toggle play */
auto mx::application::toggle(void) -> void {
	_player.toggle();
}

/* run */
auto mx::application::_run(void) -> void {

	const cm::os_status status = MIDIRestart();
	if (status != noErr) {
		throw cm::exception{status, "MIDIRestart"};
	}
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
