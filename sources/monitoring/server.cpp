#include "application.hpp"
#include <sys/socket.h>
#include <sys/un.h>


// -- S E R V E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::server::server(const ml::monitor& monitor)
: ml::watcher{}, _socket{AF_UNIX, SOCK_STREAM}, _client{monitor} {

	const char* path = "/tmp/midx.sock";
	::unlink(path);

	const ml::address addr{path};

	// bind socket
	_socket.bind(addr);

	// listen socket
	_socket.listen();

	// subscribe to monitor
	monitor.subscribe(*this);
}


// -- public methods ----------------------------------------------------------

/* broadcast */
auto ml::server::broadcast(std::string& msg) -> void {

	if (_client.is_connected() == false)
		return;

	_client.send(std::move(msg));
}


// -- public overrides --------------------------------------------------------

/* on event */
auto ml::server::on_event(ml::application& app, const struct ::kevent& ev) -> void {

	// accept new client
	auto nsck = _socket.accept();

	// set non-blocking
	nsck.non_blocking();

	// initialize client
	_client.initialize(std::move(nsck), app.monitor());
}

/* make add */
auto ml::server::make_add(void) noexcept -> struct ::kevent {
	const struct ::kevent ev {
		.ident  = static_cast<::uintptr_t>(_socket.operator int()),
		.filter = EVFILT_READ,
		.flags  = EV_ADD | EV_ENABLE | EV_CLEAR,
		.fflags = 0U,
		.data   = 0,
		.udata  = this
	};
	return ev;
}

/* make del */
auto ml::server::make_del(void) const noexcept -> struct ::kevent {
	const struct ::kevent ev {
		.ident  = static_cast<::uintptr_t>(_socket.operator int()),
		.filter = EVFILT_READ,
		.flags  = EV_DELETE,
		.fflags = 0U,
		.data   = 0,
		.udata  = nullptr
	};
	return ev;
}
