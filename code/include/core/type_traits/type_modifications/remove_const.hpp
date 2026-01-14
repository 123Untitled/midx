#ifndef type_modifications_remove_const_hpp
#define type_modifications_remove_const_hpp

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- R E M O V E  C O N S T ----------------------------------------------

	namespace impl {


		/* remove const */
		template <typename T>
		struct remove_const final {
			using type = T;
			non_instantiable_class(remove_const);
		};

		/* const specialization */
		template <typename T>
		struct remove_const<const T> final {
			using type = T;
			non_instantiable_class(remove_const);
		};

	}; // namespace impl


	/* remove const */
	template <typename T>
	using remove_const = typename mx::impl::remove_const<T>::type;

} // namespace mx

#endif // type_modifications_remove_const_hpp
