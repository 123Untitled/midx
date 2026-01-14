#ifndef midi_event_hpp
#define midi_event_hpp

#include "core/types.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M I D I  E V E N T --------------------------------------------------

	struct midi_event final {
		mx::u8   channel;
		mx::u8   note;
		mx::u8   velocity;
		mx::u32  gate;
	};

} // namespace mx

#endif // midi_event_hpp
