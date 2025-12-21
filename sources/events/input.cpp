#include "events/input.hpp"
#include "diagnostics/system_error.hpp"
#include <unistd.h>
#include <iostream>
#include "monitoring/monitor.hpp"
#include "application.hpp"

#include "debug_chars.hpp"


// -- I N P U T ---------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* monitor constructor */
mx::input::input(const mx::monitor& monitor)
: _tty{}, _bp{}, _mouse{},
	//_as{},
	_buffer{} {

	// subscribe to read events
	monitor.add_read(*this);

	// set terminal raw mode
	_tty.blocking_raw();
}



// -- public overrides --------------------------------------------------------

/* on event */
auto mx::input::on_event(mx::application& app, const struct ::kevent&) -> void {

	constexpr mx::usz size = 1024U;

	_buffer.resize(_buffer.size() + size);
	void* ptr = (_buffer.data() + _buffer.size()) - size;

	const auto bytes = ::read(STDIN_FILENO, ptr, size);

	if (bytes < 0) {

		if (errno == EINTR || errno == EAGAIN)
			return;
		throw mx::system_error{"read"};
	}

	_buffer.resize((_buffer.size() - size) + (mx::usz)bytes);



	// handle ctrl-c
	for (mx::usz i = 0U; i < _buffer.size(); ++i) {
		std::cout << dbc{_buffer[i]};
		if (_buffer[i] == 0x03U) {
			app.exit();
			break;
		}
	}
	std::cout << "\r\n";

	_buffer.clear();
}

/* ident */
auto mx::input::ident(void) const noexcept -> int {
	return STDIN_FILENO;
}
