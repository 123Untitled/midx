#include "time/timebase.hpp"
#include "diagnostics/system_error.hpp"

#include <mach/mach_time.h>


// -- T I M E B A S E ---------------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* default constructor */
mx::timebase::timebase(void) {

	// timebase info
	::mach_timebase_info_data_t timebase;

	// get timebase info
	if (::mach_timebase_info(&timebase) != KERN_SUCCESS)
		throw mx::system_error{"mach_timebase_info"}; // maybe errno not set

	// nano per millisecond
	constexpr unsigned nano_per_ms = 1'000'000;

	// absolute clock to nanoseconds
	_absolute_to_nano = static_cast<double>(timebase.numer)
					  / static_cast<double>(timebase.denom);

	// milliseconds to absolute clock
	_ms_to_absolute = (static_cast<double>(timebase.denom)
					 / static_cast<double>(timebase.numer)) * nano_per_ms;

	// nanoseconds to absolute clock
	_nano_to_absolute = static_cast<double>(timebase.denom)
					  / static_cast<double>(timebase.numer);
}


// -- private static methods --------------------------------------------------

/* shared */
auto mx::timebase::_shared(void) -> self& {
	static self instance;
	return instance;
}


// -- public accessors --------------------------------------------------------

/* ms to absolute */
auto mx::timebase::ms_to_absolute(void) noexcept -> double {
	return self::_shared()._ms_to_absolute;
}

/* absolute to nano */
auto mx::timebase::absolute_to_nano(void) noexcept -> double {
	return self::_shared()._absolute_to_nano;
}

/* nano to absolute */
auto mx::timebase::nano_to_absolute(void) noexcept -> double {
	return self::_shared()._nano_to_absolute;
}
