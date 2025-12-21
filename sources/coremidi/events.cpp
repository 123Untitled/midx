#include "coremidi/events.hpp"

#if defined(midx_macos)

#include "coremidi/output.hpp"


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
	_packet = ::MIDIEventListInit(_list, kMIDIProtocol_2_0);
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
}

/* send */
auto cm::event_list::send(const cm::output& out, const cm::destination& dst) -> void {

	if (empty())
		return;

	// send midi to output port
	const ::OSStatus err = ::MIDISendEventList(out.id(), dst.id(), _list);

	if (err != noErr)
		throw cm::exception{err, "failed to send event list"};
}


/* note on */
//auto cm::event_list::note_on(const cm::u8 channel,
//							const cm::u8 note,
//							const cm::u8 velocity) -> void {
//
//	self::_add(::MIDI1UPNoteOn(0U, channel, note, velocity));
//}

auto cm::event_list::note_on(const cm::u8 channel,
							const cm::u8 note,
							const cm::u8 velocity,
							const mx::u64 ts) -> void {

	//self::_add(::MIDI1UPNoteOn(0U, channel, note, velocity), ts);
	self::_add(::MIDI2NoteOn(0U, channel, note, 0U, 0U, velocity), ts);
	//static MIDIMessage_64 MIDI2NoteOn(UInt8 group, UInt8 channel, UInt8 noteNumber, UInt8 attributeType, UInt16 attributeData, UInt16 velocity);
	//MIDIMessageTypeForUPWord
}

/* note off */
//auto cm::event_list::note_off(const cm::u8 channel,
//							 const cm::u8 note) -> void {
//
//	self::_add(::MIDI1UPNoteOff(0U, channel, note, 0U));
//}

auto cm::event_list::note_off(const cm::u8 channel,
							 const cm::u8 note,
							 const mx::u64 ts) -> void {

	//self::_add(::MIDI1UPNoteOff(0U, channel, note, 0U), ts);
	self::_add(::MIDI2NoteOff(0U, channel, note, 0U, 0U, 0U), ts);
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
//auto cm::event_list::tick(void) -> void {
//	self::_add(CLOCK);
//}
//
///* start */
//auto cm::event_list::start(void) -> void {
//	self::_add(START);
//}
//
///* stop */
//auto cm::event_list::stop(void) -> void {
//	self::_add(STOP);
//}

/* clear */
auto cm::event_list::clear(void) -> void {

	// initialize the event packet
	_packet = ::MIDIEventListInit(_list, kMIDIProtocol_2_0);

	if (_packet == nullptr)
		throw cm::exception{0, "failed to clear event list"};
}


// -- private methods ---------------------------------------------------------

/* add */
auto cm::event_list::_add(const cm::m64 msg, mx::u64 ts) -> void {

	// check packet validity
	if (_packet == nullptr)
		return;

	// get timestamp
	//if (ts == 0U)
	//	ts = AudioGetCurrentHostTime();

	addmsg:

	// add the midi message to the packet
	_packet = ::MIDIEventListAdd(_list,
								 _buffer.size(),
								 _packet,
								 //0U,
								 ts,
								 2U,
								 reinterpret_cast<const cm::u32*>(&msg));
								 

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


#endif // midx_macos
