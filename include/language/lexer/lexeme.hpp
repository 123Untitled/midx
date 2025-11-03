#ifndef language_lexer_lexeme_hpp
#define language_lexer_lexeme_hpp

#include "types.hpp"
#include <iostream>


// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {


	// -- L E X E M E ---------------------------------------------------------

	struct lexeme final {
		const ml::u8* data;
		const ml::usz size;
	}; // struct lexeme

} // namespace lx


// overload output operator for lexeme
inline auto operator<<(std::ostream& os, const lx::lexeme& lex) -> std::ostream& {
	os.write(reinterpret_cast<const char*>(lex.data), static_cast<std::streamsize>(lex.size));
	return os;
}

#endif // language_lexer_lexeme_hpp
