#include "language/tokens/token_view.hpp"


// -- T O K E N  V I E W ------------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* chunks constructor */
tk::token_view::token_view(tk::chunk* chunks,
						   tk::token* token) noexcept
: _chunks{chunks}, _token{token} {
}


// -- public accessors --------------------------------------------------------

/* id */
auto tk::token_view::id(void) const noexcept -> tk::id {
	return _token->id;
}

/* first chunk */
auto tk::token_view::first_chunk(void) const noexcept -> tk::chunk& {
	return _chunks[_token->start];
}

/* last chunk */
auto tk::token_view::last_chunk(void) const noexcept -> tk::chunk& {
	return _chunks[_token->start + _token->count - 1U];
}

/* number of chunks */
auto tk::token_view::num_chunks(void) const noexcept -> mx::usz {
	return _token->count;
}


// -- conversion operators ----------------------------------------------------

/* to const token view */
tk::token_view::operator tk::const_token_view(void) const noexcept {
	return tk::const_token_view{_chunks, _token};
}


// -- C O N S T  T O K E N  V I E W -------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* chunks constructor */
tk::const_token_view::const_token_view(const tk::chunk* chunks,
									   const tk::token* token) noexcept
: _chunks{chunks}, _token{token} {
}


// -- public accessors --------------------------------------------------------

/* id */
auto tk::const_token_view::id(void) const noexcept -> tk::id {
	return _token->id;
}

/* first chunk */
auto tk::const_token_view::first_chunk(void) const noexcept -> const tk::chunk& {
	return _chunks[_token->start];
}

/* last chunk */
auto tk::const_token_view::last_chunk(void) const noexcept -> const tk::chunk& {
	return _chunks[_token->start + _token->count - 1U];
}

/* number of chunks */
auto tk::const_token_view::num_chunks(void) const noexcept -> mx::usz {
	return _token->count;
}



#include <ostream>

/* ostream operator */
auto operator<<(std::ostream& os, const tk::const_token_view& tv) -> std::ostream& {

	if (tv.is_null()) {
		os << "<null token>";
		return os;
	}

	const auto id = tv.id();
	const auto len = tk::max_name_len - std::strlen(tk::names[id]);

	//os << tk::colors[id] << tk::names[id] << "\x1b[0m";
	//for (mx::usz i = 0U; i < len; ++i)
	//	os << ' ';
	os << "'";

	tv.for_each_chunk(
		[](const tk::chunk& c, std::ostream& os) static -> void {
			if (c.lexeme.data == nullptr)
				return;
			const char* ptr = reinterpret_cast<const char*>(c.lexeme.data);
			for (mx::usz i = 0U; i < c.lexeme.size; ++i)
				os << ptr[i];
	}, os);
	os << "'";

	return os;
}
