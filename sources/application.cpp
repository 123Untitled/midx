#include "application.hpp"
#include "core/utility/type_operations.hpp"


// -- A P P L I C A T I O N ---------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* constructor */
mx::application::application(void)
: _running{true},
  _monitor{},
  _input{_monitor},
  _signal{_monitor},
  _server{_monitor},
  _protocol1{},
  _protocol_parser{},
  _analyzers{},
  _active{0U},
  _player{_monitor} {
}


// -- private methods ---------------------------------------------------------

auto mx::application::reparse(mx::string&& data) -> void {

	auto& analyzer = _analyzers[_active];

	analyzer.analyze(mx::move(data));
	_server.broadcast(analyzer.highlights());

	if (analyzer.has_errors() == true)
		return;


	// switch tree
	_player.switch_tree(analyzer.tree(), analyzer.tokens());
	_active = 1U - _active;
}

/* exit */
auto mx::application::exit(void) noexcept -> void {
	_running = false;
}

/* play */
auto mx::application::play(void) -> void {
	_player.play();
}

/* stop */
auto mx::application::stop(void) -> void {
	_player.stop();
}

/* toggle play */
auto mx::application::toggle(void) -> void {
	_player.toggle();

	// broadcast playing state to client
	const bool playing = _player.is_playing();

	mx::string msg = mx::string_pool::query();

	msg.append("{\"type\":\"state\",\"playing\":",
			   (playing ? "true" : "false"),
			   "}\r\n"
	);
	_server.broadcast(std::move(msg));
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
