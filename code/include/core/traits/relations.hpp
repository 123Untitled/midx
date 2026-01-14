#ifndef core_traits_relations_hpp
#define core_traits_relations_hpp

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- I S  S A M E --------------------------------------------------------

	namespace impl {


		/* is same */
		template <typename, typename>
		struct is_same final {
			static constexpr bool value = false;
			non_instantiable_class(is_same);
		};

		/* true specialization */
		template <typename T>
		struct is_same<T, T> final {
			static constexpr bool value = true;
			non_instantiable_class(is_same);
		};

		template <typename T, typename... Ts>
		struct are_same final {
			static constexpr bool value = (is_same<T, Ts>::value && ...);
			non_instantiable_class(are_same);
		};

	} // namespace impl


	/* is same */
	template <typename T, typename U>
	concept is_same = mx::impl::is_same<T, U>::value;



	// -- I S  O N E  O F -----------------------------------------------------

	/* is one of */
	template <typename T, typename... Ts>
	concept is_one_of = (mx::is_same<T, Ts> || ...);


	// -- E X T E N T ---------------------------------------------------------

	namespace impl {


		/* extent for non-array types */
		template <typename type, unsigned = 0U>
		struct extent final {
			static constexpr unsigned value = 0U;
			non_instantiable_class(extent);
		};

		/* extent for unbounded array types (first dimension) */
		template <typename type>
		struct extent<type[], 0U> final {
			static constexpr unsigned value = 0U;
			non_instantiable_class(extent);
		};

		/* extent for unbounded array types (subsequent dimensions) */
		template <typename T, unsigned D>
		struct extent<T[], D> final {
			static constexpr unsigned value = mx::impl::extent<T, D - 1U>::value;
			non_instantiable_class(extent);
		};

		/* extent for bounded array types (first dimension) */
		template <typename T, unsigned N>
		struct extent<T[N], 0U> final {
			static constexpr unsigned value = N;
			non_instantiable_class(extent);
		};

		/* extent for bounded array types (subsequent dimensions) */
		template <typename T, unsigned N, unsigned D>
		struct extent<T[N], D> final {
			static constexpr unsigned value = mx::impl::extent<T, D - 1U>::value;
			non_instantiable_class(extent);
		};

	} // namespace impl


	/* extent */
	template <typename T, unsigned D = 0U>
	constexpr unsigned extent = mx::impl::extent<T, D>::value;

} // namespace mx

#endif // core_traits_relations_hpp
