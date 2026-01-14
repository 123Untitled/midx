#ifndef coremidi_exception_hpp
#define coremidi_exception_hpp

#include "os.hpp"
#include "core/string/inplace_string.hpp"

#if defined(midx_macos)

#include <exception>
#include "coremidi/types.hpp"


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	// -- E X C E P T I O N ---------------------------------------------------

	class exception : public std::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::exception;


			// -- private members ---------------------------------------------

			/* error code */
			cm::os_status _code;

			/* error message */
			mx::inplace_string<1024U> _msg;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			exception(void) noexcept = delete;

			/* code constructor */
			template <unsigned N>
			exception(const ::OSStatus code, const char (&where)[N]) noexcept
			: std::exception{}, _code{code}, _msg{where, ": ", _midi_error(code)} {
			}

			/* copy constructor */
			exception(const self&) noexcept = default;

			/* move constructor */
			exception(self&&) noexcept = default;

			/* destructor */
			~exception(void) noexcept override = default;


			// -- public operators --------------------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* code */
			auto code(void) const noexcept -> ::OSStatus {
				return _code;
			}

			/* what */
			auto what(void) const noexcept -> const char* override {
				return _msg.data();
			}


		private:

			// -- private static methods --------------------------------------

			/* midi error */
			constexpr auto _midi_error(const cm::os_status err) -> const char* {

				switch (err) {
					case kMIDIInvalidClient:
						return "The client is invalid.";
					case kMIDIInvalidPort:
						return "The port is invalid.";
					case kMIDIWrongEndpointType:
						return "A function received a source endpoint when it required a destination endpoint, or vice versa.";
					case kMIDINoConnection:
						return "The connection you’re trying to close doesn’t exist.";
					case kMIDIUnknownEndpoint:
						return "The system doesn’t recognize the endpoint.";
					case kMIDIUnknownProperty:
						return "The property you’re trying to query isn’t set on the object.";
					case kMIDIWrongPropertyType:
						return "The value you assigned to the property is the wrong type.";
					case kMIDINoCurrentSetup:
						return "A MIDI setup object doesn’t currently exist.";
					case kMIDIMessageSendErr:
						return "The communication with the MIDI server failed.";
					case kMIDIServerStartErr:
						return "The system can’t start the MIDI server.";
					case kMIDISetupFormatErr:
						return "The system can’t read the saved state.";
					case kMIDIWrongThread:
						return "A driver is calling a non-I/O function in the server from a thread other than the server’s main thread.";
					case kMIDIObjectNotFound:
						return "The requested object doesn’t exist.";
					case kMIDIIDNotUnique:
						return "The identifier you’re trying to set isn’t unique.";
					case kMIDINotPermitted:
						return "The process doesn’t have privileges for the requested operation.";
					case kMIDIUnknownError:
						return "The system can’t perform the requested operation.";
					default:
						return "Unknown error.";
				}
			}

	}; // class exception

} // namespace coremidi

#endif // midx_macos

#endif // coremidi_exception_hpp
