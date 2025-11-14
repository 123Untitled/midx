#include "language/tokens/map_iterator.hpp"
#include "language/tokens/token_list.hpp"


// -- M A P  I T E R A T O R  -------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* base constructor */
tk::map_iterator::map_iterator(tk::tokens& tokens) noexcept
: _tokens{&tokens}, _index{0U} {
}

/* constructor */
tk::map_iterator::map_iterator(tk::tokens& tokens,
							 const mx::usz index) noexcept
: _tokens{&tokens}, _index{index} {
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
tk::map_iterator::map_iterator(void) noexcept
:  _tokens{nullptr}, _index{0U} {
}


// -- public operators --------------------------------------------------------

/* pre-increment operator */
auto tk::map_iterator::operator++(void) noexcept -> self& {
	++_index;
	return *this;
}

/* post-increment operator */
auto tk::map_iterator::operator++(int) noexcept -> self {
	self tmp{*this};
	++_index;
	return tmp;
}


/* dereference operator */
auto tk::map_iterator::operator*(void) const noexcept -> tk::raw::token& {
	return _tokens->_raw[_tokens->_map[_index]];
}

/* arrow operator */
auto tk::map_iterator::operator->(void) const noexcept -> tk::raw::token* {
	return &(_tokens->_raw[_tokens->_map[_index]]);
}


/* equality operator */
auto tk::map_iterator::operator==(const self& other) const noexcept -> bool {
	return _index == other._index;
}

/* inequality operator */
auto tk::map_iterator::operator!=(const self& other) const noexcept -> bool {
	return _index != other._index;
}


/* const [] operator */
auto tk::map_iterator::operator[](const mx::isz offset) const noexcept -> tk::raw::token& {
	return _tokens->_raw[_tokens->_map[_index + offset]];
}


// -- public iterators --------------------------------------------------------

/* const begin */
auto tk::map_iterator::begin(void) const noexcept -> self {
	return tk::map_iterator{*_tokens};
}

/* const end */
auto tk::map_iterator::end(void) const noexcept -> self {
	return tk::map_iterator{*_tokens, _tokens->_map.size()};
}


// -- public accessors --------------------------------------------------------

/* token */
auto tk::map_iterator::token(void) const noexcept -> tk::raw::token& {
	return _tokens->_raw[_tokens->_map[_index]];
}



// -- C O N S T  M A P  I T E R A T O R  --------------------------------------

// -- private lifecycle -------------------------------------------------------

/* base constructor */
tk::const_map_iterator::const_map_iterator(const tk::tokens& tokens) noexcept
: _tokens{&tokens}, _index{0U} {
}

/* constructor */
tk::const_map_iterator::const_map_iterator(const tk::tokens& tokens,
										   const mx::usz index) noexcept
: _tokens{&tokens}, _index{index} {
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
tk::const_map_iterator::const_map_iterator(void) noexcept
:  _tokens{nullptr}, _index{0U} {
}


// -- public operators --------------------------------------------------------

/* pre-increment operator */
auto tk::const_map_iterator::operator++(void) noexcept -> self& {
	++_index;
	return *this;
}

/* post-increment operator */
auto tk::const_map_iterator::operator++(int) noexcept -> self {
	self tmp{*this};
	++_index;
	return tmp;
}


/* dereference operator */
auto tk::const_map_iterator::operator*(void) const noexcept -> const tk::raw::token& {
	return _tokens->_raw[_tokens->_map[_index]];
}

/* arrow operator */
auto tk::const_map_iterator::operator->(void) const noexcept -> const tk::raw::token* {
	return &(_tokens->_raw[_tokens->_map[_index]]);
}


/* equality operator */
auto tk::const_map_iterator::operator==(const self& other) const noexcept -> bool {
	return _index == other._index;
}

/* inequality operator */
auto tk::const_map_iterator::operator!=(const self& other) const noexcept -> bool {
	return _index != other._index;
}


/* const [] operator */
auto tk::const_map_iterator::operator[](const mx::isz offset) const noexcept -> const tk::raw::token& {
	return _tokens->_raw[_tokens->_map[_index + offset]];
}


// -- public iterators --------------------------------------------------------

/* const begin */
auto tk::const_map_iterator::begin(void) const noexcept -> self {
	return tk::const_map_iterator{*_tokens};
}

/* const end */
auto tk::const_map_iterator::end(void) const noexcept -> self {
	return tk::const_map_iterator{*_tokens, _tokens->_map.size()};
}


// -- public accessors --------------------------------------------------------

/* token */
auto tk::const_map_iterator::token(void) const noexcept -> const tk::raw::token& {
	return _tokens->_raw[_tokens->_map[_index]];
}

/* index */
auto tk::const_map_iterator::index(void) const noexcept -> mx::usz {
	return _index;
}
