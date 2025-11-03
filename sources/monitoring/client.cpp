#include "monitoring/client.hpp"
#include <iostream>
#include "application.hpp"
#include <sys/socket.h>


// -- C L I E N T -------------------------------------------------------------

// -- public methods ----------------------------------------------------------

/* initialize */
auto ml::client::initialize(ml::socket&& sck, const ml::monitor& monitor) -> void {
	_socket = static_cast<ml::socket&&>(sck);
	monitor.subscribe(*this);
}

/* send */
auto ml::client::send(const std::string& msg) -> void {

	const ml::isz res = ::send(_socket, msg.data(), msg.size(), 0);
	if (res == -1)
		throw ml::system_error{"send"};
}

/* disconnect */
auto ml::client::disconnect(const ml::monitor& monitor) -> void {
	monitor.unsubscribe(*this);
	_socket.close();
	_buffer.clear();
}

/* read */
auto ml::client::read(ml::application& app) -> void {

	constexpr ml::usz buffer_size = 1024U;



	// read data from socket...
	while (true) {

		_buffer.resize(_buffer.size() + buffer_size);

		void* offset = _buffer.data() + (_buffer.size() - buffer_size);

		const ::ssize_t res = ::recv(_socket, offset, buffer_size, 0);

		if (res == -1) {

			_buffer.resize(_buffer.size() - buffer_size);

			if (errno == EAGAIN
			 || errno == EWOULDBLOCK)
				break;

			throw ml::system_error{"recv"};
		}

		// connection closed
		if (res == 0) {
			self::disconnect(app.monitor());
			break;
		}

		// new size
		const ml::usz size = (_buffer.size() - buffer_size)
						   + static_cast<ml::usz>(res);

		_buffer.resize(size);
	}
}


// -- public overrides --------------------------------------------------------

/* on event */
auto ml::client::on_event(ml::application& app, const struct ::kevent& ev) -> void {

	// detect disconnection or errors
	if ((ev.flags & EV_EOF) != 0U
	 || (ev.flags & EV_ERROR) != 0U) {
		self::disconnect(app.monitor());
		return;
	}

	if ((ev.filter == EVFILT_READ)) {
		self::read(app);
		app.protocol().feed(_buffer);
	}

	if ((ev.filter == EVFILT_WRITE)) {
		// ready to write
	}
}

/* make add */
auto ml::client::make_add(void) noexcept -> struct ::kevent {
	const struct ::kevent ev {
		.ident  = static_cast<::uintptr_t>(_socket.operator int()),
		.filter = EVFILT_READ | EVFILT_WRITE,
		.flags  = EV_ADD | EV_ENABLE | EV_CLEAR,
		.fflags = 0U,
		.data   = 0,
		.udata  = this
	};
	return ev;
}

/* make del */
auto ml::client::make_del(void) const noexcept -> struct ::kevent {
	const struct ::kevent ev {
		.ident  = static_cast<::uintptr_t>(_socket.operator int()),
		.filter = EVFILT_READ | EVFILT_WRITE,
		.flags  = EV_DELETE,
		.fflags = 0U,
		.data   = 0,
		.udata  = nullptr
	};
	return ev;
}
