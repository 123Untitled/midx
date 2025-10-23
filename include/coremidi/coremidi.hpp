#ifndef midilang_midi_coremidi_hpp
#define midilang_midi_coremidi_hpp

#include <vector>
#include "coremidi/source.hpp"
#include "coremidi/destination.hpp"


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	/* sources */
	auto sources(void) -> std::vector<cm::source>;

	/* destinations */
	auto destinations(void) -> std::vector<cm::destination>;

} // namespace coremidi

#endif // midilang_coremidi_hpp
