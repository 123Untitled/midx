#pragma once

#include "core/type_traits/type_modifications/remove_volatile.hpp"
#include "core/type_traits/type_modifications/remove_const.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- R E M O V E  C V ----------------------------------------------------

	/* remove const volatile */
	template <typename T>
	using remove_cv = ms::remove_volatile<ms::remove_const<T>>;

} // namespace ms
