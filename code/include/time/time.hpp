#ifndef midilang_time_time_hpp
#define midilang_time_time_hpp

#include "core/types.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* ms per minute */
	static constexpr mx::u64 ms_per_min = 60'000;

	/* nano per second */
	static constexpr mx::u64 nano_per_sec = 1'000'000'000;

	/* nano per millisecond */
	static constexpr mx::u64 nano_per_ms = 1'000'000;

	/* nano per minute */
	static constexpr mx::u64 nano_per_min = 60'000'000'000;

} // namespace mx

#endif // midilang_time_time_hpp
