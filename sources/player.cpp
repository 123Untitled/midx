#include "player.hpp"
#include "language/ast/tree.hpp"

#include "monitoring/monitor.hpp"
#include "monitoring/server.hpp"
#include "application.hpp"


#include <sys/event.h>


// -- P L A Y E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::player::player(const mx::monitor& monitor)
: mx::watcher{},
  _clock{},
  //_clock{monitor.kqueue(), *this},
  _eval{},
  _midi{},
  _hls{},
  _ticks{0U} {

	// add user event to monitor
	//monitor.add_user(*this);
	monitor.add_read(*this);
}


// -- public methods ----------------------------------------------------------

/* start */
auto mx::player::play(void) -> void {

	if (_clock.is_running() == true)
		return;

	// reset ticks
	_ticks = 0U;

	// start clock
	_clock.start();

	// send start clock midi event
	_midi.start();
}

/* stop */
auto mx::player::stop(void) -> void {

	if (_clock.is_running() == false)
		return;

	// stop clock
	_clock.stop();

	// send stop clock midi event
	_midi.stop();

	// reset highlights
	_hls.clear();
}

/* toggle */
auto mx::player::toggle(void) -> void {
	is_playing() == true ? stop() : play();
}

/* switch tree */
auto mx::player::switch_tree(const as::tree& tree, const tk::tokens& tks) noexcept -> void {
	 _hls.init(tks);
	_eval.init(tree, _hls);
}

/* is playing */
auto mx::player::is_playing(void) const noexcept -> bool {
	return _clock.is_running();
}


auto mx::player::_evaluate(mx::application& app, const mx::u64 timestamp) -> void {

	// make fractional time
	const auto time = mx::make_reduced_frac(_ticks, MIDI_PPQN);

	_hls.swap_now();

	_midi.off_pass(timestamp);

	const auto expr = _eval.evaluate(time);


	// fill midi events
	expr.for_each(
		[](const mx::midi_event& ev, mx::midi& m, const mx::u64 ts) static -> void {
			m.note_on(ev, ts);
		}, _midi, timestamp
	);

	// flush midi events
	_midi.flush();


	// generate json output
	if (_hls.has_changes() == true) {
		auto json = _hls.generate_json();
		app.server().broadcast(std::move(json));
	}

	++_ticks;
}

// -- public overrides --------------------------------------------------------

/* on event */
auto mx::player::on_event(mx::application& app, const struct ::kevent& ev) -> void {

	_clock.consume();

	if (_eval.is_evaluable() == false)
		return;

	auto& queue = _clock.queue();

	mx::uint i = 0U;
	for (mx::u64 ts = 0U; queue.pop(ts) == true;) {
		self::_evaluate(app, ts);
		++i;
	}
	std::cout << "Processed " << i << " tick(s)\r\n";

}

/* ident */
auto mx::player::ident(void) const noexcept -> int {
	// random ident
	//return 42;
	return _clock.ident();
}
