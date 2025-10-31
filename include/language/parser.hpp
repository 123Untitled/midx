#ifndef language_parser_hpp
#define language_parser_hpp

#include "literal.hpp"
#include <vector>


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- forward declarations ------------------------------------------------

	class parser;
	class table;
	class context;
	using action = auto (pr::parser::*)(void) -> void;


	// -- P A R S E R ---------------------------------------------------------

	class parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = pr::parser;


		private:

			// -- private members ---------------------------------------------

			/* context */
			pr::context* _ctx;

			/* stack */
			std::vector<const pr::table*> _stack;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			auto parse(pr::context&) -> void;

			auto end_of_file(void) -> void;


			/* call actions */
			template <pr::action... As>
			auto call(void) -> void {
				((this->*As)(), ...);
			}

			/* switch state */
			template <typename>
			auto switch_state(void) noexcept -> void;

			/* push state */
			template <typename>
			auto push_state(void) -> void;

			/* pop state */
			auto pop_state(void) -> void;

			/* newline */
			auto newline(void) noexcept -> void;

			/* push error */
			template <ml::literal>
			auto push_error(void) -> void;

			/* count */
			auto count(void) noexcept -> void;

			/* reset count */
			auto reset_count(void) noexcept -> void;



			// -- specifier actions -------------------------------------------

			/* search specifier */
			auto search_specifier(void) -> void;

			/* on track */
			auto on_track(void) -> void;



			// -- identifier actions ------------------------------------------

			/* handle identifier */
			auto handle_identifier(void) -> void;


			// -- parameter actions -------------------------------------------

			/* search parameter */
			auto search_parameter(void) -> void;

	}; // class parser

} // namespace pr

#endif // language_parser_hpp
