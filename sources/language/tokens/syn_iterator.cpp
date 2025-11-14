#include "language/tokens/syn_iterator.hpp"
#include "language/tokens/token_list.hpp"


// -- S Y N  I T E R A T O R  -------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* base constructor */
tk::syn_iterator::syn_iterator(tk::tokens& tokens) noexcept
: _tokens{&tokens}, _index{0U} {
}

/* constructor */
tk::syn_iterator::syn_iterator(tk::tokens& tokens,
							 const mx::usz index) noexcept
: _tokens{&tokens}, _index{index} {
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
tk::syn_iterator::syn_iterator(void) noexcept
:  _tokens{nullptr}, _index{0U} {
}


// -- public operators --------------------------------------------------------

/* pre-increment operator */
auto tk::syn_iterator::operator++(void) noexcept -> self& {
	++_index;
	return *this;
}

/* post-increment operator */
auto tk::syn_iterator::operator++(int) noexcept -> self {
	self tmp{*this};
	++_index;
	return tmp;
}


/* const dereference operator */
auto tk::syn_iterator::operator*(void) const noexcept -> tk::syn_view {
	return tk::syn_view{*_tokens, _index};
}


/* equality operator */
auto tk::syn_iterator::operator==(const self& other) const noexcept -> bool {
	return _index == other._index;
}

/* inequality operator */
auto tk::syn_iterator::operator!=(const self& other) const noexcept -> bool {
	return _index != other._index;
}


/* const [] operator */
auto tk::syn_iterator::operator[](const mx::isz offset) const noexcept -> tk::syn_view {
	return tk::syn_view{*_tokens, _index + offset};
}


// -- public iterators --------------------------------------------------------

/* const begin */
auto tk::syn_iterator::begin(void) const noexcept -> self {
	return tk::syn_iterator{*_tokens};
}

/* const end */
auto tk::syn_iterator::end(void) const noexcept -> self {
	return tk::syn_iterator{*_tokens, _tokens->_syn.size()};
}



// -- C O N S T  S Y N  I T E R A T O R  --------------------------------------

// -- private lifecycle -------------------------------------------------------

/* base constructor */
tk::const_syn_iterator::const_syn_iterator(const tk::tokens& tokens) noexcept
: _tokens{&tokens}, _index{0U} {
}

/* constructor */
tk::const_syn_iterator::const_syn_iterator(const tk::tokens& tokens,
										   const mx::usz index) noexcept
: _tokens{&tokens}, _index{index} {
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
tk::const_syn_iterator::const_syn_iterator(void) noexcept
:  _tokens{nullptr}, _index{0U} {
}


// -- public operators --------------------------------------------------------

/* pre-increment operator */
auto tk::const_syn_iterator::operator++(void) noexcept -> self& {
	++_index;
	return *this;
}

/* post-increment operator */
auto tk::const_syn_iterator::operator++(int) noexcept -> self {
	self tmp{*this};
	++_index;
	return tmp;
}


/* const dereference operator */
auto tk::const_syn_iterator::operator*(void) const noexcept -> tk::const_syn_view {
	return tk::const_syn_view{*_tokens, _index};
}


/* equality operator */
auto tk::const_syn_iterator::operator==(const self& other) const noexcept -> bool {
	return _index == other._index;
}

/* inequality operator */
auto tk::const_syn_iterator::operator!=(const self& other) const noexcept -> bool {
	return _index != other._index;
}


/* const [] operator */
auto tk::const_syn_iterator::operator[](const mx::isz offset) const noexcept -> tk::const_syn_view {
	return tk::const_syn_view{*_tokens, _index + offset};
}


// -- public iterators --------------------------------------------------------

/* const begin */
auto tk::const_syn_iterator::begin(void) const noexcept -> self {
	return tk::const_syn_iterator{*_tokens};
}

/* const end */
auto tk::const_syn_iterator::end(void) const noexcept -> self {
	return tk::const_syn_iterator{*_tokens, _tokens->_syn.size()};
}
