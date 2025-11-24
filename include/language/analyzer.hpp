#ifndef language_analyzer_hpp
#define language_analyzer_hpp

#include "language/lexer.hpp"
#include "language/parser.hpp"
#include "language/tokens/tokens.hpp"
#include "language/diagnostic.hpp"
#include "language/ast/tree.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// --  A N A L Y Z E R ----------------------------------------------------

	class analyzer final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::analyzer;


			// -- private members ---------------------------------------------

			std::string _data;


			/* lexer */
			lx::lexer _lexer;

			/* parser */
			pr::parser _parser;


			/* tokens */
			tk::tokens _tokens;

			/* tree */
			as::tree _tree;

			/* diagnostic */
			an::diagnostic _diagnostic;


			std::string _highlights;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			analyzer(void);


			// -- public methods ----------------------------------------------

			/* analyze from string */
			auto analyze(std::string&&) -> void;

			/* highlights */
			auto highlights(void) noexcept -> std::string& {
				return _highlights;
			}

			/* has errors */
			auto has_errors(void) const noexcept -> bool {
				return _diagnostic.has_errors();
			}

			/* tree */
			auto tree(void) noexcept -> as::tree& {
				return _tree;
			}

	}; // class analyzer

} // namespace mx

#endif // language_analyzer_hpp
