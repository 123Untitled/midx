#ifndef core_traits_properties_hpp
#define core_traits_properties_hpp


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- I S  E M P T Y ------------------------------------------------------

	/* is empty */
	template <typename T>
	concept is_empty = __is_empty(T);

} // namespace mx

#endif // core_traits_properties_hpp

