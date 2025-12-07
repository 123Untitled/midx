#ifndef relationships_and_property_queries_is_one_of_hpp
#define relationships_and_property_queries_is_one_of_hpp

#include "core/type_traits/relationships_and_property_queries/is_same.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- I S  O N E  O F -----------------------------------------------------

	/* is one of */
	template <typename T, typename... Ts>
	concept is_one_of = (mx::is_same<T, Ts> || ...);

} // namespace mx

#endif // relationships_and_property_queries_is_one_of_hpp
