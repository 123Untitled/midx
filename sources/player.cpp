#include "player.hpp"
#include "diagnostics/runtime_error.hpp"
#include "diagnostics/system_error.hpp"
#include "coremidi/coremidi.hpp"

#include <iostream>

// -- P L A Y E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::player::player(void)
: _clock{},
  _srcs{cm::sources()},
  _evs{},
  _thread{},
  _running{false} {

	if (_srcs.empty())
		throw ml::runtime_error("no midi sources");

	//for (const auto& src : _srcs)
	//	std::cout << src.name() << std::endl;
}

/* destructor */
ml::player::~player(void) noexcept {
	self::stop();
}


// -- public methods ----------------------------------------------------------

/* start */
auto ml::player::start(void) -> void {

	// check if clock is already running
	if (_clock.is_running() == true)
		return;

	// launch thread
	if (::pthread_create(&_thread, nullptr, _entrypoint, this) != 0)
		throw ml::system_error("pthread_create");

	_running = true;
}

/* stop */
auto ml::player::stop(void) noexcept -> void {
	_clock.stop();

	if (_running == false)
		return;

	static_cast<void>(::pthread_join(_thread, nullptr));

	_running = false;
}


// -- public overrides --------------------------------------------------------

/* start */
auto ml::player::clock_start(void) -> void {
	_evs.start();
	_evs.send(_srcs[0U]);
	_evs.clear();
	std::cout << "clock started" << std::endl;
}

/* tick */
auto ml::player::clock_tick(const ml::u64& count) -> void {

	// send tick
	_evs.tick();

	std::cout << "tick: " << count << std::endl;

	//_app->tree().play(_evs, count);

	//_events.pack(_ev);
	// send events
	_evs.send(_srcs[0U]);
	// clear events
	_evs.clear();
}

/* stop */
auto ml::player::clock_stop(void) -> void {

	std::cout << "clock stopped" << std::endl;
	// note off all notes
	for (ml::u8 ch = 0U; ch < 16U; ++ch) {
		for (ml::u8 note = 0U; note < 128U; ++note) {
			_evs.note_off(ch, note);
		}
	}

	_evs.stop();
	_evs.send(_srcs[0U]);
	_evs.clear();
}



// -- private static methods --------------------------------------------------

/* entrypoint */
auto ml::player::_entrypoint(void* arg) -> void* {
	auto& player = *static_cast<ml::player*>(arg);
	player._clock.start(player);
	return nullptr;
}
