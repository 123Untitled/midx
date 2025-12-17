#ifndef coremidi_unique_id_hpp
#define coremidi_unique_id_hpp

#include "os.hpp"

#if defined(midx_macos)

#include "coremidi/types.hpp"
#include "coremidi/exception.hpp"

#include <iostream>


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	// -- U N I Q U E  I D ----------------------------------------------------

	class unique_id final {


		private:

			// -- private members ---------------------------------------------

			static auto _is_existing(const cm::i32 uid) -> bool {

				std::cout << "------\r\n";
				const auto num_src = ::MIDIGetNumberOfSources();

				if (!num_src)
					throw cm::exception{0, "no MIDI sources available"};

			    for (cm::item_count i = 0U; i < num_src; ++i) {

			        const cm::endpoint src = ::MIDIGetSource(i);
					if (src == 0U)
						throw cm::exception{0, "MIDIGetSource returned null endpoint"};

					cm::i32 id;
					const cm::os_status status = ::MIDIObjectGetIntegerProperty(src, kMIDIPropertyUniqueID, &id);

					if (status != noErr)
						throw cm::exception{status, "MIDIObjectGetIntegerProperty failed"};

					std::cout << "SRC ID: " << id << "\r\n";
			        if (id == uid)
			            return true;
			    }


			    const auto num_dst = ::MIDIGetNumberOfDestinations();

				if (!num_dst)
					throw cm::exception{0, "no MIDI destinations available"};

			    for (cm::item_count i = 0U; i < num_dst; ++i) {
			        const cm::endpoint dst = ::MIDIGetDestination(i);
					if (dst == 0U)
						throw cm::exception{0, "MIDIGetDestination returned null endpoint"};

					cm::i32 id;
					const cm::os_status status = ::MIDIObjectGetIntegerProperty(dst, kMIDIPropertyUniqueID, &id);

					if (status != noErr)
						throw cm::exception{status, "MIDIObjectGetIntegerProperty failed"};

					std::cout << "DST ID: " << id << "\r\n";
					if (id == uid)
			            return true;
			    }

			    return false;
			}

		public:

			// -- public static methods ------------------------------------------

			static auto generate(void) -> cm::i32 {

				static cm::i32 uid = 123;

				while (_is_existing(uid))
					++uid;

				return uid;
			}

	}; // class unique_id

} // namespace coremidi

#endif // midx_macos

#endif // coremidi_unique_id_hpp
