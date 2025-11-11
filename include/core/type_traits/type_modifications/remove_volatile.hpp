#pragma once

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- R E M O V E  V O L A T I L E ----------------------------------------

	namespace impl {


		/* remove volatile */
		template <typename T>
		struct remove_volatile final {
			using type = T;
			non_instantiable(remove_volatile);
		};

		/* volatile specialization */
		template <typename T>
		struct remove_volatile<volatile T> final {
			using type = T;
			non_instantiable(remove_volatile);
		};

	} // namespace impl


	/* remove volatile */
	template <typename T>
	using remove_volatile = typename ms::impl::remove_volatile<T>::type;

} // namespace ms
