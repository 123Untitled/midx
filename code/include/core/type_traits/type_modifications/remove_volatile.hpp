#ifndef type_modifications_remove_volatile_hpp
#define type_modifications_remove_volatile_hpp

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- R E M O V E  V O L A T I L E ----------------------------------------

	namespace impl {


		/* remove volatile */
		template <typename T>
		struct remove_volatile final {
			using type = T;
			non_instantiable_class(remove_volatile);
		};

		/* volatile specialization */
		template <typename T>
		struct remove_volatile<volatile T> final {
			using type = T;
			non_instantiable_class(remove_volatile);
		};

	} // namespace impl


	/* remove volatile */
	template <typename T>
	using remove_volatile = typename mx::impl::remove_volatile<T>::type;

} // namespace mx

#endif // type_modifications_remove_volatile_hpp
