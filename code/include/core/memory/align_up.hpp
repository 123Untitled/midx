#ifndef core_memory_align_up_hpp
#define core_memory_align_up_hpp

#include "core/types.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* align up
		align a size up to the next multiple of align */
	template <typename T>
	auto align_up(const mx::usz size) noexcept -> mx::usz {
		constexpr mx::usz align = alignof(T);
		return (size + (align - 1U)) & ~(align - 1U);
	}

} // namespace mx

#endif // core_memory_align_up_hpp
