#pragma once


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- T Y P E  A T --------------------------------------------------------

	namespace impl {


		template <unsigned N, typename... Tp>
		class type_at final {


			// -- assertions --------------------------------------------------

			/* requires non-empty parameter pack */
			static_assert(sizeof...(Tp) > 0, "type_at requires at least one type");

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
	using type_at = typename ms::impl::type_at<I, Tp...>::type;

} // namespace ms
