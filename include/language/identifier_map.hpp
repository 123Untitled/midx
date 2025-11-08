#ifndef language_identifier_map_hpp
#define language_identifier_map_hpp

#include "language/identifier_key.hpp"
#include <unordered_map>


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- I D E N T I F I E R  M A P ------------------------------------------

	class identifier_map final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::identifier_map;


			// -- private members ---------------------------------------------

			/* map */
			std::unordered_map<sx::identifier_key, ml::usz,
							   sx::identifier_key::hash> _map;

		public:


			// -- public modifiers --------------------------------------------

			/* insert */
			auto insert(as::block& b, const ml::usz index) -> bool {
				const auto it = _map.try_emplace(sx::identifier_key{b}, index);
				return it.second;
			}


			/* clear */
			auto clear(void) noexcept -> void {
				_map.clear();
			}

	}; // class identifier_map

} // namespace sx

#endif // language_identifier_map_hpp
