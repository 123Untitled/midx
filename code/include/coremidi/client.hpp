#ifndef coremidi_client_hpp
#define coremidi_client_hpp

#include "os.hpp"

#if defined(midx_macos)

#include "coremidi/object.hpp"
#include <CoreMIDI/CoreMIDI.h>


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	// -- C L I E N T ---------------------------------------------------------

	class client final : public cm::object {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::client;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			client(void) noexcept = default;

			/* name constructor */
			client(const char*);

			/* deleted copy constructor */
			client(const self&) = delete;

			/* move constructor */
			client(self&&) noexcept;

			/* destructor */
			~client(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self&;


		private:

			// -- private methods ---------------------------------------------

			/* dispose */
			auto _dispose(void) noexcept -> void;


			// -- private static methods --------------------------------------

			/* _notification */
			static auto _notification(const ::MIDINotification*, void*) noexcept -> void;

	}; // class client

} // namespace coremidi

#endif // midx_macos

#endif // coremidi_client_hpp
