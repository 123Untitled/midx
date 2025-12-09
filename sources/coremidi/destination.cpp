#include "coremidi/destination.hpp"
#include "coremidi/string.hpp"

#if defined(midx_macos)

#include <iostream>

// -- D E S T I N A T I O N ---------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* id constructor */
cm::destination::destination(const ::MIDIEndpointRef& ref)
: cm::object{ref},
  _external{true} {
}

//MIDIReceiveBlock
//MIDIReadProc

/* name constructor */
cm::destination::destination(const cm::client& client, const char* name)
: _external{false} {

	// create cfstring
	cm::string cstr{name};

	// create a new destination
	const ::OSStatus err = ::MIDIDestinationCreateWithProtocol(client.id(),
															   cstr,
															   kMIDIProtocol_1_0,
															   &_ref,
															   ^(const ::MIDIEventList* evtlist, void*) {
																	cm::destination::_read_midi(*evtlist);
															   });

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
auto cm::destination::_read_midi(const ::MIDIEventList& evtlist) -> void {
	//static_cast<void>(evtlist);
	//if (evtlist == nullptr) { return; }

	std::cout << "numPackets: " << evtlist.numPackets << std::endl;

	for (std::size_t i = 0; i < evtlist.numPackets; ++i) {

		const MIDIEventPacket& packet = evtlist.packet[i];

		std::cout << "wordCount: " << packet.wordCount << std::endl;

		for (std::size_t j = 0; j < packet.wordCount; ++j) {

			const UInt32 word = packet.words[j];

			const UInt8 status = word & 0xFF;
			const UInt8 data1 = (word >> 8) & 0xFF;
			const UInt8 data2 = (word >> 16) & 0xFF;

			std::cout << "status: " << static_cast<int>(status) << std::endl;

			std::cout << "data1: " << static_cast<int>(data1) << std::endl;

			std::cout << "data2: " << static_cast<int>(data2) << std::endl;

		}
	}

}

#endif // midx_macos
