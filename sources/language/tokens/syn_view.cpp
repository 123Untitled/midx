#include "language/tokens/syn_view.hpp"
#include "language/tokens/token_list.hpp"
#include "language/tokens.hpp"


// -- S Y N  V I E W ----------------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* constructor */
tk::syn_view::syn_view(tk::tokens& tokens, const mx::usz index) noexcept
: _tokens{&tokens}, _index{index} {
}


// -- public iterators --------------------------------------------------------

/* begin */
auto tk::syn_view::begin(void) const noexcept -> tk::map_iterator {
	const auto& st = _tokens->_syn[_index];
	return tk::map_iterator{*_tokens, st.start};
}

/* end */
auto tk::syn_view::end(void) const noexcept -> tk::map_iterator {
	const auto& st = _tokens->_syn[_index];
	return tk::map_iterator{*_tokens, st.start + st.count};
}


// -- public accessors --------------------------------------------------------

/* token */
auto tk::syn_view::token(void) const noexcept -> tk::syn::token& {
	return _tokens->_syn[_index];
}



// -- C O N S T  S Y N  V I E W -----------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* constructor */
tk::const_syn_view::const_syn_view(const tk::tokens& tokens,
								   const mx::usz index) noexcept
: _tokens{&tokens}, _index{index} {
}


// -- public iterators --------------------------------------------------------

/* begin */
auto tk::const_syn_view::begin(void) const noexcept -> tk::const_map_iterator {
	const auto& st = _tokens->_syn[_index];
	return tk::const_map_iterator{*_tokens, st.start};
}

/* end */
auto tk::const_syn_view::end(void) const noexcept -> tk::const_map_iterator {
	const auto& st = _tokens->_syn[_index];
	return tk::const_map_iterator{*_tokens, st.start + st.count};
}


// -- public accessors --------------------------------------------------------

/* token */
auto tk::const_syn_view::token(void) const noexcept -> const tk::syn::token& {
	return _tokens->_syn[_index];
}
