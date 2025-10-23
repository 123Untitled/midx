#include "coremidi/coremidi.hpp"
#include "coremidi/types.hpp"

/* sources */
auto cm::sources(void) -> std::vector<cm::source> {

	// get number of sources
	const cm::item_count num_sources = ::MIDIGetNumberOfSources();

	// create sources
	std::vector<cm::source> sources;
	sources.reserve(num_sources);

	// iterate over sources
	for (cm::item_count i = 0U; i < num_sources; ++i) {

		// get source
		const cm::endpoint_reference source = ::MIDIGetSource(i);

		// create source
		sources.emplace_back(source);
	}

	// return sources
	return sources;
}

/* destinations */
auto cm::destinations(void) -> std::vector<cm::destination> {

	// get number of destinations
	const cm::item_count num_destinations = ::MIDIGetNumberOfDestinations();

	// create destinations
	std::vector<cm::destination> destinations;
	destinations.reserve(num_destinations);

	// iterate over destinations
	for (cm::item_count i = 0U; i < num_destinations; ++i) {

		// get destination
		const cm::endpoint_reference destination = ::MIDIGetDestination(i);

		// create destination
		destinations.emplace_back(destination);
	}

	// return destinations
	return destinations;
}
