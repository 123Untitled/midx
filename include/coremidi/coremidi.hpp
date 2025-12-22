#ifndef coremidi_hpp
#define coremidi_hpp

#include "os.hpp"

#if defined(midx_macos)

#include <vector>
#include "coremidi/source.hpp"
#include "coremidi/destination.hpp"


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {

	//const cm::os_status status = MIDIRestart();
	//if (status != noErr) {
	//	throw cm::exception{status, "MIDIRestart"};
	//}


	/* sources */
	auto sources(void) -> std::vector<cm::source>;

	/* destinations */
	auto destinations(void) -> std::vector<cm::destination>;

} // namespace coremidi

#endif // midx_macos

#endif // coremidi_hpp
