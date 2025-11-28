#ifndef language_ast_header_hpp
#define language_ast_header_hpp

#include <cstddef>
#include "language/ast/type.hpp"
#include "math.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- H E A D E R ---------------------------------------------------------

	struct alignas(std::max_align_t) header final {

		as::type type;
		mx::frac dur;

		header(const as::type type,
			   const mx::frac& dur) noexcept
		: type{type}, dur{dur} {
		}

		/* mod */
		auto mod(const mx::frac& time) const noexcept -> mx::frac {
			return mx::frac_mod(time, dur);
		}

	}; // struct header

} // namespace as

#endif // language_ast_header_hpp
