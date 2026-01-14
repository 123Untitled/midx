#ifndef host_time_hpp
#define host_time_hpp

#include "core/types.hpp"
#include "time/timebase.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- H O S T  T I M E ----------------------------------------------------

	class host_time final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::host_time;


			// -- private members ---------------------------------------------

			/* ticks */
			mx::u64 _ticks;


		public:

			// -- public lifecycle --------------------------------------------

			host_time(void) noexcept
			: _ticks(0U) {
			}

			explicit host_time(const mx::u64 ticks) noexcept
			: _ticks(ticks) {
			}


			// -- public static methods ---------------------------------------

			/* now */
			static auto now(void) noexcept -> self {
				return mx::host_time{::mach_absolute_time()};
			}


			// -- public methods ----------------------------------------------

			/* to nano seconds */
			auto to_ns(void) const noexcept -> double {
				return _ticks * mx::timebase::ticks_to_ns();
			}

			/* to milli seconds */
			auto to_ms(void) const noexcept -> double {
				return _ticks * mx::timebase::ticks_to_ms();
			}

			/* to micro seconds */
			auto to_us(void) const noexcept -> double {
				return _ticks * mx::timebase::ticks_to_us();
			}


			// -- public operators --------------------------------------------

			/* uint64 conversion operator */
			operator mx::u64(void) const noexcept {
				return _ticks;
			}


			/* addition operator */
			auto operator+(const self& other) const noexcept -> self {
				return self{_ticks + other._ticks};
			}

			/* subtraction operator */
			auto operator-(const self& other) const noexcept -> self {
				return self{_ticks - other._ticks};
			}
	};

} // namespace mx

#endif // host_time_hpp
