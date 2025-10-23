#include "monitoring/project_watcher.hpp"

#include "monitoring/monitor.hpp"
#include "application.hpp"


// -- P R O J E C T  W A T C H E R --------------------------------------------

ml::project_watcher::project_watcher(const char* path, ml::monitor& monitor)
: _paths{path},
  _target{_paths.file(),   O_RDONLY},
  _parent{_paths.folder(), O_EVTONLY},
  _timer{20} {

	// subscribe watchers
	monitor.subscribe(_target);
	monitor.subscribe(_parent);
}


// -- public methods ----------------------------------------------

/* process events */
auto ml::project_watcher::process_events(ml::application& app) -> void {

	auto& monitor = app.monitor();

	if (_target.has_events()
	 || _parent.has_events()) {

		_target.accumulate();
		_parent.accumulate();

		_timer.reset();
		monitor.subscribe(_timer);
		return;
	}

	if (_timer.pop_state() == false)
		return;

	// check for lost tracking
	if (_target.has_acc(NOTE_REVOKE)
	 || _parent.has_acc(NOTE_REVOKE | NOTE_DELETE | NOTE_RENAME))
		throw ml::runtime_error{"losing file monitoring"};

	auto& project = app.project();

	if (_target.has_acc(NOTE_DELETE | NOTE_RENAME)) {

		if (_target.is_open()) {
			monitor.unsubscribe(_target);
			_target.close();
		}
	}
	else if (_target.has_acc(NOTE_WRITE | NOTE_EXTEND)) {
		ml::mapped_file mf{_target};
		project.update(mf);
	}

	if (_parent.has_acc(NOTE_WRITE | NOTE_EXTEND)) {

		if (_target.is_closed()) {

			static_cast<ml::unix_descriptor&>(_target)
				= ml::try_open(_paths.file(), O_RDONLY);

			if (_target.is_open()) {
				monitor.subscribe(_target);
				ml::mapped_file mf{_target};
				project.update(mf);
			}
		}
	}

	_target.reset();
	_parent.reset();
}
