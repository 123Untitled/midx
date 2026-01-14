#ifndef type_transformations_conditional_hpp
#define type_transformations_conditional_hpp

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- C O N D I T I O N A L -----------------------------------------------

	namespace impl {

		/* forward declaration */
		template <bool, typename, typename>
		struct conditional;

		/* conditional true specialization */
		template <typename T, typename F>
		struct conditional<true, T, F> final {
			using type = T;
			non_instantiable_class(conditional);
		};

		/* conditional false specialization */
		template<typename T, typename F>
		struct conditional<false, T, F> final {
			using type = F;
			non_instantiable_class(conditional);
		};

	} // namespace impl


	/* conditional */
	template <bool B, typename T, typename F>
	using conditional = typename mx::impl::conditional<B, T, F>::type;

} // namespace mx

#endif // type_transformations_conditional_hpp
