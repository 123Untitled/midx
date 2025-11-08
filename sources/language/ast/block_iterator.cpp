#include "language/ast/block_iterator.hpp"
#include "language/ast/tree.hpp"


// -- B L O C K  I T E R A T O R ----------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* constructor */
as::block_iterator::block_iterator(as::tree& tree,
								   const ml::usz bi) noexcept
: _tree{&tree}, _bi{bi} {
}


// -- public operators --------------------------------------------------------

/* dereference operator */
auto as::block_iterator::operator*(void) const noexcept -> as::block_view {
	return as::block_view{*_tree, _tree->_block_order[_bi]};
}

/* pre-increment operator */
auto as::block_iterator::operator++(void) noexcept -> self& {
	++_bi;
	return *this;
}

/* equality operators */
auto as::block_iterator::operator==(const self& other) const noexcept -> bool {
	return _bi == other._bi;
}

/* inequality operators */
auto as::block_iterator::operator!=(const self& other) const noexcept -> bool {
	return _bi != other._bi;
}
