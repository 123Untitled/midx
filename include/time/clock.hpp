#ifndef midilang_time_clock_hpp
#define midilang_time_clock_hpp

#include "types.hpp"
#include "time/bpm.hpp"

#include <time.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- C L O C K A B L E ---------------------------------------------------

	class clockable {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::clockable;


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
			virtual auto clock_tick(const ml::u64&) -> void = 0;

	}; // class clockable


	// -- C L O C K -----------------------------------------------------------

	class clock final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::clock;


			// -- private members ---------------------------------------------

			/* running */
			bool _running;

			/* bpm */
			ml::bpm _bpm;

			/* nano clock */
			ml::i64 _nano_clock;

			/* nano target */
			ml::i64 _nano_target;

			/* count */
			ml::u64 _count;


			/* timeline */
			double _timeline;

			/* request */
			struct ::timespec  _request;

			ml::i64 _start;
			ml::i64 _last_start;
			ml::i64 _end;

			ml::i64 _elapsed;
			ml::i64 _acc_ref;
			ml::i64 _acc_stamp;
			ml::i64 _acc_diff;
			ml::i64 _loop_diff;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			clock(void) noexcept;

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
			auto start(ml::clockable&) -> void;

			/* stop */
			auto stop(void) noexcept -> void;


			// -- public modifiers --------------------------------------------

			/* bpm */
			auto bpm(const unsigned&) noexcept -> void;


			// -- public accessors --------------------------------------------

			/* is running */
			auto is_running(void) const noexcept -> bool;

			/* bpm */
			auto bpm(void) const noexcept -> const ml::bpm&;

			/* count */
			auto count(void) const noexcept -> ml::u64;

			/* timeline */
			auto timeline(void) const noexcept -> double;


		private:

			// -- private methods ---------------------------------------------

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

} // namespace ml

#endif // midilang_time_clock_hpp
