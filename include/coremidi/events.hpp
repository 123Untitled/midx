#ifndef coremidi_event_list_hpp
#define coremidi_event_list_hpp

#include "os.hpp"

#if defined(midx_macos)

#include "coremidi/source.hpp"
#include "coremidi/types.hpp"
//#include "coremidi/output.hpp"

#include <vector>


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {

	// -- forward declarations ------------------------------------------------

	class source;
	class output;
	class destination;


	// -- E V E N T  L I S T --------------------------------------------------

	class event_list final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::event_list;


			// -- private constants -------------------------------------------

			/* buffer size */
			enum : unsigned {
				BUFFER_SIZE = sizeof(::MIDIEventList),
				// The maximum size of an event list is 65,536 bytes.
				MAX_EVENT = 65'536
			};


			// -- private members ---------------------------------------------

			/* buffer */
			std::vector<UInt8> _buffer;

			/* A variable-length list of MIDI event packets. */
			/*       - protocol (MIDIProtocolID) -> The MIDI protocol variant of the events in the list. */
			/*       - numpackets (UInt32) -> The number of MIDI event packet structures in the list. */
			/*       - packet (MIDIEventpacket) -> An array of variable-length MIDI event packet structures. */
			::MIDIEventList* _list;

			/* A series of simultaneous MIDI events in Universal MIDI packets (UMP) format. */
			/*       - timeStamp (UInt64) -> The event packet timestamp. */
			/*       - wordCount (UInt32) -> The number of valid MIDI 32-bit words in this event packet. */
			/*       - words (UInt32 []) -> A variable-length stream of native-endian 32-bit Universal MIDI packets (UMP). */
			::MIDIEventPacket* _packet;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			event_list(void);

			/* copy constructor */
			event_list(const self&);

			/* move constructor */
			event_list(self&&) noexcept = default;

			/* destructor */
			~event_list(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public accessors --------------------------------------------

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _list->numPackets == 0U;
			}


			// -- public modifiers --------------------------------------------

			/* send */
			auto send(const cm::source&) -> void;

			/* send */
			auto send(const cm::output&, const cm::destination&) -> void;

			/* note on */
			//auto note_on(const cm::u8,
			//			 const cm::u8,
			//			 const cm::u8) -> void;

			/* note off */
			auto note_off(const cm::u8,
						  const cm::u8,
						  const mx::u64) -> void;

			auto note_on(const cm::u8,
						 const cm::u8,
						 const cm::u8,
						 const mx::u64) -> void;

			/* note off */
			//auto note_off(const cm::u8,
			//			  const cm::u8) -> void;

			/* tick */
			//auto tick(void) -> void;
			//
			///* start */
			//auto start(void) -> void;
			//
			///* stop */
			//auto stop(void) -> void;

			/* clear */
			auto clear(void) -> void;


		private:

			// -- private methods ---------------------------------------------

			/* add */
			auto _add(const cm::m32, mx::u64) -> void;

			/* resize */
			auto _resize(void) -> bool;


	}; // class event_list

}; // namespace coremidi

#endif // midx_macos

#endif // coremidi_event_list_hpp
