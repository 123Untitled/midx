#ifndef language_parser_hpp
#define language_parser_hpp

#include "macros.hpp"
#include "byte_range.hpp"
#include "types.hpp"
#include "literal.hpp"
#include <vector>

#include "language/tokens/token_list.hpp"

#include "language/diagnostic.hpp"

namespace an {
	class diagnostic;
}
namespace as {
	class tree;
}


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- P A R S E R ---------------------------------------------------------

	class parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = pr::parser;

			/* action type */
			using action_type = auto (pr::parser::*)(void) -> void;


		private:

			// -- private members ---------------------------------------------

			/* tokens */
			tk::tokens* _tokens;

			/* diagnostic */
			an::diagnostic* _diag;


			/* iterator */
			//tk::const_map_iterator _it;

			/* checkpoint */
			//decltype(_it) _checkpoint;


			/* state */
			action_type _state;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(tk::tokens&, an::diagnostic&, as::tree&) -> void;


		private:

			// -- private methods ---------------------------------------------

			/* initial state */
			auto initial(void) -> void;

	}; // class parser

} // namespace pr

#endif // language_parser_hpp
