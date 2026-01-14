#ifndef language_analyzer_hpp
#define language_analyzer_hpp

#include "language/lexer.hpp"
#include "language/parser.hpp"
#include "language/tokens/tokens.hpp"
#include "language/diagnostic.hpp"
#include "language/ast/tree.hpp"

#include "core/string/string.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// --  A N A L Y Z E R ----------------------------------------------------

	class analyzer final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::analyzer;


			// -- private members ---------------------------------------------

			/* data */
			mx::string _data;

			/* tokens */
			tk::tokens _tokens;

			/* lexer */
			lx::lexer _lexer;

			/* parser */
			pr::parser _parser;

			/* tree */
			as::tree _tree;

			/* diagnostic */
			an::diagnostic _diagnostic;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			analyzer(void);


			// -- public methods ----------------------------------------------

			/* analyze from string */
			auto analyze(mx::string&&) -> void;

			/* highlights */
			auto highlights(void) -> mx::string;

			/* has errors */
			auto has_errors(void) const noexcept -> bool {
				return _diagnostic.has_errors();
			}

			/* tree */
			auto tree(void) const noexcept -> const as::tree& {
				return _tree;
			}

			/* tokens */
			auto tokens(void) const noexcept -> const tk::tokens& {
				return _tokens;
			}

	}; // class analyzer

} // namespace mx

#endif // language_analyzer_hpp
