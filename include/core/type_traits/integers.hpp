#ifndef core_type_traits_integers_hpp
#define core_type_traits_integers_hpp

#include "core/type_traits/type_categories/is_void.hpp"
#include "core/type_traits/type_categories/is_integral.hpp"
#include "core/type_traits/type_transformations/conditional.hpp"


#include <type_traits>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* bits per byte */
	constexpr auto bits_per_byte = [](void) static constexpr noexcept -> auto {
		decltype(sizeof(0)) count = 0U;
		for (unsigned char byte = 1; byte != 0; byte <<= 1)
			++count;
		return count;
	}();

	/* bits */
	template <typename T>
	constexpr auto bits = sizeof(T) * mx::bits_per_byte;


	// -- I N T E G E R  S E L E C T O R --------------------------------------

	namespace impl {


		/* integer selector
		* This class is used to select the appropriate integer type based on the
		* number of bits specified. It uses template specialization to find the
		* correct signed and unsigned types for the given bit size.
		*/

		template <decltype(mx::bits<char>) B>
		class integer_selector final {


			private:

				// -- private types -------------------------------------------

				/* self type */
				using self = mx::impl::integer_selector<B>;


				// -- private static members ----------------------------------

				/* is equal */
				template <typename T>
				static constexpr bool is_equal = (mx::bits<T> == B);


				// -- private structs -----------------------------------------

				/* forward declaration */
				template <typename...>
				struct list;

				/* recursion specialization */
				template <typename T, typename... Ts>
				struct list<T, Ts...> final {
					using type = mx::conditional<is_equal<T>, T, typename list<Ts...>::type>;
					non_instantiable(list);
				};

				/* end of recursion specialization */
				template <typename T>
				struct list<T> final {
					using type = mx::conditional<is_equal<T>, T, void>;
					non_instantiable(list);
				};


			public:

				// -- public types --------------------------------------------

				/* signed type */
				using signed_type = typename list<signed char,
												  signed short,
												  signed int,
												  signed long,
												  signed long long>::type;

				/* unsigned type */
				using unsign_type = typename list<unsigned char,
												  unsigned short,
												  unsigned int,
												  unsigned long,
												  unsigned long long>::type;


				// -- public lifecycle ----------------------------------------

				/* non-instantiable class */
				non_instantiable(integer_selector);


		}; // class integer_selector

	} // namespace impl


	/* integer selector */
	template <decltype(mx::bits<char>) B>
	class integer_selector final { 


		public:

			// -- public types ------------------------------------------------

			/* signed type */
			using signed_type = typename mx::impl::integer_selector<B>::signed_type;

			/* unsigned type */
			using unsign_type = typename mx::impl::integer_selector<B>::unsign_type;


			// -- public lifecycle --------------------------------------------

			/* non-instantiable class */
			non_instantiable(integer_selector);


			// -- assertions --------------------------------------------------

			static_assert(!mx::is_void<signed_type>
					   && !mx::is_void<unsign_type>,
					   "this machine does not support this integer type.");

	}; // class integer_selector


	/* uint of */
	template <unsigned B>
	using uint_of = typename mx::integer_selector<B>::unsign_type;

	/* sint of */
	template <unsigned B>
	using sint_of = typename mx::integer_selector<B>::signed_type;

	/* wider integral */
	template <ms::is_integral T>
	using wider_integral = mx::conditional<std::is_signed_v<T>,
								typename mx::integer_selector<mx::bits<T> * 2U>::signed_type,
								typename mx::integer_selector<mx::bits<T> * 2U>::unsign_type>;

	/* narrower integral */
	template <ms::is_integral T>
	using narrower_integral = mx::conditional<std::is_signed_v<T>,
								typename mx::integer_selector<mx::bits<T> / 2U>::signed_type,
								typename mx::integer_selector<mx::bits<T> / 2U>::unsign_type>;

} // namespace mx

#endif // core_type_traits_integers_hpp
