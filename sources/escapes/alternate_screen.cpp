#include "escapes/alternate_screen.hpp"
#include "diagnostics/system_error.hpp"
#include <unistd.h>


// -- A L T E R N A T E  S C R E E N ------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::alternate_screen::alternate_screen(void) {

	// enable alternate screen
	if (::write(STDOUT_FILENO, "\x1b[?1049h", 8U) != 8)
		throw mx::system_error("write");
}

/* destructor */
mx::alternate_screen::~alternate_screen(void) noexcept {

	// disable alternate screen
	static_cast<void>(::write(STDOUT_FILENO, "\x1b[?1049l", 8U));
}
