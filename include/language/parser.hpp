#ifndef language_parser_hpp
#define language_parser_hpp

#include "language/tokens/tokens.hpp"
#include "language/diagnostic.hpp"
#include "language/parser/rules.hpp"
#include "language/ast/tree.hpp"

#include "language/parser/levels.hpp"


// -- forward declarations ----------------------------------------------------

namespace as { class tree;       }
namespace an { class diagnostic; }


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- P A R S E R ---------------------------------------------------------

	class parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = pr::parser;




			// -- private members ---------------------------------------------

			/* tree */
			as::tree _tree;

			/* tokens */
			tk::tokens* _tokens;

			/* diagnostic */
			an::diagnostic* _diag;

			tk::iterator _it;
			tk::iterator _end;

			mx::uint _depth;

			bool _back;

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(tk::tokens&, as::tree&, an::diagnostic&) -> void;



			// -- private methods ---------------------------------------------

			auto error(const char*) const -> void;
			auto error(const char*, const tk::iterator&) const -> void;


			template <pr::level L>
			auto parse_expr(const pr::precedence) -> mx::usz;



			template <pr::level L>
			auto nud_tempo(const mx::usz) -> mx::usz;

			auto nud_value(const mx::usz) -> mx::usz;

			auto nud_atomic_value(const mx::usz) -> mx::usz;


			template <pr::level L>
			auto nud_group(const mx::usz) -> mx::usz;

			auto nud_parameter(const mx::usz) -> mx::usz;

			auto nud_track_separator(const mx::usz) -> mx::usz;

			template <pr::level L>
			auto nud_permutation(const mx::usz) -> mx::usz;



			template <pr::level>
			auto led_parallel(const mx::usz) -> mx::usz;

			template <pr::level>
			auto led_crossfade(const mx::usz) -> mx::usz;



			auto lookahead(tk::iterator) const noexcept -> bool;
			auto lookahead_op(tk::iterator) const noexcept -> bool;

			auto abord(void) noexcept -> void {
				_it = _end;
			}

	}; // class parser

} // namespace pr

#endif // language_parser_hpp
