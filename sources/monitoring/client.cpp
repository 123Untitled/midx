#include "monitoring/client.hpp"
#include "application.hpp"
#include <sys/socket.h>


// -- C L I E N T -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* monitor constructor */
mx::client::client(const mx::monitor& monitor) noexcept
: _monitor{&monitor}, _socket{}, _reader{}, _queue{} {
}


// -- public methods ----------------------------------------------------------

/* initialize */
auto mx::client::initialize(mx::socket&& sck) -> void {

	// store socket
	_socket = static_cast<mx::socket&&>(sck);

	// subscribe to monitor
	_monitor->add_read(*this);
}

/* send */
auto mx::client::send(mx::string&& msg) -> void {

	// enable write event
	if (_queue.empty() == true)
		_monitor->add_write(*this);

	// queue message
	_queue.emplace_back(std::move(msg));
}

/* is connected */
auto mx::client::is_connected(void) const noexcept -> bool {
	return _socket.is_open();
}



// -- public overrides --------------------------------------------------------

/* on event */
auto mx::client::on_event(mx::application& app, const struct ::kevent& ev) -> void {

	// detect disconnection or errors
	if ((ev.flags & EV_EOF)   != 0U
	 || (ev.flags & EV_ERROR) != 0U) {
		self::_disconnect();
		return;
	}

	switch (ev.filter) {

		// handle read event
		case EVFILT_READ:
			self::_read(app);
			break;

		// handle write event
		case EVFILT_WRITE:
			self::_write(app);
			break;
	}
}

/* ident */
auto mx::client::ident(void) const noexcept -> int {
	return _socket.operator int();
}



// -- private methods ---------------------------------------------------------

/* disconnect */
auto mx::client::_disconnect(void) -> void {

	// unsubscribe write event
	if (_queue.empty() == false)
		_monitor->del_write(*this);

	// unsubscribe read event
	_monitor->del_read(*this);

	// close socket
	_socket.close();

	// clear queue
	_queue.clear();
}

/* read */
auto mx::client::_read(mx::application& app) -> void {

	// read socket
	if (_reader.recv(_socket) == false)
		return;

	// handle disconnection
	if (_reader.eof()) {
		self::_disconnect();
		return;
	}

	// feed data to application
	app.protocol().feed(_reader, app);
}

/* write */
auto mx::client::_write(mx::application& app) -> void {

	// get next message to send
	auto& msg = _queue.front();

	// send data
	const auto sent = ::send(_socket, msg.data(), msg.size(), 0);

	// handle errors
	if (sent < 0) {

		if (errno == EAGAIN
		 || errno == EWOULDBLOCK
		 || errno == EINTR)
			return;

		// throw exception
		throw mx::system_error{"send"};
	}

	// erase sent data
	msg.advance(static_cast<mx::usz>(sent));

	// if message fully sent, remove from queue
	if (msg.is_sent() == true) {
		_queue.front().store();
		_queue.pop_front();
	}

	// if all data sent, remove write event
	if (_queue.empty())
		_monitor->del_write(*this);
}
