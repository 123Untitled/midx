#ifndef coremidi_destination_hpp
#define coremidi_destination_hpp

#include "os.hpp"

#if defined(midx_macos)

#include "coremidi/object.hpp"
#include "coremidi/client.hpp"

#include "midi/midi_watcher.hpp"


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {



	// -- D E S T I N A T I O N -----------------------------------------------

	class destination final : public cm::object {


		private:


			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::destination;


			// -- private members ---------------------------------------------

			/* external */
			bool _external;




		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			destination(void) noexcept = default;

			/* id constructor */
			destination(const ::MIDIEndpointRef&);

			/* name constructor */
			destination(const cm::client&, const char*, mx::midi_watcher&);



			//destination(const cm::client& client, const char* name, mx::midi_watcher& w)
			//: _external{false} {
			//
			//	// create cfstring
			//	cm::string cstr{name};
			//
			//	// create a new destination
			//	const ::OSStatus err = ::MIDIDestinationCreate(client.id(),
			//												   cstr,
			//												   &self::_read_proc,
			//												   reinterpret_cast<void*>(&w),
			//												   &_ref);
			//
			//	// check if there was an error
			//	if (err != noErr)
			//		throw cm::exception{err, "MIDIDestinationCreateWithProtocol"};
			//
			//	// set unique id
			//	const auto uid = cm::unique_id::generate();
			//	this->unique_id(uid);
			//}

			/* deleted copy constructor */
			destination(const self&) = delete;

			/* move constructor */
			destination(self&&) noexcept;

			/* destructor */
			~destination(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self&;


		private:

			// -- private methods ---------------------------------------------

			/* dispose */
			auto _dispose(void) noexcept -> void;


			// -- private static methods --------------------------------------

			/* read midi */
			//static auto _read_proc(const ::MIDIEventList&) -> void;


			//static auto _read_proc(const ::MIDIPacketList* pl, void* refcon, void*) -> void {
			//
			//	auto* obj = reinterpret_cast<mx::midi_watcher*>(refcon);
			//
			//	// get first packet
			//	const ::MIDIPacket* packet = pl->packet;
			//
			//	// loop over packets
			//	for (cm::u32 p = 0U; p < pl->numPackets; ++p) {
			//
			//		// loop over bytes in packet
			//		for (cm::u16 b = 0U; b < packet->length; ++b)
			//			obj->feed(packet->data[b]);
			//
			//		// move to the next packet
			//		packet = ::MIDIPacketNext(packet);
			//	}
			//}

	}; // class destination

} // namespace coremidi

#endif // midx_macos

#endif // coremidi_destination_hpp
