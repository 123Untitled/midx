#include "terminal/tty.hpp"
#include "diagnostics/system_error.hpp"
#include <unistd.h>


// -- T T Y -------------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::tty::tty(void)
: _origin{self::_get()}, _raw{_origin} {

	// setup raw mode
	self::_setup_raw(_raw);
}

/* destructor */
mx::tty::~tty(void) noexcept {

	// restore terminal attributes
	static_cast<void>(::tcsetattr(STDIN_FILENO, TCSANOW, &_origin));
}


// -- private static methods --------------------------------------------------

/* get */
auto mx::tty::_get(void) -> struct ::termios {

	struct ::termios term;

	// get terminal attributes
	if (::tcgetattr(STDIN_FILENO, &term) == -1) {
		throw mx::system_error{"tcgetattr"}; }

	return term;
}

/* set */
auto mx::tty::_set(const struct ::termios& term) -> void {

	// set terminal attributes
	if (::tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		throw mx::system_error{"tcsetattr"};
}

/* setup raw */
auto mx::tty::_setup_raw(struct ::termios& term) noexcept -> void {

	// disable input flags
	term.c_iflag &= ~static_cast<::tcflag_t>(
		IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON
	);

	// disable output flags
	term.c_oflag &= ~static_cast<::tcflag_t>(
		OPOST
	);

	// disable local flags
	term.c_lflag &= ~static_cast<::tcflag_t>(
		ECHO | ECHONL | ICANON | ISIG | IEXTEN
	);

	// disable control flags
	term.c_cflag &= ~static_cast<::tcflag_t>(
		CSIZE | PARENB
	);

	// enable 8-bit characters
	term.c_cflag |= static_cast<::tcflag_t>(
		CS8
	);

	// set minimum number of characters for non-canonical read
	term.c_cc[VMIN] = 1U;

	// set timeout in deciseconds for non-canonical read
	term.c_cc[VTIME] = 0U;
}



// -- public static methods ---------------------------------------------------

/* raw */
auto mx::tty::blocking_raw(const unsigned char timeout) -> void {
	_raw.c_cc[VMIN] = timeout > 0U ? 0U : 1U;
	_raw.c_cc[VTIME] = timeout; // deciseconds
	self::_set(_raw);
}

/* non-blocking */
auto mx::tty::non_blocking_raw(void) -> void {
	_raw.c_cc[VMIN] = 0U;
	_raw.c_cc[VTIME] = 0U;
	self::_set(_raw);
}

/* restore */
auto mx::tty::restore(void) -> void {
	self::_set(_origin);
}
