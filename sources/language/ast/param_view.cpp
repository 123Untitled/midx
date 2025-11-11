#include "language/ast/param_view.hpp"
#include "language/ast/tree.hpp"


// -- P A R A M  V I E W ------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* constructor */
as::param_view::param_view(const as::tree& tree, const ml::usz pi) noexcept
: _tree{&tree}, _pi{pi} {
}


// -- public iterators --------------------------------------------------------

/* begin */
auto as::param_view::begin(void) const noexcept -> as::value_iterator {
	const auto& p = _tree->_params[_pi];
	return as::value_iterator{*_tree, p.vs};
}

/* end */
auto as::param_view::end(void) const noexcept -> as::value_iterator {
	const auto& p = _tree->_params[_pi];
	return as::value_iterator{
		*_tree,
		p.vs + p.vc
	};
}


// -- public accessors --------------------------------------------------------

/* values */
auto as::param_view::values(void) const noexcept -> const self& {
	return *this;
}

/* param */
auto as::param_view::param(void) const noexcept -> const as::param& {
	return _tree->_params[_pi];
}

/* num values */
auto as::param_view::num_values(void) const noexcept -> ml::usz {
	const auto& p = _tree->_params[_pi];
	return p.vc;
}
