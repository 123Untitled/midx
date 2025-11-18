#include "language/ast/value_view.hpp"
#include "language/ast/tree.hpp"


// -- V A L U E  V I E W ------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* constructor */
as::value_view::value_view(const as::tree& tree, const mx::usz vi) noexcept
: _tree{&tree}, _vi{vi} {
}


// -- public accessors --------------------------------------------------------

/* value */
auto as::value_view::value(void) const noexcept -> const as::value& {
	return _tree->_values[_vi];
}
