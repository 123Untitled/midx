#include "time/clock.hpp"
#include "time/realtime.hpp"
#include "time/now.hpp"
#include "time/time.hpp"
#include "diagnostics/system_error.hpp"

#include <iostream>
#include <sys/event.h>


// -- C L O C K ---------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::clock::clock(const int efd, mx::watcher& w) noexcept
: _thread{},
  _efd{efd},
  _watcher{&w},
  _running{false},
  _bpm{0U},
  _nano_clock{0},
  _nano_target{0},
  _count{0U},
  _timeline{0.0},
  _request{},
  _start{0},
  _last_start{0},
  _end{0},
  _elapsed{0},
  _acc_ref{0},
  _acc_stamp{0},
  _acc_diff{0},
  _loop_diff{0} {
}




// -- public methods ----------------------------------------------------------

/* start */
auto mx::clock::start(void) -> void {

	if (_running == true)
		return;

	// create thread
	if (::pthread_create(&_thread, nullptr, _entrypoint, this) != 0)
		throw mx::system_error{"pthread_create"};

	_running = true;
}

/* stop */
auto mx::clock::stop(void) noexcept -> void {

	if (_running == false)
		return;

	_running = false;

	static_cast<void>(::pthread_join(_thread, nullptr));
}

/* bpm */
auto mx::clock::bpm(const unsigned& bpm) noexcept -> void {
	_bpm         = bpm;
	_nano_clock  = _bpm.to_nano();
	_nano_target = _nano_clock;
}


// -- public accessors --------------------------------------------------------

/* bpm */
auto mx::clock::bpm(void) const noexcept -> const mx::bpm& {
	return _bpm;
}

/* count */
auto mx::clock::count(void) const noexcept -> mx::u64 {
	return _count;
}

/* timeline */
auto mx::clock::timeline(void) const noexcept -> double {
	return _timeline;
}

/* is running */
auto mx::clock::is_running(void) const noexcept -> bool {
	return _running;
}



// -- private static methods --------------------------------------------------

/* entrypoint */
auto mx::clock::_entrypoint(void* arg) noexcept -> void* {
	auto& clock = *static_cast<mx::clock*>(arg);
	clock._loop();
	return nullptr;
}


auto mx::clock::_loop(void) noexcept -> void {

	// set realtime thread
	if (mx::realtime(_nano_clock))
		return;

	self::bpm(125U);
	self::_init_clock();

	// tick notification
	const struct ::kevent ev {
		.ident  = 42U,
		.filter = EVFILT_USER,
		.flags  = 0U,
		.fflags = NOTE_TRIGGER,
		.data   = 0,
		.udata  = _watcher
	};

	// loop while running
	while (_running == true) {

		// print message at modulo 24 ticks
		//if ((_count % 24U) == 0U) {
		//	std::cout << "clock tick: " << _count << std::endl;
		//}

		//self::_begin();
		_start = mx::now();
		//self::_compute_timeline();
		// accumulate elapsed time
		_timeline += static_cast<double>(_elapsed)
				   / static_cast<double>(mx::nano_per_sec);

		const auto ret = ::kevent(_efd, &ev, 1, nullptr, 0, nullptr);
		if (ret == -1) {
			perror("kevent clock tick");
			return;
		}

		self::_compute_diff();
		self::_sleep();

		++_count;
	}
}


// -- private methods ---------------------------------------------------------

/* init clock */
auto mx::clock::_init_clock(void) noexcept -> void {

	_running    = true;

	_count      = 0;

	_start      = 0;
	_last_start = mx::now();
	_end        = _last_start + _nano_clock;

	_elapsed    = 0;
	_acc_ref    = 0;
	_acc_stamp  = 0;
	_acc_diff   = 0;
	_loop_diff  = 0;
	_timeline   = 0.0;

	// secondes number
	_request.tv_sec = 0;
	// nanosecondes number
	_request.tv_nsec = static_cast<long>(_nano_clock - (_nano_clock / 100));
}

/* begin */
auto mx::clock::_begin(void) noexcept -> void {
	_start = mx::now();
}

/* compute timeline */
auto mx::clock::_compute_timeline(void) noexcept -> void {

	// accumulate elapsed time
	_timeline += static_cast<double>(_elapsed)
			   / static_cast<double>(mx::nano_per_sec);
}

/* compute diff */
auto mx::clock::_compute_diff(void) noexcept -> void {

	_elapsed = _end - _last_start;

	//loop_diff = _elapsed - nanoClock;


	_acc_stamp += _elapsed;
	_acc_ref   += _nano_clock;

	if (_acc_stamp > _acc_ref) {

		_acc_diff   = _acc_stamp - _acc_ref;
		_nano_target = _nano_clock - _acc_diff;
	}

	else if (_acc_stamp < _acc_ref) {

		_acc_diff   = _acc_ref   - _acc_stamp;
		_nano_target = _nano_clock + _acc_diff;
	}

	// else if (_acc_ref == _acc_stamp)
	else {

		/*temp code*/ _acc_diff = 0;
		_nano_target = _nano_clock;
		_acc_stamp	= 0;
		_acc_ref	= 0;
	}
	//std::cout << "now: " << now() << std::endl;
	//std::cout << "timestamp: " << mach_absolute_time() << std::endl;

	//std::cout << _acc_diff << std::endl;

	_request.tv_nsec = static_cast<long>(_nano_target
					- (_nano_target / 100));

	_last_start = _start;
}

/* sleep */
auto mx::clock::_sleep(void) noexcept -> void {

	// delay sleep
	::nanosleep(&_request, nullptr);
	_end = mx::now();

	// busy wait
	while ((_end - _start) < _nano_target)
		_end = mx::now();
}
