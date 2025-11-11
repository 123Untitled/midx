#include "player.hpp"
#include "diagnostics/runtime_error.hpp"
#include "diagnostics/system_error.hpp"
#include "coremidi/coremidi.hpp"
#include "data/model.hpp"
#include "time/signature.hpp"
#include "system/concurrency/lock_guard.hpp"

#include <iostream>

// -- P L A Y E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::player::player(void)
: _mutex{},
  _model{},
  _clock{},
  _srcs{cm::sources()},
  _evs{},
  _thread{},
  _running{false} {

	if (_srcs.empty())
		throw ml::runtime_error("no midi sources");

	for (const auto& src : _srcs)
		std::cout << src.name() << std::endl;
}

/* destructor */
mx::player::~player(void) noexcept {
	self::stop();
}


// -- public methods ----------------------------------------------------------

/* start */
auto mx::player::start(void) -> void {

	// check if clock is already running
	if (_clock.is_running() == true)
		return;

	// launch thread
	if (::pthread_create(&_thread, nullptr, _entrypoint, this) != 0)
		throw ml::system_error("pthread_create");

	_running = true;
}

/* stop */
auto mx::player::stop(void) noexcept -> void {
	_clock.stop();

	if (_running == false) {
		std::cout << "clock is already stopped" << std::endl;
		return;
	}

	std::cout << "stopping player..." << std::endl;
	static_cast<void>(::pthread_join(_thread, nullptr));

	_running = false;
	std::cout << "player stopped" << std::endl;
}


// -- public overrides --------------------------------------------------------

/* start */
auto mx::player::clock_start(void) -> void {
	_evs.start();
	_evs.send(_srcs[0U]);
	_evs.clear();
	std::cout << "clock started" << std::endl;
}

auto mx::player::model(mx::model& m) noexcept -> void {
	const mx::lock_guard guard{_mutex};
	_model = &m;
}

/* tick */
auto mx::player::clock_tick(const ml::u64& count) -> void {

	// send tick
	_evs.tick();

	const ml::signature ppqn_24{1, 16};

	//std::cout << "tick: " << count << std::endl;

	if (_model && ppqn_24.is_time(count) == false)
		return;

	std::stringstream ss;
	ss << "{\"type\":\"animation\",\"highlights\":[";

	{
		const mx::lock_guard guard{_mutex};
		_model->play(ss, _evs, ppqn_24.count(count));
	}

	ss << "]}\r\n";
	auto str = ss.str();
	std::cout << str << std::endl;
	_server->broadcast(str);


	// send and clear events
	_evs.send(_srcs[0U]);
	_evs.clear();
}

/* stop */
auto mx::player::clock_stop(void) -> void {

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
auto mx::player::_entrypoint(void* arg) -> void* {
	auto& player = *static_cast<mx::player*>(arg);
	player._clock.start(player);
	return nullptr;
}
