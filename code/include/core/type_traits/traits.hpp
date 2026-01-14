#pragma once

#include "core/config/class.hpp"
#include "core/type_traits/type_modifications/remove_cv.hpp"
#include "core/type_traits/relationships_and_property_queries/is_same.hpp"
#include "core/type_traits/relationships_and_property_queries/is_one_of.hpp"
#include "core/type_traits/type_categories/is_integral.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- R E M O V E  R E F E R E N C E --------------------------------------

	namespace impl {


		/* remove reference */
		template <typename T>
		struct remove_reference final {
			using type = T;
			non_instantiable_class(remove_reference);
		};

		/* lvalue specialization */
		template <typename T>
		struct remove_reference<T&> final {
			using type = T;
			non_instantiable_class(remove_reference);
		};

		/* rvalue specialization */
		template <typename T>
		struct remove_reference<T&&> final {
			using type = T;
			non_instantiable_class(remove_reference);
		};

	} // namespace impl


	/* remove reference */
	template <typename T>
	using remove_reference = typename mx::impl::remove_reference<T>::type;






	// -- R E M O V E  P O I N T E R ------------------------------------------

	namespace impl {


		/* remove pointer */
		template <typename T>
		struct remove_pointer final {
			using type = T;
			non_instantiable_class(remove_pointer);
		};

		/* pointer specialization */
		template <typename T>
		struct remove_pointer<T*> final {
			using type = T;
			non_instantiable_class(remove_pointer);
		};

		/* const pointer specialization */
		template <typename T>
		struct remove_pointer<T* const> final {
			using type = T;
			non_instantiable_class(remove_pointer);
		};

		/* volatile pointer specialization */
		template <typename T>
		struct remove_pointer<T* volatile> final {
			using type = T;
			non_instantiable_class(remove_pointer);
		};

		/* const volatile pointer specialization */
		template <typename T>
		struct remove_pointer<T* const volatile> final {
			using type = T;
			non_instantiable_class(remove_pointer);
		};

	} // namespace impl


	/* remove pointer */
	template <typename T>
	using remove_pointer = typename mx::impl::remove_pointer<T>::type;



	// -- R E M O V E  E X T E N T --------------------------------------------

	namespace impl {

		/* remove extent */
		template <typename T>
		struct remove_extent final {
			using type = T;
			non_instantiable_class(remove_extent);
		};

		/* array with unknown bound specialization */
		template <typename T>
		struct remove_extent<T[]> final {
			using type = T;
			non_instantiable_class(remove_extent);
		};

		/* array with known bound specialization */
		template <typename T, decltype(sizeof(0)) N>
		struct remove_extent<T[N]> final {
			using type = T;
			non_instantiable_class(remove_extent);
		};

	} // namespace impl


	/* remove extent */
	template <typename T>
	using remove_extent = typename mx::impl::remove_extent<T>::type;






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


	// -- I S  B O O L --------------------------------------------------------

	/* is bool */
	template <typename T>
	concept is_bool = mx::is_same<mx::remove_cv<T>, bool>;




	// -- I S  F L O A T I N G  P O I N T -------------------------------------

	/* is floating point */
	template <typename T>
	concept is_floating_point = mx::is_one_of<mx::remove_cv<T>,
		float, double, long double>;


	// -- I S  A R I T H M E T I C --------------------------------------------

	/* is arithmetic */
	template <typename T>
	concept is_arithmetic = mx::is_integral<T> || mx::is_floating_point<T>;


	// -- I S  C H A R --------------------------------------------------------

	template <typename T>
	concept is_character = mx::is_one_of<mx::remove_cv<T>,
		char, wchar_t, char8_t, char16_t, char32_t>;




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



	// -- T O  L V A L U E  R E F E R E N C E ---------------------------------

	/* forward lvalue helper */
	template <typename T>
	using to_lvalue = mx::remove_reference<T>&;


	// -- T O  R V A L U E  R E F E R E N C E ---------------------------------

	/* forward rvalue helper */
	template <typename T>
	using to_rvalue = mx::remove_reference<T>&&;



} // namespace mx
