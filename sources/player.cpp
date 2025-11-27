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
  _engine{} {

	// add user event to monitor
	monitor.add_user(*this);
}


// -- public methods ----------------------------------------------------------

/* start */
auto mx::player::start(void) -> void {

	if (_clock.is_running() == true)
		return;

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
	_tree = &tree;
}


// -- public overrides --------------------------------------------------------

/* on event */
auto mx::player::on_event(mx::application& app, const struct ::kevent& ev) -> void {

	static mx::u64 tick_count = 0U;

	if (_tree == nullptr) {
		++tick_count;
		return;
	}

	// make fractional time
	const auto time = mx::make_reduced_frac(tick_count, MIDI_PPQN);

	std::stringstream ss;
	ss << "{\"type\":\"animation\",\"highlights\":[";

	_engine.off_pass();
	_tree->play(ss, _engine, time);
	_engine.flush();

	auto str = ss.str();

	if (str.empty() == false) {

		if (str.back() == ',')
			str.pop_back();
		str += "]}\r\n";

		app.server().broadcast(str);
	}
	else {
		std::cout << "EMPTY PLAYLOAD\n";
	}
	++tick_count;
}

/* ident */
auto mx::player::ident(void) const noexcept -> int {
	// random ident
	return 42;
}
