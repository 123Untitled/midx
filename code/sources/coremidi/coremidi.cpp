#include "coremidi/coremidi.hpp"
#include "coremidi/types.hpp"

#if defined(midx_macos)


namespace cm {

	auto get_number_of_sources(void) -> cm::item_count {
		const cm::item_count num = ::MIDIGetNumberOfSources();
		if (num == 0U)
			throw cm::exception(0, "no MIDI sources available");
		return num;
	}

	auto get_number_of_destinations(void) -> cm::item_count {
		const cm::item_count num = ::MIDIGetNumberOfDestinations();
		if (num == 0U)
			throw cm::exception(0, "no MIDI destinations available");
		return num;
	}

} // namespace cm



/* sources */
auto cm::sources(void) -> std::vector<cm::source> {

	// get number of sources
	const auto num = cm::get_number_of_sources();

	// create sources
	std::vector<cm::source> sources;
	sources.reserve(num);

	// iterate over sources
	for (cm::item_count i = 0U; i < num; ++i) {

		// get source
		const cm::endpoint src = ::MIDIGetSource(i);

		// create source
		sources.emplace_back(src);

		cm::object o{src};
		std::cout << "src: " << o.name() << "\r\n";
		std::cout << "src: " << o.manufacturer() << "\r\n";
		std::cout << "src: " << o.model() << "\r\n";
	}

	// return sources
	return sources;
}

/* destinations */
auto cm::destinations(void) -> std::vector<cm::destination> {

	// get number of destinations
	const cm::item_count num = cm::get_number_of_destinations();

	// create destinations
	std::vector<cm::destination> destinations;
	destinations.reserve(num);

	// iterate over destinations
	for (cm::item_count i = 0U; i < num; ++i) {

		// get destination
		const cm::endpoint dst = ::MIDIGetDestination(i);

		// create destination
		destinations.emplace_back(dst);

		cm::object o{dst};
		std::cout << "dest: " << o.name() << "\r\n";
		std::cout << "dest: " << o.manufacturer() << "\r\n";
		std::cout << "dest: " << o.model() << "\r\n";
	}

	// return destinations
	return destinations;
}

#endif // midx_macos
