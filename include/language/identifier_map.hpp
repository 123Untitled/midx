#ifndef language_identifier_map_hpp
#define language_identifier_map_hpp

#include "language/identifier_key.hpp"
#include "language/ast/value.hpp"
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
			auto insert(const as::block& b, const ml::usz index) -> bool {
				const auto it = _map.try_emplace(sx::identifier_key{b}, index);
				return it.second;
			}

			/* insert */
			auto insert(const sp::id spec_id,
						const lx::lexeme& lex,
						const ml::usz index) -> bool {
				const auto it = _map.try_emplace(
						sx::identifier_key{spec_id, lex}, index);
				return it.second;
			}

			/* find */
			auto find(const as::param& p, const as::value& v, mx::usz& index) const noexcept -> sp::id {
				const sp::id spec_id = p.param_to_spec();
				const lx::lexeme& lex = v.token().lexeme;

				const sx::identifier_key key{spec_id, lex};
				const auto it = _map.find(key);

				if (it == _map.end())
					return sp::id::invalid;

				index = it->second;
				return spec_id;
			}


			/* clear */
			auto clear(void) noexcept -> void {
				_map.clear();
			}

	}; // class identifier_map

} // namespace sx

#endif // language_identifier_map_hpp
