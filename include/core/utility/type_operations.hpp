#pragma once

#include "core/type_traits/traits.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	/* forward (lvalue) */
	template <typename T>
	constexpr auto forward(ms::to_lvalue<T> arg) noexcept -> T&& {
		return static_cast<T&&>(arg);
	}

	/* forward (rvalue) */
	template <typename T>
	constexpr auto forward(ms::to_rvalue<T> arg) noexcept -> T&& {
		static_assert(!ms::is_lvalue_reference<T>, "forward<T> requires an rvalue reference type");
		return static_cast<T&&>(arg);
	}

	/* move */
	template <typename T>
	constexpr auto move(T&& arg) noexcept -> ms::to_rvalue<T> {
		return static_cast<ms::to_rvalue<T>>(arg);
	}

} // namespace ms
