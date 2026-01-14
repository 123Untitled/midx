#ifndef core_traits_sequences_hpp
#define core_traits_sequences_hpp

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- I N D E X  S E Q U E N C E ------------------------------------------

	template <unsigned... Is>
	class index_sequence final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::index_sequence<Is...>;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr index_sequence(void) noexcept = default;

			/* copy constructor */
			constexpr index_sequence(const self&) noexcept = default;

			/* move constructor */
			constexpr index_sequence(self&&) noexcept = default;

			/* destructor */
			constexpr ~index_sequence(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto constexpr operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto constexpr operator=(self&&) noexcept -> self& = default;

	}; // class index_sequence



	// -- M A K E  I N D E X  S E Q U E N C E ---------------------------------

	namespace impl {


		template <unsigned N>
		class make_index_sequence final {


			private:

				/* forward declaration */
				template <unsigned...>
				struct list;

				/* end of recursion */
				template <unsigned I, unsigned... S> requires (I == 0U)
				struct list<I, S...> {
					using type = mx::index_sequence<S...>;
					non_instantiable_class(list);
				};

				/* recursive call */
				template <unsigned I, unsigned... S> requires (I > 0U)
				struct list<I, S...> {
					using type = typename list<I - 1U, I - 1U, S...>::type;
					non_instantiable_class(list);
				};


			public:

				// -- public lifecycle ----------------------------------------

				/* non instantiable class */
				non_instantiable_class(make_index_sequence);


				// -- public types --------------------------------------------

				/* type of the integer sequence */
				using type = typename list<N>::type;

		}; // class make_index_sequence

	} // namespace impl


	/* make index sequence */
	template <unsigned N>
	using make_index_sequence = typename mx::impl::make_index_sequence<N>::type;

} // namespace mx

#endif // core_traits_sequences_hpp
