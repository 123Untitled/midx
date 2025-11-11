#pragma once

#include "core/config/class.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- I S  B A S I C  S T R I N G -----------------------------------------

	/* forward declaration */
	template <typename>
	class basic_string;


	namespace impl {

		/* is basic string false */
		template <typename>
		struct is_basic_string final {
			static constexpr bool value = false;
			non_instantiable(is_basic_string);
		};

		/* is basic string true */
		template <typename C>
		struct is_basic_string<ms::basic_string<C>> final {
			static constexpr bool value = true;
			non_instantiable(is_basic_string);
		};

		/* is basic string of false */
		template <typename, typename>
		struct is_basic_string_of final {
			static constexpr bool value = false;
			non_instantiable(is_basic_string_of);
		};

		/* is basic string of true */
		template <typename C>
		struct is_basic_string_of<ms::basic_string<C>, C> final {
			static constexpr bool value = true;
			non_instantiable(is_basic_string_of);
		};

	} // namespace impl


	/* is basic string */
	template <typename T>
	concept is_basic_string = ms::impl::is_basic_string<T>::value;

	/* is basic string of */
	template <typename T, typename C>
	concept is_basic_string_of = ms::impl::is_basic_string_of<T, C>::value;


	// -- I S  B A S I C  S T R I N G  V I E W --------------------------------

	/* forward declaration */
	template <typename>
	class basic_string_view;


	namespace impl {

		/* is basic string view false */
		template <typename>
		struct is_basic_string_view final {
			static constexpr bool value = false;
			non_instantiable(is_basic_string_view);
		};

		/* is basic string view true */
		template <typename C>
		struct is_basic_string_view<ms::basic_string_view<C>> final {
			static constexpr bool value = true;
			non_instantiable(is_basic_string_view);
		};

		/* is basic string view of false */
		template <typename, typename>
		struct is_basic_string_view_of final {
			static constexpr bool value = false;
			non_instantiable(is_basic_string_view_of);
		};

		/* is basic string view of true */
		template <typename C>
		struct is_basic_string_view_of<ms::basic_string_view<C>, C> final {
			static constexpr bool value = true;
			non_instantiable(is_basic_string_view_of);
		};

	} // namespace impl


	/* is basic string view */
	template <typename T>
	concept is_basic_string_view = ms::impl::is_basic_string_view<T>::value;

	/* is basic string view of */
	template <typename T, typename C>
	concept is_basic_string_view_of = ms::impl::is_basic_string_view_of<T, C>::value;



	// -- I S  I N P L A C E  B A S I C  S T R I N G --------------------------

	/* inplace_basic_string */
	template <typename, unsigned>
	class inplace_basic_string;


	namespace impl {

		/* is inplace basic string false */
		template <typename>
		struct is_inplace_basic_string final {
			static constexpr bool value = false;
			non_instantiable(is_inplace_basic_string);
		};

		/* is inplace basic string true */
		template <typename C, unsigned N>
		struct is_inplace_basic_string<ms::inplace_basic_string<C, N>> final {
			static constexpr bool value = true;
			non_instantiable(is_inplace_basic_string);
		};

		/* is inplace basic string of false */
		template <typename, typename>
		struct is_inplace_basic_string_of final {
			static constexpr bool value = false;
			non_instantiable(is_inplace_basic_string_of);
		};

		/* is inplace basic string of true */
		template <unsigned N, typename C>
		struct is_inplace_basic_string_of<ms::inplace_basic_string<C, N>, C> final {
			static constexpr bool value = true;
			non_instantiable(is_inplace_basic_string_of);
		};

	} // namespace impl


	/* is inplace basic string */
	template <typename T>
	concept is_inplace_basic_string = ms::impl::is_inplace_basic_string<T>::value;

	/* is inplace basic string of */
	template <typename T, typename C>
	concept is_inplace_basic_string_of = ms::impl::is_inplace_basic_string_of<T, C>::value;



	// -- I S  S T R I N G  L I K E -------------------------------------------

	/* is string like */
	template <typename T>
	concept is_string_like = ms::is_basic_string<T>
						  || ms::is_basic_string_view<T>
						  || ms::is_inplace_basic_string<T>;

	/* are string like */
	template <typename... Ts>
	concept are_string_like = (ms::is_string_like<Ts> && ...);


	/* is string like of */
	template <typename T, typename C>
	concept is_string_like_of = ms::is_basic_string_of<T, C>
							 || ms::is_basic_string_view_of<T, C>
							 || ms::is_inplace_basic_string_of<T, C>;

	/* are string like of */
	template <typename C, typename... Ts>
	concept are_string_like_of = (ms::is_string_like_of<Ts, C> && ...);

} // namespace ms
