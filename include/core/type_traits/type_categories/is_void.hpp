#ifndef core_type_traits_is_void_hpp
#define core_type_traits_is_void_hpp

#include "core/type_traits/relationships_and_property_queries/is_same.hpp"
#include "core/type_traits/type_modifications/remove_cv.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- I S  V O I D --------------------------------------------------------

	/* is void */
	template <typename T>
	concept is_void = mx::is_same<ms::remove_cv<T>, void>;

} // namespace mx

#endif // core_type_traits_is_void_hpp
