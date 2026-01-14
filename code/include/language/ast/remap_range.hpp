#ifndef language_ast_remap_range_hpp
#define language_ast_remap_range_hpp

#include "core/types.hpp"
#include <cstddef>


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- R E M A P  R A N G E ------------------------------------------------

	struct alignas(std::max_align_t) remap_range final {
		mx::usz start; // start index in arena
		mx::usz count; // count of nodes
		remap_range(void) noexcept
		: start{0U}, count{0U} {
		}
		remap_range(const mx::usz s,
					const mx::usz c) noexcept
		: start{s}, count{c} {
		}

		auto end(void) const noexcept -> mx::usz {
			return start + count;
		}

	}; // struct remap_range

} // namespace as

#endif // language_ast_remap_range_hpp
