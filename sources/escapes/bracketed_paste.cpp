#include "escapes/bracketed_paste.hpp"
#include <unistd.h>

#include "core/types.hpp"
#include "core/string/string_view.hpp"
#include "diagnostics/system_error.hpp"


// -- B R A C K E T E D  P A S T E --------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::bracketed_paste::bracketed_paste(void) {

	constexpr mx::string_view view{"\x1b[?2004h"};

	// enable bracketed paste mode
	const mx::isz bytes = ::write(STDOUT_FILENO, view.data(), view.size());

	if (bytes != view.size())
		throw mx::system_error{"write"};
}

/* destructor */
mx::bracketed_paste::~bracketed_paste(void) noexcept {

	// disable bracketed paste mode
	static_cast<void>(::write(STDOUT_FILENO, "\x1b[?2004l", 8U));
}
