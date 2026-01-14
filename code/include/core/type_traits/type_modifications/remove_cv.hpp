#ifndef type_modifications_remove_cv_hpp
#define type_modifications_remove_cv_hpp

#include "core/type_traits/type_modifications/remove_volatile.hpp"
#include "core/type_traits/type_modifications/remove_const.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- R E M O V E  C V ----------------------------------------------------

	/* remove const volatile */
	template <typename T>
	using remove_cv = mx::remove_volatile<mx::remove_const<T>>;

} // namespace mx

#endif // type_modifications_remove_cv_hpp
