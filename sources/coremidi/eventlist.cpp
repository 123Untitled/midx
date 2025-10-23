#include "coremidi/eventlist.hpp"

/* default constructor */
cm::eventlist::eventlist(void)
: _buffer{}, _list{nullptr}, _packet{nullptr} {

	// reserve default buffer size
	_buffer.resize(BUFFER_SIZE);

	// initialize the event list
	_list = reinterpret_cast<::MIDIEventList*>(_buffer.data());

	// initialize the event packet
	_packet = ::MIDIEventListInit(_list, kMIDIProtocol_1_0);
}

/* copy constructor */
cm::eventlist::eventlist(const self& other)
: _buffer{other._buffer},
  _list{reinterpret_cast<MIDIEventList*>(_buffer.data())},
  _packet{&(_list->packet[_list->numPackets ? (_list->numPackets - 1U) : 0U])} {
}


// -- public modifiers --------------------------------------------------------

/* clear */
auto cm::eventlist::clear(void) -> void {

	// initialize the event packet
	_packet = ::MIDIEventListInit(_list, kMIDIProtocol_1_0);

	if (_packet == nullptr)
		throw cm::exception{0, "failed to clear event list"};
}

/* send */
auto cm::eventlist::send(const cm::source& source) -> void {

	// check packet validity
	//if (_packet == nullptr)
	//	return;

	// send midi to source device
	const ::OSStatus err = ::MIDIReceivedEventList(source.id(), _list);

	if (err != noErr)
		throw cm::exception{err, "failed to send event list"};
}



/* note on */
auto cm::eventlist::note_on(const cm::u8 channel,
							const cm::u8 note,
							const cm::u8 velocity) -> void {

	self::_add(::MIDI1UPNoteOn(0U, channel, note, velocity));
}

/* note off */
auto cm::eventlist::note_off(const cm::u8 channel,
							 const cm::u8 note) -> void {

	self::_add(::MIDI1UPNoteOff(0U, channel, note, 0U));
}

//UInt32 msg = MIDICLOCKTICK;
/*
msg = on ? 0x20900000 : 0x20800000; // note on or off
msg = msg | ((note & 0x7F) << 8); // note
msg = msg | 127; // velocity
*/



/* common midi messages */
enum : cm::m32 {
	CLOCK = 0x10F80000,
	START = 0x10FA0000,
	STOP  = 0x10FC0000,
	ON    = 0x20900000,
	OFF   = 0x20800000
};


/* tick */
auto cm::eventlist::tick(void) -> void {
	self::_add(CLOCK);
}

/* start */
auto cm::eventlist::start(void) -> void {
	self::_add(START);
}

/* stop */
auto cm::eventlist::stop(void) -> void {
	self::_add(STOP);
}


// -- private methods ---------------------------------------------------------

/* add */
auto cm::eventlist::_add(const cm::m32& msg) -> void {

	// check packet validity
	if (_packet == nullptr)
		return;

	addmsg:

	// add the midi message to the packet
	_packet = ::MIDIEventListAdd(_list,
								 _buffer.size(),
								 _packet,
								 0U,
								 1U,
								 &msg);

	// check packet validity
	if (!_packet) {
		std::cout << "midi eventlist resize\r\n";
		// check if the buffer can be resized
		if (_resize() == false) {
			std::cout << "\x1b[32m" << "buffer full" << "\x1b[0m" << std::endl;
			exit(1);
			return;
		}
		// try again
		goto addmsg;
	}
}

/* resize */
auto cm::eventlist::_resize(void) -> bool {

	if (_buffer.size() == MAX_EVENT)
		return false;

	auto size = _buffer.size() * 2U;

	// check if the max number of events has been reached
	if (size > MAX_EVENT)
		size = MAX_EVENT;

	// else resize the buffer
	_buffer.resize(size);

	// update the list and packet pointers
	_list = reinterpret_cast<MIDIEventList*>(_buffer.data());

	// update the packet pointer
	const unsigned i = _list->numPackets ? (_list->numPackets - 1U) : 0U;

	_packet = &(_list->packet[i]);

	return true;
}
