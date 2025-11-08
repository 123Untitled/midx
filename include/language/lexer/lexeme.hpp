#ifndef language_lexer_lexeme_hpp
#define language_lexer_lexeme_hpp

#include "types.hpp"
#include <iostream>


// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {


	// -- L E X E M E ---------------------------------------------------------

	struct lexeme final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = lx::lexeme;


		public:

			// -- public members ----------------------------------------------

			const ml::u8* data;
			const ml::usz size;



			// -- public operators --------------------------------------------

			/* equality operator */
			auto operator==(const self& other) const noexcept -> bool {

				// size check
				if (size != other.size)
					return false;

				// data check
				for (ml::usz i = 0; i < size; ++i) {
					if (data[i] != other.data[i])
						return false; }

				return true;
			}

			/* inequality operator */
			auto operator!=(const lexeme& other) const noexcept -> bool {
				return !(*this == other);
			}


			// -- public methods ----------------------------------------------

			/* hash */
			auto hash(void) const noexcept -> ml::usz {
				ml::usz h = 0U;
				for (ml::usz i = 0U; i < size; ++i)
					h = h * 31U + static_cast<ml::usz>(data[i]);
				return h;
			}

	}; // struct lexeme

} // namespace lx


// overload output operator for lexeme
inline auto operator<<(std::ostream& os, const lx::lexeme& lex) -> std::ostream& {
	os.write(reinterpret_cast<const char*>(lex.data), static_cast<std::streamsize>(lex.size));
	return os;
}

#endif // language_lexer_lexeme_hpp
