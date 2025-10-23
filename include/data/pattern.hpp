#ifndef ml_data_pattern_hpp
#define ml_data_pattern_hpp

#include "data/track_pool.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- P A T T E R N -------------------------------------------------------

	class pattern final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::pattern;


			// -- private members ---------------------------------------------

			/* tracks */
			ml::track_pool _tracks;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			pattern(void)
			: _tracks{} {
			}


			// -- public methods ----------------------------------------------

			auto play(cm::eventlist& evs, const ml::usz& timeline) -> void {
				_tracks.play(evs, timeline);
			}


			// -- public modifiers --------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void {
				_tracks.reset();
			}

	}; // class pattern

} // namespace ml

#endif // ml_data_pattern_hpp
