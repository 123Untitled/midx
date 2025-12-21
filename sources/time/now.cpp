#include "time/now.hpp"
#include "time/timebase.hpp"

#include <mach/mach_time.h>


/* now */
auto mx::now(void) noexcept -> mx::u64 {
	return static_cast<mx::u64>(::mach_absolute_time()
							* mx::timebase::ticks_to_ns());
}
