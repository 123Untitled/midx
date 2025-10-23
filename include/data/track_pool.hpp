#ifndef ml_data_track_pool_hpp
#define ml_data_track_pool_hpp

#include "data/track.hpp"
#include <unordered_map>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- T R A C K  P O O L --------------------------------------------------

	class track_pool final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::track_pool;


			// -- private members ---------------------------------------------

			/* tracks */
			std::unordered_map<std::string, ml::track> _tracks;


		public:

			// -- public methods ----------------------------------------------

			/* play */
			auto play(cm::eventlist& evs, const ml::usz& timeline) -> void {
				for (auto& [_, track] : _tracks)
					track.play(evs, timeline);
			}


			// -- public modifiers --------------------------------------------

			/* find or create */
			auto find_or_create(const std::string& name) -> ml::track& {
				return _tracks[name];
			}

			/* reset */
			auto reset(void) noexcept -> void {
				for (auto& [_, t] : _tracks)
					t.reset();
			}

	}; // class track_pool

} // namespace ml

#endif //ml_data_track_pool_hpp
