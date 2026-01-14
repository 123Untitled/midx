#include "coremidi/destination.hpp"
#include "coremidi/string.hpp"
//#include "coremidi/unique_id.hpp"

#if defined(midx_macos)

#include <iostream>

// -- D E S T I N A T I O N ---------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* id constructor */
cm::destination::destination(const ::MIDIEndpointRef& ref)
: cm::object{ref},
  _external{true} {
}


template <typename T>
static auto _dispatch_events(const ::MIDIEventList&, T&) -> void;


/* name constructor */
cm::destination::destination(const cm::client& client, const char* name, mx::midi_watcher& w)
: _external{false} {

	// create cfstring
	cm::string cstr{name};

	// create a new destination
	const ::OSStatus err = ::MIDIDestinationCreateWithProtocol(
								client.id(), cstr,
								kMIDIProtocol_2_0, &_ref,
								^ auto (const ::MIDIEventList* evtlist, void*) noexcept -> void {
									_dispatch_events(*evtlist, w);
								}
							);

	// check if there was an error
	if (err != noErr)
		throw cm::exception{err, "MIDIDestinationCreateWithProtocol"};
}

/* move constructor */
cm::destination::destination(self&& other) noexcept
: cm::object{other._ref},
  _external{other._external} {
	other._ref = 0U;
	other._external = true;
}

/* destructor */
cm::destination::~destination(void) noexcept {
	// dispose endpoint
	self::_dispose();
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto cm::destination::operator=(self&& other) noexcept -> self& {

	// check self assignment
	if (this == &other)
		return *this;

	// dispose endpoint
	self::_dispose();

	// move source
	_ref       = other._ref;
	other._ref = 0U;
	_external  = other._external;
	other._external = true;

	// return self
	return *this;
}

// -- private methods ---------------------------------------------------------

/* dispose */
auto cm::destination::_dispose(void) noexcept -> void {

	// check source mode
	if (_external)
		return;

	// check endpoint validity
	if (_ref == 0U)
		return;

	// dispose endpoint
	static_cast<void>(
			::MIDIEndpointDispose(_ref)
		);
}


// -- private static methods --------------------------------------------------

/* read midi */
template <typename T>
static auto _dispatch_events(const ::MIDIEventList& evtlist, T& parser) -> void {
	//static_cast<void>(evtlist);
	//if (evtlist == nullptr) { return; }
	std::cout << "MIDI Event List - Num Packets: " << evtlist.numPackets << "\r\n";

	const auto* packet = evtlist.packet;

	for (cm::u32 i = 0U; i < evtlist.numPackets; ++i) {

		for (cm::u32 j = 0U; j < packet->wordCount; ++j) {

			const cm::u32 word = packet->words[j];
			// extract bytes
			cm::u8 byte0 = (word >> 0)  & 0xFF;
			cm::u8 byte1 = (word >> 8)  & 0xFF;
			cm::u8 byte2 = (word >> 16) & 0xFF;
			cm::u8 byte3 = (word >> 24) & 0xFF;
			parser.feed(byte0);
			parser.feed(byte1);
			parser.feed(byte2);
			parser.feed(byte3);
		}

		packet = ::MIDIEventPacketNext(packet);
	}
}

			//const cm::u8 status = word & 0xFF;
			//const cm::u8 data1  = (word >> 8) & 0xFF;
			//const cm::u8 data2  = (word >> 16) & 0xFF;
			//
			//parser.feed(status);

#endif // midx_macos
