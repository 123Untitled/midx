#ifndef core_type_traits_conditional_hpp
#define core_type_traits_conditional_hpp

#include "macros.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- C O N D I T I O N A L -----------------------------------------------

	namespace impl {

		template <bool, typename, typename>
		struct conditional;

		template <typename T, typename F>
		struct conditional<true, T, F> final {
			using type = T;
			non_instantiable_class(conditional);
		};

		template <typename T, typename F>
		struct conditional<false, T, F> final {
			using type = F;
			non_instantiable_class(conditional);
		};

	} // namespace impl


	/* conditional */
	template <bool B, typename T, typename F>
	using conditional = typename mx::impl::conditional<B, T, F>::type;

} // namespace mx

#endif // core_type_traits_conditional_hpp
