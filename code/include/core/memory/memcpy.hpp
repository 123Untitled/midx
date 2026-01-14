#ifndef core_memory_memcpy_hpp
#define core_memory_memcpy_hpp

#include "core/types.hpp"
#include <string.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M E M C P Y ---------------------------------------------------------

	/* memcpy */
	template <typename T>
	constexpr auto memcpy(T* dst, const T* src, const mx::usz& size) noexcept -> void {

		// manual copy if consteval evaluation
		if consteval {
			for (mx::usz i = 0U; i < size; ++i)
				dst[i] = src[i];
		}
		else {
			static_cast<void>(::memcpy(dst, src, size * sizeof(T)));
		}
	}

	/* copy safe */
	template <typename T>
	constexpr auto memcpy(T* dst, T* end, const T* src, const mx::usz count) noexcept -> mx::usz {
		const auto left = static_cast<mx::usz>(end - dst);
		const auto size = count < left ? count : left;
		mx::memcpy(dst, src, size);
		return size;
	}

} // namespace mx

#endif // core_memory_memcpy_hpp
