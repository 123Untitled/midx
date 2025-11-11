#pragma once

#include "core/type_traits/relationships_and_property_queries/is_same.hpp"
#include "core/type_traits/type_modifications/remove_cv.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- I S  V O I D --------------------------------------------------------

	/* is void */
	template <typename T>
	concept is_void = ms::is_same<ms::remove_cv<T>, void>;

} // namespace ms
