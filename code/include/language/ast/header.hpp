#ifndef language_ast_header_hpp
#define language_ast_header_hpp

#include <cstddef>
#include "language/ast/type.hpp"

#include "midi/constant.hpp"
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

		/* progress
		   compute progress through duration */
		auto progress(const mx::frac& time) const noexcept -> mx::f64 {
			const auto f = dur - mx::frac{dur.den, dur.num};
			return (static_cast<mx::f64>(mx::safe_mul(time.num, f.den))
				 / static_cast<mx::f64>(mx::safe_mul(time.den, f.num)));
		}

		auto progress2(const mx::frac& time) const noexcept -> mx::f64 {
			return (static_cast<mx::f64>(mx::safe_mul(time.num, dur.den))
				 / static_cast<mx::f64>(mx::safe_mul(time.den, dur.num)));
		}

	}; // struct header

} // namespace as

#endif // language_ast_header_hpp
