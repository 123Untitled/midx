#ifndef ml_data_pattern_pool_hpp
#define ml_data_pattern_pool_hpp

#include "data/pattern.hpp"
#include <unordered_map>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- P A T T E R N  P O O L ----------------------------------------------

	class pattern_pool final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::pattern_pool;


			// -- private members ---------------------------------------------

			/* patterns */
			std::unordered_map<std::string, ml::pattern> _patterns;


		public:


			// -- public methods ----------------------------------------------


			// -- public modifiers --------------------------------------------

			/* find or create */
			auto find_or_create(const std::string& name) -> ml::pattern& {
				return _patterns[name];
			}

			/* reset */
			auto reset(void) noexcept -> void {
				for (auto& [_, p] : _patterns)
					p.reset();
			}

	}; // class pattern_pool

} // namespace ml

#endif // ml_data_pattern_pool_hpp
