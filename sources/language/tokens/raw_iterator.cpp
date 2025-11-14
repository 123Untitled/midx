#include "language/tokens/raw_iterator.hpp"
#include "language/tokens/token_list.hpp"


// -- R A W  I T E R A T O R  -------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* base constructor */
tk::raw_iterator::raw_iterator(tk::tokens& tokens) noexcept
: _tokens{&tokens}, _index{0U} {
}

/* constructor */
tk::raw_iterator::raw_iterator(tk::tokens& tokens,
							 const mx::usz index) noexcept
: _tokens{&tokens}, _index{index} {
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
tk::raw_iterator::raw_iterator(void) noexcept
:  _tokens{nullptr}, _index{0U} {
}


// -- public operators --------------------------------------------------------

/* pre-increment operator */
auto tk::raw_iterator::operator++(void) noexcept -> self& {
	++_index;
	return *this;
}

/* post-increment operator */
auto tk::raw_iterator::operator++(int) noexcept -> self {
	self tmp{*this};
	++_index;
	return tmp;
}


/* const dereference operator */
auto tk::raw_iterator::operator*(void) const noexcept -> tk::raw::token& {
	return _tokens->_raw[_index];
}


/* equality operator */
auto tk::raw_iterator::operator==(const self& other) const noexcept -> bool {
	return _index == other._index;
}

/* inequality operator */
auto tk::raw_iterator::operator!=(const self& other) const noexcept -> bool {
	return _index != other._index;
}


/* const [] operator */
auto tk::raw_iterator::operator[](const mx::isz offset) const noexcept -> tk::raw::token& {
	return _tokens->_raw[_index + offset];
}


// -- public iterators --------------------------------------------------------

/* const begin */
auto tk::raw_iterator::begin(void) const noexcept -> self {
	return tk::raw_iterator{*_tokens};
}

/* const end */
auto tk::raw_iterator::end(void) const noexcept -> self {
	return tk::raw_iterator{*_tokens, _tokens->_raw.size()};
}


// -- public accessors --------------------------------------------------------

/* token */
auto tk::raw_iterator::token(void) const noexcept -> tk::raw::token& {
	return _tokens->_raw[_index];
}



// -- C O N S T  R A W  I T E R A T O R  --------------------------------------

// -- private lifecycle -------------------------------------------------------

/* base constructor */
tk::const_raw_iterator::const_raw_iterator(const tk::tokens& tokens) noexcept
: _tokens{&tokens}, _index{0U} {
}

/* constructor */
tk::const_raw_iterator::const_raw_iterator(const tk::tokens& tokens,
										   const mx::usz index) noexcept
: _tokens{&tokens}, _index{index} {
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
tk::const_raw_iterator::const_raw_iterator(void) noexcept
:  _tokens{nullptr}, _index{0U} {
}


// -- public operators --------------------------------------------------------

/* pre-increment operator */
auto tk::const_raw_iterator::operator++(void) noexcept -> self& {
	++_index;
	return *this;
}

/* post-increment operator */
auto tk::const_raw_iterator::operator++(int) noexcept -> self {
	self tmp{*this};
	++_index;
	return tmp;
}


/* const dereference operator */
auto tk::const_raw_iterator::operator*(void) const noexcept -> const tk::raw::token& {
	return _tokens->_raw[_index];
}


/* equality operator */
auto tk::const_raw_iterator::operator==(const self& other) const noexcept -> bool {
	return _index == other._index;
}

/* inequality operator */
auto tk::const_raw_iterator::operator!=(const self& other) const noexcept -> bool {
	return _index != other._index;
}


/* const [] operator */
auto tk::const_raw_iterator::operator[](const mx::isz offset) const noexcept -> const tk::raw::token& {
	return _tokens->_raw[_index + offset];
}


// -- public iterators --------------------------------------------------------

/* const begin */
auto tk::const_raw_iterator::begin(void) const noexcept -> self {
	return tk::const_raw_iterator{*_tokens};
}

/* const end */
auto tk::const_raw_iterator::end(void) const noexcept -> self {
	return tk::const_raw_iterator{*_tokens, _tokens->_raw.size()};
}


// -- public accessors --------------------------------------------------------

/* token */
auto tk::const_raw_iterator::token(void) const noexcept -> const tk::raw::token& {
	return _tokens->_raw[_index];
}
