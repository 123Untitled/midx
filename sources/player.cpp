#include "player.hpp"
#include "language/ast/tree.hpp"
#include "time/signature.hpp"

#include "monitoring/monitor.hpp"
#include "monitoring/server.hpp"
#include "application.hpp"


#include <iostream>
#include <sstream>

#include <sys/event.h>

// -- P L A Y E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::player::player(const mx::monitor& monitor)
: mx::watcher{},
  _clock{monitor.kqueue(), *this},
  _tree{nullptr},
  _engine{},
  _ticks{0U} {

	// add user event to monitor
	monitor.add_user(*this);
}


// -- public methods ----------------------------------------------------------

/* start */
auto mx::player::start(void) -> void {

	if (_clock.is_running() == true)
		return;

	// reset ticks
	_ticks = 0U;

	// start clock
	_clock.start();

	// send start clock midi event
	_engine.start();
}

/* stop */
auto mx::player::stop(void) -> void {

	if (_clock.is_running() == false)
		return;

	// stop clock
	_clock.stop();

	// send stop clock midi event
	_engine.stop();
}

/* switch tree */
auto mx::player::switch_tree(as::tree& tree) noexcept -> void {
	_hl_tracker.init(*tree.tokens);
	_eval.init(tree, *tree.tokens, _hl_tracker);
	_tree = &tree;
}

/* is playing */
auto mx::player::is_playing(void) const noexcept -> bool {
	return _clock.is_running();
}


// -- public overrides --------------------------------------------------------

/* on event */
auto mx::player::on_event(mx::application& app, const struct ::kevent& ev) -> void {

	if (_eval.is_evaluable() == false) {
		return;
	}

	// make fractional time
	const auto time = mx::make_reduced_frac(_ticks, MIDI_PPQN);

	static std::string s;

	_hl_tracker.begin_frame();
	_engine.off_pass();
	_eval.evaluate(s, _engine, time);
	_engine.flush();

	// only send if highlights changed
	if (_hl_tracker.end_frame(s)) {
		s.insert(0, "{\"type\":\"animation\",");
		s.append("}\r\n");
		app.server().broadcast(s);
	}

	s.clear();
	++_ticks;
}

/* ident */
auto mx::player::ident(void) const noexcept -> int {
	// random ident
	return 42;
}
