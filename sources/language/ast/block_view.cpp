#include "language/ast/block_view.hpp"
#include "language/ast/tree.hpp"


// -- B L O C K  V I E W ------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* constructor */
as::block_view::block_view(as::tree& tree, const mx::usz bi) noexcept
: _tree{&tree}, _bi{bi} {
}


// -- public iterators --------------------------------------------------------

/* begin */
auto as::block_view::begin(void) const noexcept -> as::param_iterator {
	return as::param_iterator{
		*_tree,
		_tree->_blocks[_bi].ps
	};
}

/* end */
auto as::block_view::end(void) const noexcept -> as::param_iterator {
	const auto& b = _tree->_blocks[_bi];
	return as::param_iterator{
		*_tree, b.ps + b.pc
	};
}


// -- public accessors --------------------------------------------------------

/* params */
auto as::block_view::params(void) noexcept -> self& {
	return *this;
}

/* block */
auto as::block_view::block(void) noexcept -> as::block& {
	return _tree->_blocks[_bi];
}

/* block index */
auto as::block_view::bi(void) const noexcept -> mx::usz {
	return _bi;
}

/* num params */
auto as::block_view::num_params(void) const noexcept -> mx::usz {
	return _tree->_blocks[_bi].pc;
}
