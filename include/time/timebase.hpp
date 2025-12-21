#ifndef timebase_hpp
#define timebase_hpp

#include <mach/mach_time.h>
#include "core/types.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- T I M E B A S E -----------------------------------------------------

	class timebase final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::timebase;


			// -- private members ---------------------------------------------

			/* timebase info */
			::mach_timebase_info_data_t _timebase;

			/* ticks to nanoseconds */
			double _ticks_to_ns;

			/* ticks to milliseconds */
			double _ticks_to_ms;

			/* ticks to microseconds */
			double _ticks_to_us;

			/* nanoseconds to ticks */
			double _ns_to_ticks;

			/* milliseconds to ticks */
			double _ms_to_ticks;

			/* microseconds to ticks */
			double _us_to_ticks;


			enum : mx::u64 {
				/* nanoseconds per microsecond */
				ns_per_us = 1'000ull,
				/* microseconds per second */
				us_per_s  = 1'000'000ull,
				/* nanoseconds per second */
				ns_per_s  = 1'000'000'000ull,
				/* nanoseconds per millisecond */
				ns_per_ms = 1'000'000ull
			};


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			timebase(void);

			/* deleted copy constructor */
			timebase(const self&) = delete;

			/* deleted move constructor */
			timebase(self&&) = delete;

			/* destructor */
			~timebase(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) -> const self&;


		public:

			// -- public static methods ---------------------------------------


			/* num */
			static auto num(void) -> mx::u64;

			/* den */
			static auto den(void) -> mx::u64;

			/* ticks to ns */
			static auto ticks_to_ns(void) -> double;

			/* ticks to ms */
			static auto ticks_to_ms(void) -> double;

			/* ticks to us */
			static auto ticks_to_us(void) -> double;

			/* ns to ticks */
			static auto ns_to_ticks(void) -> double;

			/* ms to ticks */
			static auto ms_to_ticks(void) -> double;


			/* us to ticks */
			template <typename T = double>
			static auto us_to_ticks(const mx::u64 us) -> T {
				return static_cast<T>(static_cast<double>(us) * self::_shared()._us_to_ticks);
			}

			/* ms to ticks */
			template <typename T = double>
			static auto ms_to_ticks(const mx::u64 ms) -> T {
				return static_cast<T>(static_cast<double>(ms) * self::_shared()._ms_to_ticks);
			}

	}; // class timebase

} // namespace mx

#endif // timebase_hpp
