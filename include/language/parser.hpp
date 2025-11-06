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

			// -- private structs ---------------------------------------------

			struct context final {
				action_type st;
				tk::token*  tk;
				auto execute(self& s) -> void {
					(s.*st)();
				}
			}; // struct context


			// -- private members ---------------------------------------------

			/* state stack */
			std::vector<context> _states;

			/* tokens */
			tk::token_list* _tokens;

			/* diagnostic */
			an::diagnostic* _diag;

			/* current token */
			tk::token* _current;

			/* previous token */
			tk::token* _prev;

			/* ast */
			as::tree* _tree;

			/* id counter */
			ml::usz _counter;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(tk::token_list&, an::diagnostic&, as::tree&) -> void;


			/* switch state */
			template <typename>
			auto switch_state(void) noexcept -> void;

			/* push state */
			template <typename>
			auto push_state(void) -> void;

			/* push error */
			template <ml::literal>
			auto push_error(const tk::token* = nullptr) -> void;

			/* push warning */
			template <ml::literal>
			auto push_warning(const tk::token* = nullptr) -> void;


			auto new_id(void) noexcept -> ml::usz {
				auto x = _counter++;
				x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
				x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
				x = x ^ (x >> 31);
				return x;
			}





			// -- main states -------------------------------------------------

			#define GENERATE_STATE(name) \
				auto state_##name(void) -> void; \
				struct name final { \
					non_instantiable_class(name); \
					static constexpr action_type state = &self::state_##name; }



			template <bool>
			auto state_expect_identifier(void) -> void;

			template <bool N>
			struct expect_identifier final {
				non_instantiable_class(expect_identifier);
				static constexpr action_type state = &self::state_expect_identifier<N>;
			};


			GENERATE_STATE(expect_block);
			GENERATE_STATE(expect_specifier);
			//GENERATE_STATE(expect_identifier);
			GENERATE_STATE(expect_identifier_nested);
			GENERATE_STATE(expect_dot);
			GENERATE_STATE(expect_parameter);
			GENERATE_STATE(expect_value);
			GENERATE_STATE(panic_block);
			GENERATE_STATE(panic_remove_block);
			GENERATE_STATE(panic_parameter);

			#undef GENERATE_STATE

			//auto expect_block(void) -> void;
			//auto expect_specifier(void) -> void;
			//auto expect_identifier(void) -> void;
			//auto expect_identifier_nested(void) -> void;
			//auto expect_dot(void) -> void;
			//auto expect_parameter(void) -> void;
			//auto expect_value(void) -> void;
			//auto panic_block(void) -> void;
			//auto panic_remove_block(void) -> void;
			//auto panic_parameter(void) -> void;

	}; // class parser

} // namespace pr

#endif // language_parser_hpp
