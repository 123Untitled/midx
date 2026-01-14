#ifndef midx_mouse_hpp
#define midx_mouse_hpp

#include "diagnostics/system_error.hpp"
#include "core/string/string_view.hpp"
#include <unistd.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M O U S E -----------------------------------------------------------

	class mouse final {


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			mouse(void) {

				constexpr mx::string_view esc{"\x1b[?1003h\x1b[?1006h"};

				// enable mouse
				if (::write(STDOUT_FILENO, esc.data(), esc.size()) != esc.size())
					throw mx::system_error{"write"};
			}

			/* copy constructor */
			mouse(const mouse&) noexcept = default;

			/* move constructor */
			mouse(mouse&&) noexcept = default;

			/* destructor */
			~mouse(void) noexcept {

				constexpr mx::string_view esc{"\x1b[?1003l\x1b[?1006l"};

				// disable mouse
				static_cast<void>(::write(STDOUT_FILENO, esc.data(), esc.size()));
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const mouse&) noexcept -> mouse& = default;

			/* move assignment operator */
			auto operator=(mouse&&) noexcept -> mouse& = default;

	}; // class mouse

} // namespace mx

#endif // midx_mouse_hpp
