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

			/* ast */
			as::tree* _tree;

			/* diagnostic */
			an::diagnostic* _diag;

			/* state */
			action_type _state;


			/* specifier */
			tk::token* _specifier;

			/* current token */
			tk::token* _current;

			/* previous token */
			tk::token* _prev;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(tk::token_list&, an::diagnostic&, as::tree&) -> void;


		private:

			// -- private methods ---------------------------------------------

			/* switch state */
			template <typename>
			auto switch_state(void) noexcept -> void;

			/* push error */
			template <ml::literal>
			auto push_error(const tk::token* = nullptr) -> void;

			/* push warning */
			template <ml::literal>
			auto push_warning(const tk::token* = nullptr) -> void;



			// -- main states -------------------------------------------------

			#define GENERATE_STATE(name) \
				auto state_##name(void) -> void; \
				struct name final { \
					non_instantiable_class(name); \
					static constexpr action_type state = &self::state_##name; }


			GENERATE_STATE(expect_block_open);
			GENERATE_STATE(expect_block_close);
			GENERATE_STATE(expect_specifier);
			GENERATE_STATE(expect_identifier);
			GENERATE_STATE(expect_dot);
			GENERATE_STATE(expect_parameter);
			GENERATE_STATE(expect_value);

			GENERATE_STATE(panic_block);
			GENERATE_STATE(panic_parameter);

			#undef GENERATE_STATE

	}; // class parser

} // namespace pr

#endif // language_parser_hpp
