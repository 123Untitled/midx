#include "monitoring/monitor.hpp"
#include "monitoring/watcher.hpp"
#include "diagnostics/system_error.hpp"
#include "application.hpp"


// -- M O N I T O R -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* path constructor */
mx::monitor::monitor(void)
: _kqueue{::kqueue()} /* uninitialized */ {

	// check errors
	if (_kqueue == -1)
		throw mx::system_error{"kqueue"};
}


// -- public methods ----------------------------------------------------------

/* add read */
auto mx::monitor::add_read(mx::watcher& w) const -> void {
	// record read event
	// level-triggered mode
	self::record<EVFILT_READ, EV_ADD | EV_ENABLE>(w.ident(), &w);
}

/* add write */
auto mx::monitor::add_write(mx::watcher& w) const -> void {
	// record write event
	// level-triggered mode
	self::record<EVFILT_WRITE, EV_ADD | EV_ENABLE>(w.ident(), &w);
}

/* del read */
auto mx::monitor::del_read(mx::watcher& w) const -> void {
	// delete read event
	self::record<EVFILT_READ, EV_DELETE>(w.ident());
}

/* del write */
auto mx::monitor::del_write(mx::watcher& w) const -> void {
	// delete write event
	self::record<EVFILT_WRITE, EV_DELETE>(w.ident());
}

/* add user */
auto mx::monitor::add_user(mx::watcher& w) const -> void {

	// make user event
	const struct kevent ev {
		.ident  = static_cast<uintptr_t>(w.ident()),
		.filter = EVFILT_USER,
		.flags  = EV_ADD | EV_ENABLE | EV_CLEAR,
		.fflags = 0U,
		.data   = 0U,
		.udata  = &w
	};

	// record event
	if (::kevent(_kqueue, &ev, 1, nullptr, 0, nullptr) == -1)
		throw mx::system_error{"kevent"};
}

/* wait */
auto mx::monitor::wait(mx::application& app) -> void {

	// 200 ms timeout
	constexpr struct timespec timeout {
		.tv_sec  = 0,
		.tv_nsec = 200 * 1000 * 1000
	};

	// wait events
	const auto ret = ::kevent(_kqueue, nullptr, 0,
							  _events, BUFFER_SIZE,
							  nullptr);
							  //&timeout);

	// check errors
	if (ret < 0) {

		// ignore interrupts
		if (errno == EINTR)
			return;

		// throw
		throw mx::system_error{"kevent"};
	}

	for (int i = 0; i < ret; ++i) {
		// handle event
		auto* watcher = static_cast<mx::watcher*>(_events[i].udata);
		watcher->on_event(app, _events[i]);
	}
}

