#ifndef midilang_coremidi_exception_hpp
#define midilang_coremidi_exception_hpp

#include <exception>
#include "coremidi/types.hpp"
#include <CoreMIDI/CoreMIDI.h>


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
			const char* _what;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			exception(void) noexcept = delete;

			/* code constructor */
			exception(const ::OSStatus code, const char* what) noexcept
			: std::exception{}, _code{code}, _what{what} {
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
				return _what;
			}

	}; // class exception

} // namespace coremidi

#endif // midilang_coremidi_exception_hpp
