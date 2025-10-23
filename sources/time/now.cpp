#include "time/now.hpp"
#include "time/timebase.hpp"

#include <mach/mach_time.h>


/* now */
auto ml::now(void) noexcept -> ml::u64 {
	return ::mach_absolute_time()
		* ml::timebase::absolute_to_nano();
}
