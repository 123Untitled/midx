#ifndef core_traits_categories_hpp
#define core_traits_categories_hpp

#include "relations.hpp"
#include "transformations.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- I S  V O I D --------------------------------------------------------

	/* is void */
	template <typename T>
	concept is_void = mx::is_same<mx::remove_cv<T>, void>;


	// -- I S  C H A R --------------------------------------------------------

	template <typename T>
	concept is_character = mx::is_one_of<mx::remove_cv<T>,
		char, wchar_t, char8_t, char16_t, char32_t>;


	// -- I S  B O O L --------------------------------------------------------

	/* is bool */
	template <typename T>
	concept is_bool = mx::is_same<mx::remove_cv<T>, bool>;


	// -- I S  I N T E G R A L ------------------------------------------------

	/* is integral */
	template <typename T>
	concept is_integral = mx::is_one_of<mx::remove_cv<T>,
		char, wchar_t, char8_t, char16_t, char32_t,
		signed char, unsigned char,
		short, unsigned short,
		int, unsigned int,
		long, unsigned long,
		long long, unsigned long long,
		bool>;


	// -- I S  F L O A T I N G  P O I N T -------------------------------------

	/* is floating point */
	template <typename T>
	concept is_floating_point = mx::is_one_of<mx::remove_cv<T>,
		float, double, long double>;


	// -- I S  A R I T H M E T I C --------------------------------------------

	/* is arithmetic */
	template <typename T>
	concept is_arithmetic = mx::is_integral<T> || mx::is_floating_point<T>;


	// -- I S  A R R A Y ------------------------------------------------------

	namespace impl {

		/* is array */
		template <typename T>
		struct is_array final {
			static constexpr bool value = false;
			non_instantiable_class(is_array);
		};

		/* is array unbounded specialization */
		template <typename T>
		struct is_array<T[]> final {
			static constexpr bool value = true;
			non_instantiable_class(is_array);
		};

		/* is array bounded specialization */
		template <typename T, decltype(sizeof(0)) N>
		struct is_array<T[N]> final {
			static constexpr bool value = true;
			non_instantiable_class(is_array);
		};

	} // namespace impl


	/* is array */
	template <typename T>
	concept is_array = mx::impl::is_array<T>::value;

	/* is array of */
	template <typename T, typename U>
	concept is_array_of = mx::is_array<T> && mx::is_same<mx::remove_cve<T>, U>;


	// -- I S  P O I N T E R --------------------------------------------------

	namespace impl {

		/* is pointer */
		template <typename T>
		struct is_pointer {
			static constexpr bool value = false;
			non_instantiable_class(is_pointer);
		};

		/* pointer specialisation */
		template <typename T>
		struct is_pointer<T*> {
			static constexpr bool value = true;
			non_instantiable_class(is_pointer);
		};

		/* helper */
		template <typename T>
		struct is_pointer_helper final : public mx::impl::is_pointer<mx::remove_cv<T>> {
			non_instantiable_class(is_pointer_helper);
		};

	} // namespace impl


	/* is pointer */
	template<typename T>
	concept is_pointer = mx::impl::is_pointer_helper<T>::value;


	/* is pointer of */
	template <typename T, typename U>
	concept is_pointer_of = mx::is_pointer<T> && mx::is_same<mx::remove_cvp<T>, U>;


	// -- I S  L V A L U E  R E F E R E N C E ---------------------------------

	namespace impl {

		/* is lvalue reference */
		template <typename T>
		struct is_lvalue_reference final {
			static constexpr bool value = false;
			non_instantiable_class(is_lvalue_reference);
		};

		/* lvalue reference specialisation */
		template <typename T>
		struct is_lvalue_reference<T&> final {
			static constexpr bool value = true;
			non_instantiable_class(is_lvalue_reference);
		};

	} // namespace impl


	/* is lvalue reference */
	template <typename T>
	concept is_lvalue_reference = mx::impl::is_lvalue_reference<T>::value;

} // namespace mx

#endif // core_traits_categories_hpp
