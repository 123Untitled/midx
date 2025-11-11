#include "monitoring/client.hpp"
#include <iostream>
#include "application.hpp"
#include <sys/socket.h>


// -- C L I E N T -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* monitor constructor */
ml::client::client(const ml::monitor& monitor) noexcept
: _monitor{&monitor}, _socket{}, _reader{} {
}


// -- public methods ----------------------------------------------------------

/* initialize */
auto ml::client::initialize(ml::socket&& sck, const ml::monitor& monitor) -> void {
	_socket = static_cast<ml::socket&&>(sck);
	monitor.subscribe(*this);
}

/* send */
auto ml::client::send(std::string&& msg) -> void {

	_send_buffer = static_cast<std::string&&>(msg);

	const struct ::kevent ev {
		.ident  = static_cast<::uintptr_t>(_socket.operator int()),
		.filter = EVFILT_WRITE,
		.flags  = EV_ADD | EV_ENABLE,
		.fflags = 0U,
		.data   = 0,
		.udata  = this
	};

	_monitor->record(ev);
}

/* disconnect */
auto ml::client::disconnect(const ml::monitor& monitor) -> void {
	monitor.unsubscribe(*this);
	_socket.close();
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

		// read socket
		if (_reader.recv(_socket) == false)
			return;

		// handle disconnection
		if (_reader.eof()) {
			this->disconnect(app.monitor());
			return;
		}

		// feed data to application
		app.protocol().feed(_reader);
	}

	if ((ev.filter == EVFILT_WRITE)) {
		std::cout << "client: write event\n";

		// send data
		const auto sent = ::send(
			_socket,
			_send_buffer.data(),
			_send_buffer.size(),
			0);

		if (sent < 0)
			throw ml::system_error{"send"};

		std::cout << "client: sent " << sent << " bytes\n";

		// erase sent data
		_send_buffer.erase(0U, static_cast<mx::usz>(sent));

		// if all data sent, remove write event
		if (_send_buffer.empty()) {
			const struct ::kevent dev {
				.ident  = static_cast<::uintptr_t>(_socket.operator int()),
				.filter = EVFILT_WRITE,
				.flags  = EV_DELETE,
				.fflags = 0U,
				.data   = 0,
				.udata  = nullptr
			};
			_monitor->record(dev);
			std::cout << "client: all data sent, write event removed\n";
		}

	}
}

/* make add */
auto ml::client::make_add(void) noexcept -> struct ::kevent {
	const struct ::kevent ev {
		.ident  = static_cast<::uintptr_t>(_socket.operator int()),
		.filter = EVFILT_READ,
		.flags  = EV_ADD | EV_ENABLE /* | EV_CLEAR, stay in level-triggered mode */,
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
		.filter = EVFILT_READ,
		.flags  = EV_DELETE,
		.fflags = 0U,
		.data   = 0,
		.udata  = nullptr
	};
	return ev;
}
