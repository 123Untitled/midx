#ifndef coremidi_string_hpp
#define coremidi_string_hpp

#include "os.hpp"

#if defined(midx_macos)

#include <CoreFoundation/CoreFoundation.h>


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	// -- forward declarations ------------------------------------------------

	class object;


	// -- S T R I N G ---------------------------------------------------------

	class string final {


		// -- friends ---------------------------------------------------------

		/* object as friend */
		friend class cm::object;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::string;


			// -- private members ---------------------------------------------

			/* string */
			::CFStringRef _string;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			string(void) noexcept
			: _string{nullptr} {
			}

			/* string view constructor */
			string(const char* str)
			: _string{::CFStringCreateWithCString(kCFAllocatorDefault,
												  str,
												  kCFStringEncodingASCII)} {

				// check if string was created
				if (_string == nullptr)
					throw "error creating string";
			}

			/* copy constructor */
			string(const self& other)
			: _string{::CFStringCreateCopy(kCFAllocatorDefault, other._string)} {

				// check if string was created
				if (_string == nullptr)
					throw "error creating string";
			}

			/* move constructor */
			string(self&& other) noexcept
			: _string{other._string} {
				other._string = nullptr;
			}

			/* destructor */
			~string(void) noexcept {
				// release string
				if (_string == nullptr)
					return;
				::CFRelease(_string);
			}


			// -- public accessors --------------------------------------------

			/* length */
			auto length(void) const -> ::CFIndex {
				return ::CFStringGetLength(_string);
			}


			// -- public conversion operators ---------------------------------

			/* ::CFStringRef conversion operator */
			operator ::CFStringRef(void) const noexcept {
				return _string;
			}


	}; // class string

} // namespace coremidi

#endif // midx_macos

#endif // coremidi_string_hpp
