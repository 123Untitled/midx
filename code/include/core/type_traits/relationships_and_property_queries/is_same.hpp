#ifndef relationships_and_property_queries_is_same_hpp
#define relationships_and_property_queries_is_same_hpp

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- I S  S A M E --------------------------------------------------------

	namespace impl {


		/* is same */
		template <typename, typename>
		struct is_same final {
			static constexpr bool value = false;
			non_instantiable(is_same);
		};

		/* true specialization */
		template <typename T>
		struct is_same<T, T> final {
			static constexpr bool value = true;
			non_instantiable(is_same);
		};

		template <typename T, typename... Ts>
		struct are_same final {
			static constexpr bool value = (is_same<T, Ts>::value && ...);
			non_instantiable(are_same);
		};

	} // namespace impl


	/* is same */
	template <typename T, typename U>
	concept is_same = mx::impl::is_same<T, U>::value;

} // namespace mx

#endif // relationships_and_property_queries_is_same_hpp
