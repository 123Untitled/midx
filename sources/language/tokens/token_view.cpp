#include "language/tokens/token_view.hpp"


// -- T O K E N  V I E W ------------------------------------------------------

// -- private lifecycle -------------------------------------------------------

/* chunks constructor */
tk::token_view::token_view(const tk::chunk* chunks) noexcept
: /* uninitialized */ _chunks{chunks} {
}


// -- public accessors --------------------------------------------------------

/* id */
auto tk::token_view::id(void) const noexcept -> tk::id {
	return _token->id;
}
