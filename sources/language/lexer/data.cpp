#include "language/lexer/data.hpp"
#include "language/lexer/context.hpp"



// -- D A T A -----------------------------------------------------------------

/* start range */
auto lx::data::start_range(const lx::context& ctx) -> void {
	_ranges.emplace_back(
			tk::range{
				ctx.line(),
				ctx.column(),
				0U, 0U
			}
	);
}

/* end range */
auto lx::data::end_range(const lx::context& ctx) -> ml::uint {
	tk::range& r = _ranges.back();
	r.row_tail = ctx.line();
	r.col_tail = ctx.column(); // maybe + 1U ?
	return static_cast<ml::uint>(_ranges.size() - 1U);
}
