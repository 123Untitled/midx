#ifndef core_types_hpp
#define core_types_hpp

#include "core/type_traits/integers.hpp"
#include <cstddef>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- integer types -------------------------------------------------------

	/* 8-bit types */
	using i8  = mx::sint_of<8U>;
	using u8  = mx::uint_of<8U>;

	/* 16-bit types */
	using i16 = mx::sint_of<16U>;
	using u16 = mx::uint_of<16U>;

	/* 32-bit types */
	using i32 = mx::sint_of<32U>;
	using u32 = mx::uint_of<32U>;

	/* 64-bit types */
	using i64 = mx::sint_of<64U>;
	using u64 = mx::uint_of<64U>;

	/* pointer-sized types */
	using usz = mx::uint_of<mx::bits<void*>>;
	using isz = mx::sint_of<mx::bits<void*>>;

	/* native types */
	using sint = mx::sint_of<mx::bits<signed>>;
	using uint = mx::uint_of<mx::bits<unsigned>>;


	// -- floating-point types ------------------------------------------------

	/* 32-bit types */
	using f32 = float;

	/* 64-bit types */
	using f64 = double;


	// -- max align type ------------------------------------------------------

	using max_align = std::max_align_t;


} // namespace mx

#endif // core_types_hpp
