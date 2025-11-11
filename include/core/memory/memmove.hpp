#ifndef core_memory_memmove_hpp
#define core_memory_memmove_hpp

#include "core/types.hpp"
#include <string.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M E M M O V E -------------------------------------------------------

	/* memmove */
	template <typename T>
	auto memmove(T* dst, const T* src, const mx::usz size) noexcept -> void {
		static_cast<void>(::memmove(dst, src, size * sizeof(T)));
	}

} // namespace mx

#endif // core_memory_memmove_hpp
