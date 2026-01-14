#ifndef core_utility_exchange_hpp
#define core_utility_exchange_hpp

#include "core/utility/type_operations.hpp"
#include <utility>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* exchange */
	template <typename T, typename U = T>
	constexpr auto exchange(T& obj, U&& new_value)
		noexcept(std::is_nothrow_move_constructible<T>::value
			  && std::is_nothrow_assignable<T&, U>::value) -> T {

			T old = mx::move(obj);
			  obj = mx::forward<U>(new_value);
			return old;
	}

} // namespace mx

#endif // core_utility_exchange_hpp
