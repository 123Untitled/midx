#ifndef core_math_math_hpp
#define core_math_math_hpp

#include "core/types.hpp"

// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	constexpr float pi = 3.14159265358979323846f;
	constexpr float deg_to_rad = mx::pi / 180.0f;
	constexpr float rad_to_deg = 180.0f / mx::pi;



	inline auto trunc(const mx::f32 value) noexcept -> mx::f32 {
		// remove the fractional part of the float value
		return static_cast<mx::f32>(static_cast<mx::i32>(value));
	}

	inline auto trunc(const mx::f64 value) noexcept -> mx::f64 {
		// remove the fractional part of the double value
		return static_cast<mx::f64>(static_cast<mx::i64>(value));
	}

	inline auto fmod(const mx::f32 value, const mx::f32 divisor) noexcept -> mx::f32 {
		// calculate the remainder of the division
		return value - (mx::trunc(value / divisor) * divisor);
	}


	//template <typename T>
	inline auto abs(float value) noexcept -> float {
		mx::u32* ptr = reinterpret_cast<mx::u32*>(&value);
		*ptr &= 0x7FFFFFFF; // clear the sign bit
		return value;
	}

	constexpr auto pow10(const mx::usz exp) noexcept -> mx::usz {
		mx::usz result = 1U;
		for (mx::usz i = 0U; i < exp; ++i)
			result *= 10U;
		return result;
	}

} // namespace mx

#endif // core_math_math_hpp
