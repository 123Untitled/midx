#ifndef language_parser_hpp
#define language_parser_hpp

#include "language/tokens/tokens.hpp"
#include "language/diagnostic.hpp"
#include "language/parser/rules.hpp"
#include "language/ast.hpp"


// -- forward declarations ----------------------------------------------------

namespace as { class tree;       }
namespace an { class diagnostic; }


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {

	enum class level : mx::uint;

	// -- P A R S E R ---------------------------------------------------------

	class parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = pr::parser;




			// -- private members ---------------------------------------------

			/* arena */
			as::arena _arena;

			/* tokens */
			tk::tokens* _tokens;
			as::tree*   _tree;

			/* diagnostic */
			an::diagnostic* _diag;

			tk::iterator _it;
			tk::iterator _end;

			mx::uint _depth;

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(tk::tokens&, as::tree&, an::diagnostic&) -> void;



			// -- private methods ---------------------------------------------

			auto error(const char*) -> void;
			auto error(const char*, const tk::iterator&) -> void;


			template <pr::level>
			auto parse_expr(const pr::precedence) -> mx::usz;



			template <pr::level>
			auto nud_tempo(const mx::usz) -> mx::usz;

			template <pr::level>
			auto nud_value(const mx::usz) -> mx::usz;

			template <pr::level>
			auto nud_group(const mx::usz) -> mx::usz;

			template <pr::level>
			auto nud_parameter(const mx::usz) -> mx::usz;

			template <pr::level>
			auto nud_permutation(const mx::usz) -> mx::usz;

			template <pr::level>
			auto led_parallel(const mx::usz) -> mx::usz;

			template <pr::level>
			auto led_crossfade(const mx::usz) -> mx::usz;

			template <pr::level>
			auto led_tracksep(const mx::usz) -> mx::usz;


			auto merge_as_sequence(const mx::usz, const mx::usz) -> mx::usz;
			auto append_to_sequence(const mx::usz, const mx::usz) -> mx::usz;

			auto abord(void) noexcept -> void {
				_it = _end;
			}

	}; // class parser

} // namespace pr

#endif // language_parser_hpp
