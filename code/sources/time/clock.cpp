#include "time/clock.hpp"


// -- C L O C K ---------------------------------------------------------------

// -- private static methods --------------------------------------------------

/* entrypoint */
auto mx::clock::_entrypoint(void* ptr) noexcept -> void* {
	static_cast<self*>(ptr)->_run();
	return nullptr;
}

// -- private methods ---------------------------------------------------------

/* run */
auto mx::clock::_run(void) noexcept -> void {

	if (_realtime.apply() == false) {
		std::cerr << "Failed to set realtime policy\r\n";
		_running.store(false, std::memory_order_release);
		return;
	}

	// latency in ticks (30 ms)
	const mx::u64 latency = mx::timebase::ms_to_ticks<mx::u64>(30U);

	_next = mx::frac{mx::host_time::now()
		  + mx::host_time{latency}};

	while (is_running()) {


		const auto now   = mx::host_time::now();
		const auto futur = now + mx::host_time{latency};

		const mx::frac deadline{futur};

		if (_next <= deadline) {

			mx::usz count = 0U;

			do {

				// timestamp
				const mx::u64 ts = _next.value<mx::u64>();

				if (_queue.push(ts/* + latency*/) == false)
					break; // queue full

				++count;

				_next += _host_per_tick;
				//std::cout << _next << "\r\n";

			} while (_next <= deadline);

			// notify via pipe
			static_cast<void>(::write(_pipe.wr(), &count, sizeof(count)));
		}

		// sleep less than next window
		// not well implemented yet...

		struct ::timespec  request;

		//mx::host_time sl{_host_per_tick.value<mx::u64>()};
		//auto sl_ns = sl.to_ns();
		//sl_ns /= 2U; // sleep half time

		// sleep 20 ms
		const mx::u64 sl_ns = 20U * 1'000'000U;

		request.tv_sec  = 0;
		request.tv_nsec = static_cast<long>(sl_ns);
		//std::cout << "sleep until: " << request.tv_nsec << " ns\r\n";


		static_cast<void>(::nanosleep(&request, nullptr));
	}
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::clock::clock(void)
: _running{false},

  _bpm{_compute_bpm_to_frac(130.0)},
  _host_per_tick{_compute_host_per_tick(_bpm)},

  _next{},

  _queue{},
  _pipe{mx::pipe()} {


	_pipe.non_blocking_wr();


	// half host per tick
	const auto hpt = _host_per_tick / 2U;

	_realtime.period(hpt.value<mx::u32>());
}

/* destructor */
mx::clock::~clock(void) noexcept {
	stop();
}


// -- public accessors --------------------------------------------------------

/* is running */
auto mx::clock::is_running(void) const noexcept -> bool {
	return _running.load(std::memory_order_acquire);
}
