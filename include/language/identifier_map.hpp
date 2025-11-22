#ifndef language_identifier_map_hpp
#define language_identifier_map_hpp

#include "language/identifier_key.hpp"
#include <unordered_map>


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	struct ident_value final {
		mx::usz index;
		bool empty;
	};


	// -- I D E N T I F I E R  M A P ------------------------------------------

	class identifier_map final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::identifier_map;


			// -- private members ---------------------------------------------

			/* map */
			std::unordered_map<sx::identifier_key, sx::ident_value,
							   sx::identifier_key::hash> _map;

		public:


			// -- public modifiers --------------------------------------------

			/* insert */
			auto insert(const lx::lexeme& l, const mx::usz index) -> bool {
				const auto it = _map.try_emplace(sx::identifier_key{l},
												 sx::ident_value{index, index == 0U});
				return it.second;
			}

			/* find */
			auto find(const lx::lexeme& l) const noexcept -> sx::ident_value {

				const sx::identifier_key key{l};
				const auto it = _map.find(key);

				if (it == _map.end())
					return sx::ident_value{0U, false};

				return it->second;
			}


			/* clear */
			auto clear(void) noexcept -> void {
				_map.clear();
			}

	}; // class identifier_map

} // namespace sx

#endif // language_identifier_map_hpp
