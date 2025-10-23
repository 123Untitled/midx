#ifndef midilang_coremidi_input_hpp
#define midilang_coremidi_input_hpp

#include "coremidi/string.hpp"
#include "coremidi/client.hpp"
#include "coremidi/destination.hpp"
#include "coremidi/source.hpp"


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	// -- I N P U T -----------------------------------------------------------

	class input final : public cm::object {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::input;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			input(void) noexcept = default;

			/* client constructor */
			input(const cm::client& client, const char* name) {

				// create cfstring
				corefoundation::string cstr{name};

				// create input port
				const ::OSStatus result = ::MIDIInputPortCreateWithProtocol(
											client.id(),
											cstr,
											kMIDIProtocol_1_0,
											&_ref,
											^(const MIDIEventList*, void*) {
											});

				// check if port was created
				if (result != noErr)
					throw coremidi::exception{result, "error creating midi input port"};
			}

			/* deleted copy constructor */
			input(const self&) = delete;

			/* move constructor */
			input(self&& other) noexcept
			: coremidi::object{other._ref} {

				// reset other
				other._ref = 0U;
			}

			/* destructor */
			~input(void) noexcept {

				// dispose port
				self::_dispose();
			}


			// -- public assignment operators -----------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				// check self assignment
				if (this == &other)
					return *this;

				// dispose port
				self::_dispose();

				// move port
				_ref       = other._ref;
				other._ref = 0U;

				return *this;
			}


			// -- public modifiers ----------------------------------------

			/* connect */
			auto connect(const cm::source& ep) -> void {

				// connect endpoint to port
				::OSStatus status = ::MIDIPortConnectSource(_ref, ep.id(), nullptr);

				if (status != noErr)
					throw coremidi::exception{status, "error connecting midi endpoint to port"};
			}

			/* disconnect */
			auto disconnect(const cm::source& ep) -> void {

				// disconnect endpoint from port
				::OSStatus status = ::MIDIPortDisconnectSource(_ref, ep.id());

				if (status != noErr)
					throw coremidi::exception{status, "error disconnecting midi endpoint from port"};
			}


		private:

			// -- private methods -----------------------------------------

			/* dispose */
			auto _dispose(void) noexcept -> void {

				// check port validity
				if (_ref == 0U)
					return;

				// dispose port
				static_cast<void>(
						::MIDIPortDispose(_ref)
				);
			}

	}; // class input

} // namespace coremidi

#endif // midilang_coremidi_input_hpp
