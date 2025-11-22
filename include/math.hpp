#ifndef math_hpp
#define math_hpp

#include "core/types.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* gcd
	   compute the greatest common divisor of two numbers */
	constexpr auto gcd(mx::usz a, mx::usz b) noexcept -> mx::usz {
		while (b != 0U) {
			const mx::usz t = b;
			b = a % b;
			a = t;
		}
		return a;
	}


	/* lcm
	   compute the least common multiple of two numbers */
	constexpr auto lcm(mx::usz a, mx::usz b) noexcept -> mx::usz {
		if (a == 0U || b == 0U)
			return 0U;
		return (a / mx::gcd(a, b)) * b;
	}


} // namespace mx

#endif // math_hpp
