#ifndef language_parser_hpp
#define language_parser_hpp

#include "byte_range.hpp"
#include "types.hpp"
#include "literal.hpp"
#include <vector>

#include "language/tokens/token_list.hpp"
#include "language/ast.hpp"

#include "language/syntax/syntax_layout.hpp"


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


			/* buffer */
			const ml::u8* _buffer;

			/* tokens */
			tk::token_list* _tokens;

			/* current token */
			tk::token* _current;


			//std::vector<ast::block> _blocks;

			struct block_context final {
					ml::usz bi;
					ml::usz pi;
				public:
					explicit block_context(const ml::usz bi) noexcept
					: bi{bi}, pi{UINT64_MAX} {
					}
			};

			std::vector<block_context> _ctx; // stack of block indices

			/* state stack */
			std::vector<action_type> _states;

			std::vector<std::string> _errors;
			/* syntax stack */
			//std::vector<sx::block_view_type> _context;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(const ml::byte_range&, tk::token_list&) -> void;


			/* switch state */
			template <typename>
			auto switch_state(void) noexcept -> void;

			/* push state */
			template <typename>
			auto push_state(void) -> void;




			/* pop state */
			auto pop_state(void) -> void;

			/* pop level */
			auto pop_level(void) -> void;

			/* push error */
			template <ml::literal>
			auto push_error(void) -> void;


			auto top_block(void) noexcept -> ast::block&;

			/* push block */
			auto push_block(void) -> void;

			/* push param */
			auto push_param(void) -> void;

			/* push value */
			auto push_value(void) -> void;

			/* push nested block */
			auto push_nested_block(void) -> void;

			/* mark invalid */
			auto mark_invalid(void) noexcept -> void;


			// -- main states -------------------------------------------------

			#define GENERATE_STATE(name) \
				auto state_##name(void) -> void; \
				struct name final { \
					non_instantiable_class(name); \
					static constexpr action_type state = &self::state_##name; }

			GENERATE_STATE(expect_block);
			GENERATE_STATE(expect_specifier);
			GENERATE_STATE(expect_identifier);
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
