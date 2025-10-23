#ifndef midilang_time_time_hpp
#define midilang_time_time_hpp

#include "types.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	/* ms per minute */
	static constexpr ml::u64 ms_per_min = 60'000;

	/* nano per second */
	static constexpr ml::u64 nano_per_sec = 1'000'000'000;

	/* nano per millisecond */
	static constexpr ml::u64 nano_per_ms = 1'000'000;

	/* nano per minute */
	static constexpr ml::u64 nano_per_min = 60'000'000'000;

} // namespace ml

#endif // midilang_time_time_hpp
