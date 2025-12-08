#ifndef coremidi_source_hpp
#define coremidi_source_hpp

#include "os.hpp"

#if defined(midx_macos)

#include "coremidi/object.hpp"
#include "coremidi/client.hpp"
#include "coremidi/types.hpp"

#include <iostream>


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	using midi_endpoint_reference = ::MIDIEndpointRef;


	// -- S O U R C E ---------------------------------------------------------

	class source final : public cm::object {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::source;


			// -- private members ---------------------------------------------

			/* external */
			bool _external;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			source(void) noexcept = default;

			/* id constructor */
			source(const ::MIDIEndpointRef&);

			/* name constructor */
			source(const cm::client&, const char*);

			/* deleted copy constructor */
			source(const self&) = delete;

			/* move constructor */
			source(self&&) noexcept;

			/* destructor */
			~source(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self&;


		private:

			// -- private methods ---------------------------------------------

			/* dispose */
			auto _dispose(void) noexcept -> void;

	}; // class source

} // namespace coremidi

#endif // midx_macos

#endif // coremidi_source_hpp
