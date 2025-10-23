#ifndef midilang_coremidi_output_hpp
#define midilang_coremidi_output_hpp

#include "coremidi/string.hpp"
#include "coremidi/client.hpp"
#include "coremidi/destination.hpp"
#include "coremidi/source.hpp"


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	// -- O U T P U T ---------------------------------------------------------

	class output final : public cm::object {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::output;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			output(void) noexcept = default;

			/* client constructor */
			output(const cm::client& client, const char* name) {

				// create cfstring
				cm::string cstr{name};

				// create output port
				::OSStatus result = ::MIDIOutputPortCreate(client.id(), cstr, &_ref);

				// check if port was created
				if (result != noErr)
					throw cm::exception{result, "error creating midi output port"};
			}

			/* deleted copy constructor */
			output(const self&) = delete;

			/* move constructor */
			output(self&& other) noexcept
			: cm::object{other._ref} {

				// reset other
				other._ref = 0U;
			}

			/* destructor */
			~output(void) noexcept {

				// dispose port
				self::_dispose();
			}


			// -- public assignment operators ---------------------------------

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


		private:

			// -- private methods ---------------------------------------------

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


	}; // class output

} // namespace coremidi

#endif // midilang_coremidi_output_hpp
