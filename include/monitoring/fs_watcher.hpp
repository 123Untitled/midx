#ifndef ml_monitoring_fs_watcher_hpp
#define ml_monitoring_fs_watcher_hpp

#include "types.hpp"
#include "system/unix_descriptor.hpp"
#include "monitoring/watcher.hpp"

#include <string>
#include <sys/event.h>

#include <iostream>

// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- F S  W A T C H E R --------------------------------------------------

	class fs_watcher : public ml::watcher,
					   public ml::unix_descriptor {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::fs_watcher;


			// -- private members ---------------------------------------------

			/* path */
			std::string _path;

			/* events */
			ml::u32 _evs;

			/* accumulated events */
			ml::u32 _acc;


		public:

			// -- public lifecycle --------------------------------------------

			/* path constructor */
			template <typename... Tp>
			fs_watcher(const char* path, const Tp&... args)
			: ml::watcher{},
			  ml::unix_descriptor{ml::open(path, args...)},
			  _path{path},
			  _evs{0U}, _acc{0U} {
			}

			/* deleted copy constructor */
			fs_watcher(const self&) = delete;

			/* move constructor */
			fs_watcher(self&&) noexcept = default;

			/* destructor */
			virtual ~fs_watcher(void) noexcept = default;


			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(ml::application&, const struct ::kevent& ev) -> void override final {
				_evs |= ev.fflags;
			}

			/* make add */
			auto make_add(void) noexcept -> struct ::kevent override final {
				return {
					.ident  = static_cast<::uintptr_t>(ml::unix_descriptor::operator int()),
					.filter = EVFILT_VNODE,
					.flags  = EV_ADD | EV_ENABLE | EV_CLEAR,
					.fflags = NOTE_WRITE | NOTE_EXTEND | NOTE_RENAME | NOTE_DELETE | NOTE_REVOKE,
					.data   = 0,
					.udata  = static_cast<void*>(this)
				};
			}

			/* make del */
			auto make_del(void) const noexcept -> struct ::kevent override final {
				return {
					.ident  = static_cast<::uintptr_t>(ml::unix_descriptor::operator int()),
					.filter = EVFILT_VNODE,
					.flags  = EV_DELETE,
					.fflags = 0U,
					.data   = 0,
					.udata  = nullptr
				};
			}


			// -- public accessors --------------------------------------------

			/* has events */
			auto has_events(void) const noexcept -> bool {
				return _evs != 0U;
			}

			/* has accumulated events */
			auto has_acc(const ml::u32& ev) const noexcept -> bool {
				return (_acc & ev) != 0U;
			}

			/* path */
			auto path(void) const noexcept -> const std::string& {
				return _path;
			}


			// -- public modifiers --------------------------------------------

			/* accumulate events */
			auto accumulate(void) noexcept -> void {
				_acc |= _evs;
				_evs = 0U;
			}

			/* reset */
			auto reset(void) noexcept -> void {
				_evs = 0U;
				_acc = 0U;
			}

	}; // class fs_watcher

} // namespace ml

#endif // ml_monitoring_fs_watcher_hpp
