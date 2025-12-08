#include "coremidi/packet_list.hpp"

#if defined(midx_macos)


/* default constructor */
cm::packet_list::packet_list(void)
: _buffer{}, _list{nullptr}, _packet{nullptr},
  _client{"midx"}, _source{_client, "midx"} {

	// reserve default buffer size
	_buffer.resize(BUFFER_SIZE);

	// initialize the event list
	_list = reinterpret_cast<::MIDIPacketList*>(_buffer.data());

	// initialize the event packet
	_packet = ::MIDIPacketListInit(_list);
}

/* copy constructor */
cm::packet_list::packet_list(const self& other)
: _buffer{other._buffer},
  _list{reinterpret_cast<MIDIPacketList*>(_buffer.data())},
  _packet{&(_list->packet[_list->numPackets ? (_list->numPackets - 1U) : 0U])} {
}


// -- public modifiers --------------------------------------------------------

/* clear */
auto cm::packet_list::clear(void) -> void {

	// initialize the event packet
	_packet = ::MIDIPacketListInit(_list);

	if (_packet == nullptr)
		throw cm::exception{0, "failed to clear event list"};
}

/* send */
auto cm::packet_list::send(const cm::source& source) -> void {

	// send midi to source device
	const ::OSStatus err = ::MIDIReceived(source.id(), _list);

	if (err != noErr)
		throw cm::exception{err, "failed to send event list"};
}

/* send */
auto cm::packet_list::send(void) -> void {

	// send midi to source device
	const ::OSStatus err = ::MIDIReceived(_source.id(), _list);

	if (err != noErr)
		throw cm::exception{err, "failed to send event list"};
}


//UInt32 msg = MIDICLOCKTICK;
/*
msg = on ? 0x20900000 : 0x20800000; // note on or off
msg = msg | ((note & 0x7F) << 8); // note
msg = msg | 127; // velocity
*/



/* common midi messages */
//enum : cm::m32 {
//	CLOCK = 0x10F80000,
//	START = 0x10FA0000,
//	STOP  = 0x10FC0000,
//	ON    = 0x20900000,
//	OFF   = 0x20800000
//};

enum : cm::byte {
	CLOCK = 0xF8,
	START = 0xFA,
	STOP  = 0xFC,
	ON    = 0x90,
	OFF   = 0x80
};


/* note on */
auto cm::packet_list::note_on(const cm::u8 channel,
							const cm::u8 note,
							const cm::u8 velocity) -> void {

	const cm::byte msg[3U] {
		static_cast<cm::byte>(ON | (channel & 0x0FU)),
		static_cast<cm::byte>(note & 0x7FU),
		static_cast<cm::byte>(velocity & 0x7FU)
	};

	_add(msg);
}

/* note off */
auto cm::packet_list::note_off(const cm::u8 channel,
							   const cm::u8 note) -> void {

	const cm::byte msg[3U] {
		static_cast<cm::byte>(OFF | (channel & 0x0FU)),
		static_cast<cm::byte>(note & 0x7FU),
		0U
	};

	_add(msg);
}


/* tick */
auto cm::packet_list::tick(void) -> void {
	_add({CLOCK});
}

/* start */
auto cm::packet_list::start(void) -> void {
	_add({START});
}

/* stop */
auto cm::packet_list::stop(void) -> void {
	_add({STOP});
}


// -- private methods ---------------------------------------------------------

/* add */
template <unsigned N>
auto cm::packet_list::_add(const cm::byte (&msg)[N]) -> void {

	// check packet validity
	if (_packet == nullptr)
		return;

	addmsg:

	// add the midi message to the packet
	_packet = ::MIDIPacketListAdd(_list,
								 _buffer.size(),
								 _packet,
								 0U, N, msg);

	// check packet validity
	if (!_packet) {

		// check if the buffer can be resized
		if (_resize() == false)
			throw cm::exception{0, "failed to add midi message: buffer full"};

		// try again
		goto addmsg;
	}
}

/* resize */
auto cm::packet_list::_resize(void) -> bool {

	if (_buffer.size() == MAX_EVENT)
		return false;

	auto size = _buffer.size() * 2U;

	// check if the max number of events has been reached
	if (size > MAX_EVENT)
		size = MAX_EVENT;

	// else resize the buffer
	_buffer.resize(size);

	// update the list and packet pointers
	_list = reinterpret_cast<MIDIPacketList*>(_buffer.data());

	// update the packet pointer
	const unsigned i = _list->numPackets ? (_list->numPackets - 1U) : 0U;

	_packet = &(_list->packet[i]);

	return true;
}

#endif // midx_macos
