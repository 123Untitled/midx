#include "monitoring/project_watcher.hpp"

#include "monitoring/monitor.hpp"
#include "application.hpp"


// -- P R O J E C T  W A T C H E R --------------------------------------------

mx::project_watcher::project_watcher(const char* path, const mx::monitor& monitor)
: _paths{path},
  _target{_paths.file(),   O_RDONLY},
  _parent{_paths.folder(), O_EVTONLY},
  _timer{20} {

	// subscribe watchers
	monitor.subscribe(_target);
	monitor.subscribe(_parent);
}


// -- public methods ----------------------------------------------

/* has_changes */
auto mx::project_watcher::has_changes(mx::monitor& monitor) -> bool {

	bool changed = false;

	if (_target.has_events()
	 || _parent.has_events()) {

		_target.accumulate();
		_parent.accumulate();

		_timer.reset();
		monitor.subscribe(_timer);
		return false;
	}

	if (_timer.pop_state() == false)
		return false;

	// check for lost tracking
	if (_target.has_acc(NOTE_REVOKE)
	 || _parent.has_acc(NOTE_REVOKE | NOTE_DELETE | NOTE_RENAME))
		throw mx::runtime_error{"losing file monitoring"};


	if (_target.has_acc(NOTE_DELETE | NOTE_RENAME)) {

		if (_target.is_open()) {
			monitor.unsubscribe(_target);
			_target.close();
		}
	}
	else if (_target.has_acc(NOTE_WRITE | NOTE_EXTEND)) {
		changed = true;
	}

	if (_parent.has_acc(NOTE_WRITE | NOTE_EXTEND)) {

		if (_target.is_closed()) {

			static_cast<mx::unix_descriptor&>(_target)
				= mx::try_open(_paths.file(), O_RDONLY);

			if (_target.is_open()) {
				monitor.subscribe(_target);
				changed = true;
			}
		}
	}

	_target.reset();
	_parent.reset();

	return changed;
}
