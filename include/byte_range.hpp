#ifndef byte_range_hpp
#define byte_range_hpp

#include "types.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- B Y T E  R A N G E --------------------------------------------------

	class byte_range final {
 

		public:

			// -- public members ----------------------------------------------

			const ml::u8* begin;

			const ml::u8* end;

	}; // class byte_range

} // namespace ml

#endif // byte_range_hpp
