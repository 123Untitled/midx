#include "time/timebase.hpp"
#include "diagnostics/system_error.hpp"


// -- T I M E B A S E ---------------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* default constructor */
mx::timebase::timebase(void)
/* uninitialized */ {

	// get timebase info
	if (::mach_timebase_info(&_timebase) != KERN_SUCCESS)
		throw mx::system_error{"mach_timebase_info"}; // maybe errno not set

	// nano per millisecond
	//constexpr mx::u64 ns_per_ms = 1'000'000;

	// ticks to nanoseconds
	_ticks_to_ns = static_cast<double>(_timebase.numer)
				 / static_cast<double>(_timebase.denom);

	// ticks to milliseconds
	_ticks_to_ms = _ticks_to_ns / static_cast<double>(ns_per_ms);

	/* ticks to microseconds */
	_ticks_to_us = _ticks_to_ns / static_cast<double>(ns_per_us);


	// nanoseconds to ticks
	_ns_to_ticks = static_cast<double>(_timebase.denom)
				 / static_cast<double>(_timebase.numer);

	// milliseconds to ticks
	_ms_to_ticks = _ns_to_ticks * static_cast<double>(ns_per_ms);


	// microseconds to ticks
	_us_to_ticks = _ns_to_ticks * static_cast<double>(ns_per_us);
}


// -- private static methods --------------------------------------------------

/* shared */
auto mx::timebase::_shared(void) -> const self& {
	static self instance;
	return instance;
}


// -- public accessors --------------------------------------------------------

/* num */
auto mx::timebase::num(void) -> mx::u64 {
	return static_cast<mx::u64>(self::_shared()._timebase.numer);
}

/* den */
auto mx::timebase::den(void) -> mx::u64 {
	return static_cast<mx::u64>(self::_shared()._timebase.denom);
}

/* ticks to nanoseconds */
auto mx::timebase::ticks_to_ns(void) -> double {
	return self::_shared()._ticks_to_ns;
}

/* ticks to milliseconds */
auto mx::timebase::ticks_to_ms(void) -> double {
	return self::_shared()._ticks_to_ms;
}

/* ticks to microseconds */
auto mx::timebase::ticks_to_us(void) -> double {
	return self::_shared()._ticks_to_us;
}

/* nanoseconds to ticks */
auto mx::timebase::ns_to_ticks(void) -> double {
	return self::_shared()._ns_to_ticks;
}

/* milliseconds to ticks */
auto mx::timebase::ms_to_ticks(void) -> double {
	return self::_shared()._ms_to_ticks;
}
