#include "language/ast/value_iterator.hpp"
#include "language/ast/tree.hpp"


// -- V A L U E  I T E R A T O R ----------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* constructor */
as::value_iterator::value_iterator(const as::tree& tree, const mx::usz vi) noexcept
: _tree{&tree}, _vi{vi} {
}


// -- public operators --------------------------------------------------------

/* [] operator */
auto as::value_iterator::operator[](const mx::isz index) const noexcept -> const as::value& {
	return _tree->_values[_vi + index];
}

/* dereference operator */
auto as::value_iterator::operator*(void) const noexcept -> const as::value& {
	return _tree->_values[_vi];
}

/* pre-increment operator */
auto as::value_iterator::operator++(void) noexcept -> self& {
	++_vi;
	return *this;
}

/* equality operators */
auto as::value_iterator::operator==(const self& other) const noexcept -> bool {
	return _vi == other._vi;
}

/* inequality operators */
auto as::value_iterator::operator!=(const self& other) const noexcept -> bool {
	return _vi != other._vi;
}
