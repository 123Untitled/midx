#pragma once

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- R E M O V E  C O N S T ----------------------------------------------

	namespace impl {


		/* remove const */
		template <typename T>
		struct remove_const final {
			using type = T;
			non_instantiable(remove_const);
		};

		/* const specialization */
		template <typename T>
		struct remove_const<const T> final {
			using type = T;
			non_instantiable(remove_const);
		};

	}; // namespace impl


	/* remove const */
	template <typename T>
	using remove_const = typename ms::impl::remove_const<T>::type;

} // namespace ms
