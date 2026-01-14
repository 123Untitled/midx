#ifndef core_string_is_string_hpp
#define core_string_is_string_hpp

#include "core/config/class.hpp"
#include "core/traits/relations.hpp"
#include "core/traits/categories.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- I S  S T R I N G ----------------------------------------------------

	/* forward declaration */
	class string;


	/* is string */
	template <typename T>
	concept is_string = mx::is_same<T, mx::string>;



	// -- I S  S T R I N G  V I E W -------------------------------------------

	/* forward declaration */
	class string_view;

	/* is basic string view */
	template <typename T>
	concept is_string_view = mx::is_same<T, mx::string_view>;


	// -- I S  I N P L A C E  B A S I C  S T R I N G --------------------------

	/* inplace_basic_string */
	template <unsigned>
	class inplace_string;


	namespace impl {

		/* is inplace string false */
		template <typename>
		struct is_inplace_string final {
			static constexpr bool value = false;
			non_instantiable_class(is_inplace_string);
		};

		/* is inplace string true */
		template <unsigned N>
		struct is_inplace_string<mx::inplace_string<N>> final {
			static constexpr bool value = true;
			non_instantiable_class(is_inplace_string);
		};

	} // namespace impl


	/* is inplace string */
	template <typename T>
	concept is_inplace_string = mx::impl::is_inplace_string<T>::value;



	// -- I S  S T R I N G  C L A S S -----------------------------------------

	/* is string class */
	template <typename T>
	concept is_string_class = mx::is_string<T>
						   || mx::is_string_view<T>
						   || mx::is_inplace_string<T>;

	/* are string class */
	template <typename... Ts>
	concept are_string_class = (mx::is_string_class<Ts> && ...);


	// -- I S  S T R I N G  L I K E -------------------------------------------

	/* is string like */
	template <typename T>
	concept is_string_like = mx::is_string_class<T>
						  || mx::is_array_of<T, char>
						  || mx::is_pointer_of<T, char>;

} // namespace mx

#endif // core_string_is_string_hpp
