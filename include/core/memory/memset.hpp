#ifndef memset_hpp
#define memset_hpp

#include "core/types.hpp"
#include <string.h>




// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	constexpr auto bzero(void* ptr, const mx::usz size) noexcept -> void {

		if (__builtin_is_constant_evaluated()) {

			auto* p = static_cast<mx::u8*>(ptr);
			for (mx::usz i = 0U; i < size; ++i)
				p[i] = 0U;
		}

		else {
			static_cast<void>(::memset(ptr, 0, size));
		}
	}

} // namespace mx

#endif // memset_hpp
