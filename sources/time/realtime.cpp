#include "time/realtime.hpp"
#include "time/timebase.hpp"

#include <mach/mach.h>


// -- R E A L T I M E ---------------------------------------------------------


// -- private static methods --------------------------------------------------

/* minimum rt computation */
auto mx::realtime::min_rt_quantum(void) -> mx::u32 {
	// 50 us
	return static_cast<mx::u32>(mx::timebase::us_to_ticks(50U));
}

/* maximum rt computation */
auto mx::realtime::max_rt_quantum(void) -> mx::u32 {
	// 50 ms
	return static_cast<mx::u32>(mx::timebase::ms_to_ticks(50U));
}


// -- public methods ----------------------------------------------------------

/* apply */
auto mx::realtime::apply(void) noexcept -> bool {

	// policy data
	::thread_time_constraint_policy_data_t policy {

		/* period: This is the nominal amount of time between separate
		 * processing arrivals, specified in absolute time units.  A
		 * value of 0 indicates that there is no inherent periodicity in
		 * the computation. */
		.period = _period,

		/* computation: This is the nominal amount of computation
		 * time needed during a separate processing arrival, specified
		 * in absolute time units.  The thread may be preempted after
		 * the computation time has elapsed.
		 * If (computation < constraint/2) it will be forced to
		 * constraint/2 to avoid unintended preemption and associated
		 * timer interrupts. */
		.computation = _computation,

		/* constraint: This is the maximum amount of real time that
		 * may elapse from the start of a separate processing arrival
		 * to the end of computation for logically correct functioning,
		 * specified in absolute time units.  Must be (>= computation).
		 * Note that latency = (constraint - computation). */
		.constraint = _constraint,

		/* preemptible: IGNORED (This indicates that the computation may be
		 * interrupted, subject to the constraint specified above.) */
		.preemptible = true
	};


	// get current thread
	const auto self_thread = ::mach_thread_self();

	// set thread policy
	const auto ret = ::thread_policy_set(self_thread,
							   // real-time behavior
							   THREAD_TIME_CONSTRAINT_POLICY,
							   // policy data
							   reinterpret_cast<::thread_policy_t>(&policy),
							   // policy count
							   THREAD_TIME_CONSTRAINT_POLICY_COUNT);


	return ret == KERN_SUCCESS;
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::realtime::realtime(void) noexcept
: _min_rt_quantum{min_rt_quantum()},
  _max_rt_quantum{max_rt_quantum()},
  _period{0U},
  _computation{_min_rt_quantum},
  _constraint{_min_rt_quantum * 2U} {
}
