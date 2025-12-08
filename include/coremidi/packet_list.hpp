#ifndef coremidi_packet_list_hpp
#define coremidi_packet_list_hpp

#include "os.hpp"

#if defined(midx_macos)

#include "coremidi/source.hpp"
#include "coremidi/types.hpp"
#include <vector>


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	// -- P A C K E T  L I S T ------------------------------------------------

	class packet_list final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::packet_list;


			// -- private constants -------------------------------------------

			/* buffer size */
			enum : unsigned {
				BUFFER_SIZE = sizeof(::MIDIPacketList),
				// The maximum size of an event list is 65,536 bytes.
				MAX_EVENT = 65'536
			};


			// -- private members ---------------------------------------------

			/* buffer */
			std::vector<UInt8> _buffer;


			/* MIDIPacketList
			   A list of MIDI events the system sends to or receives from an endpoint.
					- numPackets -> The number of MIDI packets in the list.
					- packet     -> An open-ended array of variable-length MIDI packets. */
			::MIDIPacketList* _list;

			/* MIDIPacket
			   A collection of simultaneous MIDI events.
					- timeStamp -> The MIDI packet timestamp.
					- length -> The number of valid MIDI data bytes in this packet.
					- data -> A variable-length stream of MIDI messages. */
			::MIDIPacket* _packet;


			cm::client _client;
			cm::source _source;

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			packet_list(void);

			/* copy constructor */
			packet_list(const self&);

			/* move constructor */
			packet_list(self&&) noexcept = default;

			/* destructor */
			~packet_list(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public accessors --------------------------------------------

			/* empty */
			auto empty(void) const -> bool {
				return _list->numPackets == 0U;
			}


			// -- public modifiers --------------------------------------------

			/* clear */
			auto clear(void) -> void;

			/* send */
			auto send(const cm::source&) -> void;
			auto send(void) -> void;

			/* note on */
			auto note_on(const cm::u8,
						 const cm::u8,
						 const cm::u8) -> void;

			/* note off */
			auto note_off(const cm::u8,
						  const cm::u8) -> void;

			/* tick */
			auto tick(void) -> void;

			/* start */
			auto start(void) -> void;

			/* stop */
			auto stop(void) -> void;


		private:

			// -- private methods ---------------------------------------------

			/* add */
			template <unsigned N>
			auto _add(const cm::byte (&)[N]) -> void;


			/* resize */
			auto _resize(void) -> bool;



	}; // class packet_list

} // namespace cm

#endif // midx_macos

#endif // coremidi_packet_list_hpp
