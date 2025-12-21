#ifndef realtime_hpp
#define realtime_hpp

#include "core/types.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- R E A L T I M E -----------------------------------------------------

	class realtime final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::realtime;


			// -- private members ---------------------------------------------

			/* min computation */
			mx::u32 _min_rt_quantum;

			/* max computation */
			mx::u32 _max_rt_quantum;


			/* period */
			mx::u32 _period;

			/* computation */
			mx::u32 _computation;

			/* constraint */
			mx::u32 _constraint;


			// -- private static methods --------------------------------------

			/* minimum rt computation */
			static auto min_rt_quantum(void) -> mx::u32;

			/* maximum rt computation */
			static auto max_rt_quantum(void) -> mx::u32;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			realtime(void) noexcept;


			// -- public methods ----------------------------------------------

			/* apply */
			auto apply(void) noexcept -> bool;


			// -- public modifiers --------------------------------------------

			/* period */
			auto period(const mx::u32 value) noexcept -> void {
				_period = value;
			}

	}; // class realtime

} // namespace mx

#endif // realtime_hpp
