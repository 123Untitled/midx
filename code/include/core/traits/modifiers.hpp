#ifndef core_traits_modifiers_hpp
#define core_traits_modifiers_hpp

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- R E M O V E  C O N S T ----------------------------------------------

	namespace impl {


		/* remove const */
		template <typename T>
		struct remove_const final {
			using type = T;
			non_instantiable_class(remove_const);
		};

		/* const specialization */
		template <typename T>
		struct remove_const<const T> final {
			using type = T;
			non_instantiable_class(remove_const);
		};

	}; // namespace impl


	/* remove const */
	template <typename T>
	using remove_const = typename mx::impl::remove_const<T>::type;


	// -- R E M O V E  V O L A T I L E ----------------------------------------

	namespace impl {


		/* remove volatile */
		template <typename T>
		struct remove_volatile final {
			using type = T;
			non_instantiable_class(remove_volatile);
		};

		/* volatile specialization */
		template <typename T>
		struct remove_volatile<volatile T> final {
			using type = T;
			non_instantiable_class(remove_volatile);
		};

	} // namespace impl


	/* remove volatile */
	template <typename T>
	using remove_volatile = typename mx::impl::remove_volatile<T>::type;


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


	// -- R E M O V E  C V ----------------------------------------------------

	/* remove const volatile */
	template <typename T>
	using remove_cv = mx::remove_volatile<mx::remove_const<T>>;

} // namespace mx

#endif // core_traits_modifiers_hpp
