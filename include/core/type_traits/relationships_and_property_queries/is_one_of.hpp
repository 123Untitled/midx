#pragma once

#include "core/type_traits/relationships_and_property_queries/is_same.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- I S  O N E  O F -----------------------------------------------------

	/* is one of */
	template <typename T, typename... Ts>
	concept is_one_of = (ms::is_same<T, Ts> || ...);

} // namespace ms
