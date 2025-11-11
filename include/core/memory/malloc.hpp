#ifndef core_memory_malloc_hpp
#define core_memory_malloc_hpp

#include "diagnostics/system_error.hpp"
#include "core/types.hpp"

#include <stdlib.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* malloc */
	template <typename T>
	auto allocate(const mx::usz size = 1U) -> T* {

		void* ptr = ::malloc(size * sizeof(T));

		if (ptr == nullptr)
			throw ml::system_error{"malloc"};

		return static_cast<T*>(ptr);
	}

	/* calloc */
	template <typename T>
	auto callocate(const mx::usz size = 1U) -> T* {

		void* ptr = ::calloc(size, sizeof(T));

		if (ptr == nullptr)
			throw ml::system_error{"calloc"};

		return static_cast<T*>(ptr);
	}

	/* realloc */
	template <typename T>
	auto reallocate(T* old_ptr, const mx::usz size) -> T* {

		void* new_ptr = ::realloc(old_ptr, size * sizeof(T));

		if (new_ptr == nullptr)
			throw ml::system_error{"realloc"};

		return static_cast<T*>(new_ptr);
	}

	/* free */
	template <typename T>
	auto deallocate(T* ptr) noexcept -> void {

		::free(ptr);
	}

} // namespace mx

#endif // core_memory_malloc_hpp
