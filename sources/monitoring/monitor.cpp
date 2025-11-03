#include "monitoring/monitor.hpp"
#include "monitoring/watcher.hpp"
#include "diagnostics/system_error.hpp"
#include "application.hpp"


// -- M O N I T O R -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* path constructor */
ml::monitor::monitor(void)
: _kqueue{::kqueue()} /* uninitialized */ {

	// check errors
	if (_kqueue == -1)
		throw ml::system_error{"kqueue"};
}


// -- public methods ----------------------------------------------------------

/* subscribe */
auto ml::monitor::subscribe(ml::watcher& w) const -> void {

	// make event
	const struct ::kevent ev = w.make_add();

	// record event
	if (::kevent(_kqueue, &ev, 1, nullptr, 0, nullptr) == -1)
		throw ml::system_error{"kevent"};
}

/* unsubscribe */
auto ml::monitor::unsubscribe(ml::watcher& w) const -> void {

	// make event
	const struct ::kevent ev = w.make_del();

	// record event
	if (::kevent(_kqueue, &ev, 1, nullptr, 0, nullptr) == -1)
		throw ml::system_error{"kevent"};
}

/* wait */
auto ml::monitor::wait(ml::application& app) -> void {

	// wait events
	const auto ret = ::kevent(_kqueue, nullptr, 0,
							  _events, BUFFER_SIZE,
							  nullptr);


	// check errors
	if (ret < 0) {

		// ignore interrupts
		if (errno == EINTR)
			return;

		// throw
		throw ml::system_error{"kevent"};
	}

	for (int i = 0; i < ret; ++i) {

		// handle event
		auto* watcher = static_cast<ml::watcher*>(_events[i].udata);
		watcher->on_event(app, _events[i]);
	}
}
