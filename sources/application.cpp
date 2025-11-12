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

	auto& analyzer = _analyzers[_active];

	analyzer.analyze(std::move(data));
	_server.broadcast(analyzer.highlights());

	if (analyzer.has_errors() == true)
		return;

	// switch model
	_player.switch_model(analyzer.model());
	_active = (_active == 0U) ? 1U : 0U;

	_player.start();
}

/* exit */
auto mx::application::exit(void) noexcept -> void {
	_running = false;
}

/* run */
auto mx::application::_run(void) -> void {

	while (_running == true)
		_monitor.wait(*this);

	_player.stop();
}


// -- public static methods ---------------------------------------------------

/* run */
auto mx::application::run(void) -> void {
	mx::application{}._run();
}
