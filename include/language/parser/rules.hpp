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

			/* nud type */
			using nud_type = auto (pr::parser::*)(const mx::usz) -> mx::usz;

			/* led type */
			using led_type = auto (pr::parser::*)(const mx::usz) -> mx::usz;



			// -- public members ----------------------------------------------

			/* null denotation */
			const nud_type nud;

			/* left denotation */
			const led_type led;

			/* precedence */
			const pr::precedence pre;

			/* can start an expression */
			const bool can_start;

			/* is infix */
			const bool is_infix;

			/* is prefix */
			const bool is_prefix;

			/* is postfix */
			const bool is_postfix;



	}; // class rule

} // namespace pr

#endif // language_parser_rules_hpp
