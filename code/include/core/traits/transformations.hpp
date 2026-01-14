#ifndef core_traits_transformations_hpp
#define core_traits_transformations_hpp

#include "core/config/class.hpp"
#include "core/traits/modifiers.hpp"


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


	// -- T O  L V A L U E  R E F E R E N C E ---------------------------------

	/* forward lvalue helper */
	template <typename T>
	using to_lvalue_reference = mx::remove_reference<T>&;


	// -- T O  R V A L U E  R E F E R E N C E ---------------------------------

	/* forward rvalue helper */
	template <typename T>
	using to_rvalue_reference = mx::remove_reference<T>&&;


	// -- R E M O V E  C V R --------------------------------------------------

	/* remove const volatile reference */
	template <typename T>
	using remove_cvr = mx::remove_cv<mx::remove_reference<T>>;



	// -- R E M O V E  C V P --------------------------------------------------

	/* remove const volatile pointer */
	template <typename T>
	using remove_cvp = mx::remove_cv<mx::remove_pointer<T>>;


	// -- R E M O V E  C V E --------------------------------------------------

	/* remove const volatile extent */
	template <typename T>
	using remove_cve = mx::remove_cv<mx::remove_extent<T>>;


	// -- R E M O V E  C V P E ------------------------------------------------

	/* remove const volatile pointer extent */
	template <typename T>
	using remove_cvpe = mx::remove_cv<mx::remove_pointer<mx::remove_extent<T>>>;



	// -- T Y P E  A T --------------------------------------------------------

	namespace impl {


		template <unsigned N, typename... Tp>
		class type_at final {


			// -- assertions --------------------------------------------------

			/* requires non-empty parameter pack */
			static_assert(sizeof...(Tp) > 0U, "type_at requires at least one type");

			/* requires valid index */
			static_assert(N < sizeof...(Tp), "type_at index out of bounds");


			private:

				// -- private implementation ----------------------------------

				/* forward declaration */
				template <unsigned, typename...>
				struct list;

				/* end of recursion */
				template <unsigned I, typename T, typename... Ts> requires (I == N)
				struct list<I, T, Ts...> final {
					using type = T;
				};

				/* specialization for the next index */
				template <unsigned I, typename T, typename... Ts> requires (I < N)
				struct list<I, T, Ts...> final {
					using type = typename list<I + 1U, Ts...>::type;
				};


			public:

				// -- public types --------------------------------------------

				/* type indexed by IDX */
				using type = typename list<0U, Tp...>::type;

		}; // class type_at

	} // namespace impl


	/* type at */
	template <unsigned I, typename... Tp>
	using type_at = typename mx::impl::type_at<I, Tp...>::type;


} // namespace mx

#endif // core_traits_transformations_hpp
