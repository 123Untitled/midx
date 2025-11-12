#ifndef midilang_time_clock_hpp
#define midilang_time_clock_hpp

#include <pthread.h>

#include "core/types.hpp"
#include "time/bpm.hpp"
#include "monitoring/watcher.hpp"

#include <time.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- C L O C K A B L E ---------------------------------------------------

	class clockable {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::clockable;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			clockable(void) noexcept = default;

			/* copy constructor */
			clockable(const self&) noexcept = default;

			/* move constructor */
			clockable(self&&) noexcept = default;

			/* destructor */
			virtual ~clockable(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public interface --------------------------------------------

			/* start */
			virtual auto clock_start(void) -> void = 0;

			/* stop */
			virtual auto clock_stop(void) -> void = 0;

			/* tick */
			virtual auto clock_tick(const mx::u64&) -> void = 0;

	}; // class clockable


	// -- C L O C K -----------------------------------------------------------

	class clock final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::clock;


			// -- private members ---------------------------------------------

			/* thread */
			::pthread_t _thread;

			/* event fd */
			int _efd;

			mx::watcher* _watcher;

			/* running */
			bool _running;

			/* bpm */
			mx::bpm _bpm;

			/* nano clock */
			mx::i64 _nano_clock;

			/* nano target */
			mx::i64 _nano_target;

			/* count */
			mx::u64 _count;


			/* timeline */
			double _timeline;

			/* request */
			struct ::timespec  _request;

			mx::i64 _start;
			mx::i64 _last_start;
			mx::i64 _end;

			mx::i64 _elapsed;
			mx::i64 _acc_ref;
			mx::i64 _acc_stamp;
			mx::i64 _acc_diff;
			mx::i64 _loop_diff;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			clock(const int efd, mx::watcher&) noexcept;

			/* copy constructor */
			clock(const self&) noexcept = default;

			/* move constructor */
			clock(self&&) noexcept = default;

			/* destructor */
			~clock(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* start */
			auto start(void) -> void;

			/* stop */
			auto stop(void) noexcept -> void;

			/* bpm */
			auto bpm(const unsigned&) noexcept -> void;


			// -- public accessors --------------------------------------------

			/* is running */
			auto is_running(void) const noexcept -> bool;

			/* bpm */
			auto bpm(void) const noexcept -> const mx::bpm&;

			/* count */
			auto count(void) const noexcept -> mx::u64;

			/* timeline */
			auto timeline(void) const noexcept -> double;


		private:

			// -- private static methods --------------------------------------

			/* entrypoint */
			static auto _entrypoint(void*) noexcept -> void*;


			// -- private methods ---------------------------------------------


			/* loop */
			auto _loop(void) noexcept -> void;

			/* init clock */
			auto _init_clock(void) noexcept -> void;

			/* begin */
			auto _begin(void) noexcept -> void;

			/* compute timeline */
			auto _compute_timeline(void) noexcept -> void;

			/* compute diff */
			auto _compute_diff(void) noexcept -> void;

			/* sleep */
			auto _sleep(void) noexcept -> void;

	}; // class clock

} // namespace mx

#endif // midilang_time_clock_hpp
