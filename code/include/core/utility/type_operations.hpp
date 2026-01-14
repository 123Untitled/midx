#ifndef core_utility_type_operations_hpp
#define core_utility_type_operations_hpp

#include "core/traits/transformations.hpp"
#include "core/traits/categories.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* forward (lvalue) */
	template <typename T>
	constexpr auto forward(mx::to_lvalue_reference<T> arg) noexcept -> T&& {
		return static_cast<T&&>(arg);
	}

	/* forward (rvalue) */
	template <typename T>
	constexpr auto forward(mx::to_rvalue_reference<T> arg) noexcept -> T&& {
		static_assert(!mx::is_lvalue_reference<T>, "forward<T> requires an rvalue reference type");
		return static_cast<T&&>(arg);
	}

	/* move */
	template <typename T>
	constexpr auto move(T&& arg) noexcept -> mx::to_rvalue_reference<T> {
		return static_cast<mx::to_rvalue_reference<T>>(arg);
	}

} // namespace mx

#endif // core_utility_type_operations_hpp
