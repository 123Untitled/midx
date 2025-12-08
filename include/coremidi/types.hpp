#ifndef midilang_coremidi_types_hpp
#define midilang_coremidi_types_hpp


#include <CoreMIDI/CoreMIDI.h>

namespace cm {


	using object_ref = ::MIDIObjectRef;

	using os_status = ::OSStatus;


	// -- midi messages -------------------------------------------------------

	/* midi message 32 */
	using m32 = ::MIDIMessage_32;

	/* midi message 64 */
	using m64 = ::MIDIMessage_64;

	/* midi message 96 */
	using m96 = ::MIDIMessage_96;

	/* midi message 128 */
	using m128 = ::MIDIMessage_128;



	/* timestamp */
	using timestamp = ::MIDITimeStamp;

	/* protocol id */
	using protocol_id = ::MIDIProtocolID;

	/* item count */
	using item_count = ::ItemCount;

	/* endpoint reference */
	using endpoint_reference = ::MIDIEndpointRef;



	using u32 = UInt32;
	using s32 = SInt32;
	using u64 = UInt64;
	using s64 = SInt64;

	using u8 = UInt8;
	using i8 = SInt8;

	using byte = ::Byte;

} // namespace coremidi

#endif // midilang_coremidi_types_hpp
