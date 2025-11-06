#include "language/ast/block_view.hpp"
#include "language/ast/tree.hpp"


// -- B L O C K  V I E W ------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* constructor */
as::block_view::block_view(const as::tree& tree, const ml::usz bi) noexcept
: _tree{&tree}, _bi{bi} {
}


// -- public iterators --------------------------------------------------------

/* begin */
auto as::block_view::begin(void) const noexcept -> as::param_iterator {
	return as::param_iterator{
		*_tree,
		_tree->_blocks[_bi]._params_start
	};
}

/* end */
auto as::block_view::end(void) const noexcept -> as::param_iterator {
	const auto& b = _tree->_blocks[_bi];
	return as::param_iterator{
		*_tree,
		b._params_start + b._params_count
	};
}


// -- public accessors --------------------------------------------------------

/* params */
auto as::block_view::params(void) const noexcept -> const self& {
	return *this;
}

/* block */
auto as::block_view::block(void) const noexcept -> const as::block& {
	return _tree->_blocks[_bi];
}
