#include "application.hpp"
#include <sys/socket.h>
#include <sys/un.h>


// -- S E R V E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::server::server(const mx::monitor& monitor)
: mx::watcher{}, _socket{AF_UNIX, SOCK_STREAM}, _client{monitor} {

	const char* path = "/tmp/midx.sock";
	::unlink(path);

	const mx::address addr{path};

	// bind socket
	_socket.bind(addr);

	// listen socket
	_socket.listen();

	// subscribe to monitor
	monitor.add_read(*this);
}


// -- public methods ----------------------------------------------------------

/* broadcast */
auto mx::server::broadcast(const std::string& msg) -> void {

	if (_client.is_connected() == false)
		return;

	_client.send(std::move(msg));
}


// -- public overrides --------------------------------------------------------

/* on event */
auto mx::server::on_event(mx::application& app, const struct ::kevent& ev) -> void {

	// accept new client
	auto nsck = _socket.accept();

	// set non-blocking
	nsck.non_blocking();

	// initialize client
	_client.initialize(static_cast<mx::socket&&>(nsck));
}

/* ident */
auto mx::server::ident(void) const noexcept -> int {
	return _socket.operator int();
}
