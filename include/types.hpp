#ifndef types_hpp
#define types_hpp

#include <stdint.h>
#include <sys/types.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {

	using uint = unsigned int;
	using sint = signed   int;

	using u8 = ::uint8_t;
	using i8 = ::int8_t;
	using u16 = ::uint16_t;
	using i16 = ::int16_t;
	using u32 = ::uint32_t;
	using i32 = ::int32_t;
	using u64 = ::uint64_t;
	using i64 = ::int64_t;

	using usz = ::size_t;
	using isz = ::ssize_t;

} // namespace ml

#endif // types_hpp
