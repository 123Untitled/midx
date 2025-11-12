#include "time/now.hpp"
#include "time/timebase.hpp"

#include <mach/mach_time.h>


/* now */
auto mx::now(void) noexcept -> mx::u64 {
	return ::mach_absolute_time()
		* mx::timebase::absolute_to_nano();
}
