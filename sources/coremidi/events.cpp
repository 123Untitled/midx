#include "coremidi/events.hpp"

#if defined(midx_macos)


// -- E V E N T  L I S T ------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
cm::event_list::event_list(void)
: _buffer{}, _list{nullptr}, _packet{nullptr} {

	// reserve default buffer size
	_buffer.resize(BUFFER_SIZE);

	// initialize the event list
	_list = reinterpret_cast<::MIDIEventList*>(_buffer.data());

	// initialize the event packet
	_packet = ::MIDIEventListInit(_list, kMIDIProtocol_1_0);
}

/* copy constructor */
cm::event_list::event_list(const self& other)
: _buffer{other._buffer},
  _list{reinterpret_cast<MIDIEventList*>(_buffer.data())},
  _packet{&(_list->packet[_list->numPackets ? (_list->numPackets - 1U) : 0U])} {
}


// -- public modifiers --------------------------------------------------------

/* send */
auto cm::event_list::send(const cm::source& source) -> void {

	if (empty())
		return;

	// send midi to source device
	const ::OSStatus err = ::MIDIReceivedEventList(source.id(), _list);

	if (err != noErr)
		throw cm::exception{err, "failed to send event list"};

	// initialize the event packet after sending
	_clear();
}

/* note on */
auto cm::event_list::note_on(const cm::u8 channel,
							const cm::u8 note,
							const cm::u8 velocity) -> void {

	self::_add(::MIDI1UPNoteOn(0U, channel, note, velocity));

}

/* note off */
auto cm::event_list::note_off(const cm::u8 channel,
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
auto cm::event_list::tick(void) -> void {
	self::_add(CLOCK);
}

/* start */
auto cm::event_list::start(void) -> void {
	self::_add(START);
}

/* stop */
auto cm::event_list::stop(void) -> void {
	self::_add(STOP);
}


// -- private methods ---------------------------------------------------------
#include <CoreAudio/HostTime.h>   // ou AudioToolbox.h si tu préfères

/* add */
auto cm::event_list::_add(const cm::m32& msg) -> void {

	// check packet validity
	if (_packet == nullptr)
		return;

	// get timestamp
	const ::MIDITimeStamp ts = AudioGetCurrentHostTime();

	addmsg:

	// add the midi message to the packet
	_packet = ::MIDIEventListAdd(_list,
								 _buffer.size(),
								 _packet,
								 //0U,
								 ts,
								 1U,
								 &msg);

	// check packet validity
	if (_packet)
		return;

	// check if the buffer can be resized
	if (_resize() == false)
		throw cm::exception{0, "failed to add midi message"};

	// try again
	goto addmsg;
}

/* resize */
auto cm::event_list::_resize(void) -> bool {

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

/* clear */
auto cm::event_list::_clear(void) -> void {

	// initialize the event packet
	_packet = ::MIDIEventListInit(_list, kMIDIProtocol_1_0);

	if (_packet == nullptr)
		throw cm::exception{0, "failed to clear event list"};
}

#endif // midx_macos
