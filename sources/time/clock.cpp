#include "time/clock.hpp"
#include "time/realtime.hpp"
#include "time/now.hpp"
#include "time/time.hpp"


// -- C L O C K ---------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::clock::clock(void) noexcept
: _running{false},
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

/* run */
auto ml::clock::start(ml::clockable& obj) -> void {

	// set realtime thread
	if (ml::realtime(_nano_clock))
		return;

	self::bpm(130U);

	self::_init_clock();


	// start notification
	obj.clock_start();

	// loop while running
	while (_running == true) {

		//self::_begin();
		_start = ml::now();
		//self::_compute_timeline();
		// accumulate elapsed time
		_timeline += static_cast<double>(_elapsed)
				   / static_cast<double>(ml::nano_per_sec);

		// tick notification
		obj.clock_tick(_count);

		self::_compute_diff();
		self::_sleep();

		++_count;
	}

	// stop notification
	obj.clock_stop();
}

/* stop */
auto ml::clock::stop(void) noexcept -> void {
	_running = false;
}


// -- public modifiers --------------------------------------------------------

/* bpm */
auto ml::clock::bpm(const unsigned& bpm) noexcept -> void {
	_bpm         = bpm;
	_nano_clock  = _bpm.to_nano();
	_nano_target = _nano_clock;
}


// -- public accessors --------------------------------------------------------

/* bpm */
auto ml::clock::bpm(void) const noexcept -> const ml::bpm& {
	return _bpm;
}

/* count */
auto ml::clock::count(void) const noexcept -> ml::u64 {
	return _count;
}

/* timeline */
auto ml::clock::timeline(void) const noexcept -> double {
	return _timeline;
}

/* is running */
auto ml::clock::is_running(void) const noexcept -> bool {
	return _running;
}



// -- private methods ---------------------------------------------------------

/* init clock */
auto ml::clock::_init_clock(void) noexcept -> void {

	_running    = true;

	_count      = 0;

	_start      = 0;
	_last_start = ml::now();
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
auto ml::clock::_begin(void) noexcept -> void {
	_start = ml::now();
}

/* compute timeline */
auto ml::clock::_compute_timeline(void) noexcept -> void {

	// accumulate elapsed time
	_timeline += static_cast<double>(_elapsed)
			   / static_cast<double>(ml::nano_per_sec);
}

/* compute diff */
auto ml::clock::_compute_diff(void) noexcept -> void {

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
auto ml::clock::_sleep(void) noexcept -> void {

	// delay sleep
	::nanosleep(&_request, nullptr);
	_end = ml::now();

	// busy wait
	while ((_end - _start) < _nano_target)
		_end = ml::now();
}
