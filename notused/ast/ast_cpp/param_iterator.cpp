#include "language/ast/param_iterator.hpp"


// -- P A R A M  I T E R A T O R ----------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* constructor */
as::param_iterator::param_iterator(const as::tree& tree, const mx::usz pi) noexcept
: _tree{&tree}, _pi{pi} {
}


// -- public operators --------------------------------------------------------

/* dereference operator */
auto as::param_iterator::operator*(void) const noexcept -> as::param_view {
	return as::param_view{*_tree, _pi};
}

/* pre-increment operator */
auto as::param_iterator::operator++(void) noexcept -> self& {
	++_pi;
	return *this;
}

/* equality operators */
auto as::param_iterator::operator==(const self& other) const noexcept -> bool {
	return _pi == other._pi;
}

/* inequality operators */
auto as::param_iterator::operator!=(const self& other) const noexcept -> bool {
	return _pi != other._pi;
}
