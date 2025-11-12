#ifndef language_parser_states_hpp
#define language_parser_states_hpp

#include "language/parser/table.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- S T A T E -----------------------------------------------------------

	template <mx::literal N, pr::is_rule... Rs>
	struct state {
		non_instantiable_class(state);
		static constexpr const char* name = N.data;
		static constexpr pr::table  table = pr::make_table<Rs...>();
	}; // class state


	class expect_block;
	class expect_specifier;
	class expect_identifier;
	class expect_dot;
	class expect_parameter;

	class panic_block;
	class panic_parameter;


	class expect_block : public pr::state<"expect_block",

		// invalid
		pr::rule<tk::all,
					&pr::parser::push_error<"expected block">,
					&pr::parser::mark_invalid>,

		// open bracket
		pr::rule<tk::bracket_open,
					&pr::parser::push_state<pr::expect_specifier>>,

		// end of tokens
		pr::rule<tk::end_of_tokens>
	>{};


	class expect_specifier : public pr::state<"expect_specifier",

		// invalid
		pr::rule<tk::all,
					&pr::parser::push_error<"expected specifier">,
					&pr::parser::switch_state<pr::panic_block>>,

		// close bracket
		pr::rule<tk::bracket_close,
					&pr::parser::pop_state>,

		// text
		pr::rule<tk::text,
					&pr::parser::search_specifier>

		// next state is triggered by search_specifier action
	>{};


	class expect_identifier : public pr::state<"expect_identifier",

		// invalid
		pr::rule<tk::all,
					&pr::parser::push_error<"expected identifier or parameter">>,

		// dot
		pr::rule<tk::dot,
					&pr::parser::switch_state<pr::expect_parameter>>,

		// string
		pr::rule<tk::string,
					&pr::parser::handle_identifier,
					&pr::parser::switch_state<pr::expect_dot>>
	>{};


	class expect_dot : public pr::state<"expect_dot",

		// invalid
		pr::rule<tk::all,
					&pr::parser::push_error<"expected parameter">,
					&pr::parser::switch_state<pr::panic_parameter>>,

		// dot
		pr::rule<tk::dot,
					&pr::parser::switch_state<pr::expect_parameter>>
	>{};


	class expect_parameter : public pr::state<"expect_parameter",

		// invalid
		pr::rule<tk::all,
					&pr::parser::push_error<"expected parameter">,
					&pr::parser::switch_state<pr::panic_parameter>>,

		// text
		pr::rule<tk::text,
					&pr::parser::search_parameter>

		// next state is triggered by search_parameter action
	>{};


	class panic_block : public pr::state<"panic_block",

		// invalid
		pr::rule<tk::all,
					&pr::parser::mark_invalid>,

		// open bracket
		pr::rule<tk::bracket_open,
					&pr::parser::push_state<pr::panic_block>>,

		// close bracket
		pr::rule<tk::bracket_close,
					&pr::parser::pop_state>

		// need to handle next tokens implementation like (){} ??
	>{};

	class panic_parameter : public pr::state<"panic_parameter",

		// invalid
		pr::rule<tk::all,
					&pr::parser::mark_invalid>,

		// dot
		pr::rule<tk::dot,
					&pr::parser::push_state<pr::expect_parameter>>,

		// open bracket
		pr::rule<tk::bracket_open,
					&pr::parser::push_state<pr::panic_block>>

		// need to handle next tokens implementation like (){} ??
	>{};


} // namespace pr

#endif // language_parser_states_hpp
