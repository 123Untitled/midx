#ifndef language_parser_rules_hpp
#define language_parser_rules_hpp

#include "language/parser/precedence.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- forward declarations ------------------------------------------------

	class parser;


	// -- R U L E -------------------------------------------------------------

	class rule final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = pr::rule;


		public:

			// -- public types ------------------------------------------------

			/* eval type */
			using eval_type = auto (pr::parser::*)(const mx::usz) -> mx::usz;


			// -- public members ----------------------------------------------

			/* eval function */
			const eval_type eval;

			/* precedence */
			const mx::uint pre;
			//const pr::precedence pre;

	}; // class rule

} // namespace pr

#endif // language_parser_rules_hpp
