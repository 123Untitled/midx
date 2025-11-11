#pragma once

#include <utility>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	/* exchange */
	template <class T, class U = T>
	constexpr auto exchange(T& obj, U&& new_value)
		noexcept(std::is_nothrow_move_constructible<T>::value
			  && std::is_nothrow_assignable<T&, U>::value) -> T {

			T old_value = std::move(obj);
			obj = std::forward<U>(new_value);
			return old_value;
	}

} // namespace ms
