#ifndef core_endian_hpp
#define core_endian_hpp

#include "core/types.hpp"
#include "core/platform.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- E N D I A N ---------------------------------------------------------

	enum class endian : mx::uint {
		little = 0xFEED,
		big    = 0xBEEF,
		#if   defined(midx_endian_little)
		native = little
		#elif defined(midx_endian_big)
		native = big
		#else
		native = 0xDEAD
		#endif
	}; // enum endian

} // namespace mx

#endif // core_endian_hpp
