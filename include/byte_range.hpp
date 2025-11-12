#ifndef byte_range_hpp
#define byte_range_hpp

#include "types.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- B Y T E  R A N G E --------------------------------------------------

	class byte_range final {
 

		public:

			// -- public members ----------------------------------------------

			const mx::u8* begin;

			const mx::u8* end;

	}; // class byte_range

} // namespace mx

#endif // byte_range_hpp
