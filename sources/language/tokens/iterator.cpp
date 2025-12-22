#include "language/tokens/iterator.hpp"
#include "language/tokens/tokens.hpp"


// -- I T E R A T O R ---------------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* tokens constructor */
tk::iterator::iterator(tk::tokens* tokens) noexcept
: _tokens{tokens}, _index{0U} {
}

/* end constructor */
tk::iterator::iterator(tk::tokens* tokens, const mx::usz end) noexcept
: _tokens{tokens}, _index{end} {
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
tk::iterator::iterator(void) noexcept
: _tokens{nullptr}, _index{0U} {
}


// -- public operators --------------------------------------------------------

/* dereference operator */
auto tk::iterator::operator*(void) const noexcept -> tk::token_view {
	return tk::token_view{
				_tokens->_chunks.data(),
				&_tokens->_tokens[_tokens->_filtered[_index]]
	};
}

/* pre-increment operator */
auto tk::iterator::operator++(void) noexcept -> self& {
	++_index;
	return *this;
}

/* post-increment operator */
auto tk::iterator::operator++(int) noexcept -> self {
	self tmp = *this;
	++_index;
	return tmp;
}

/* pre-decrement operator */
auto tk::iterator::operator--(void) noexcept -> self& {
	--_index;
	return *this;
}

/* post-decrement operator */
auto tk::iterator::operator--(int) noexcept -> self {
	self tmp = *this;
	--_index;
	return tmp;
}

/* equality operator */
auto tk::iterator::operator==(const self& other) const noexcept -> bool {
	return _index == other._index;
}

/* inequality operator */
auto tk::iterator::operator!=(const self& other) const noexcept -> bool {
	return _index != other._index;
}


// -- public iterators --------------------------------------------------------

/* begin */
auto tk::iterator::begin(void) const noexcept -> self {
	return self{_tokens};
}

/* end */
auto tk::iterator::end(void) const noexcept -> self {
	return self{_tokens, _tokens->_filtered.size()};
}


// -- public accessors --------------------------------------------------------

/* token */
auto tk::iterator::token(void) const noexcept -> tk::token& {
	return _tokens->_tokens[_tokens->_filtered[_index]];
}

/* view */
auto tk::iterator::view(void) const noexcept -> tk::token_view {
	return **this;
}

/* is end */
auto tk::iterator::is_end(void) const noexcept -> bool {
	return !(_index < _tokens->_filtered.size())
		|| self::token().id == tk::end_of_tokens;
}

/* index */
auto tk::iterator::index(void) const noexcept -> mx::usz {
	return _index;
}


// -- C O N S T  I T E R A T O R ----------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* tokens constructor */
tk::const_iterator::const_iterator(const tk::tokens* tokens) noexcept
: _tokens{tokens}, _index{0U} {
}

/* end constructor */
tk::const_iterator::const_iterator(const tk::tokens* tokens, const mx::usz end) noexcept
: _tokens{tokens}, _index{end} {
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
tk::const_iterator::const_iterator(void) noexcept
: _tokens{nullptr}, _index{0U} {
}


// -- public operators --------------------------------------------------------

/* dereference operator */
auto tk::const_iterator::operator*(void) const noexcept -> tk::const_token_view {
	return tk::const_token_view{
				_tokens->_chunks.data(),
				&_tokens->_tokens[_tokens->_filtered[_index]]
	};
}

/* pre-increment operator */
auto tk::const_iterator::operator++(void) noexcept -> self& {
	++_index;
	return *this;
}

/* post-increment operator */
auto tk::const_iterator::operator++(int) noexcept -> self {
	self tmp = *this;
	++_index;
	return tmp;
}

/* equality operator */
auto tk::const_iterator::operator==(const self& other) const noexcept -> bool {
	return _index == other._index;
}

/* inequality operator */
auto tk::const_iterator::operator!=(const self& other) const noexcept -> bool {
	return _index != other._index;
}


// -- public iterators --------------------------------------------------------

/* begin */
auto tk::const_iterator::begin(void) const noexcept -> self {
	return self{_tokens};
}

/* end */
auto tk::const_iterator::end(void) const noexcept -> self {
	return self{_tokens, _tokens->_filtered.size()};
}


// -- public accessors --------------------------------------------------------

/* token */
auto tk::const_iterator::token(void) const noexcept -> const tk::token& {
	return _tokens->_tokens[_tokens->_filtered[_index]];
}

/* view */
auto tk::const_iterator::view(void) const noexcept -> tk::const_token_view {
	return **this;
}

/* is end */
auto tk::const_iterator::is_end(void) const noexcept -> bool {
	return !(_index < _tokens->_filtered.size())
		|| self::token().id == tk::end_of_tokens;
}

/* index */
auto tk::const_iterator::index(void) const noexcept -> mx::usz {
	return _index;
}
