#ifndef midilang_coremidi_destination_hpp
#define midilang_coremidi_destination_hpp

#include "coremidi/object.hpp"
#include "coremidi/client.hpp"
#include "coremidi/types.hpp"

#include <iostream>


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
			destination(const cm::client&, const char*);

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
			static auto _read_midi(const ::MIDIEventList&) -> void;

	}; // class destination

} // namespace coremidi

#endif // midilang_coremidi_destination_hpp
