#ifndef language_parser_hpp
#define language_parser_hpp

#include "language/tokens/tokens.hpp"
#include "language/diagnostic.hpp"
#include "language/parser/rules.hpp"
#include "language/ast/tree.hpp"

#include "language/parser/levels.hpp"

#include "language/syntax/parameter.hpp"
#include "language/identifier_map.hpp"

#include "math.hpp"


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
			as::tree* _tree;

			/* tokens */
			tk::tokens* _tokens;

			/* diagnostic */
			an::diagnostic* _diag;

			tk::iterator _it;
			tk::iterator _end;

			mx::uint _depth;

			bool _back;

			pa::id _last_param;

			/* identifiers */
			sx::identifier_map _idents;

			mx::frac _tempo;
			//double _factor;

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void);


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(tk::tokens&, as::tree&, an::diagnostic&) -> void;



			auto apply_tempo(const mx::usz steps) noexcept -> mx::frac {
				//return mx::frac{steps * _tempo.den,
				//				_tempo.num};
				return mx::frac{steps * 1, 1};
			}


			// -- private methods ---------------------------------------------

			auto error(const char*) const -> void;
			auto error(const char*, const tk::iterator&) const -> void;

			template <pr::level L, typename... Tp>
			auto debug_level(const Tp&... args) -> void {
				std::cout << '[' << _depth << ']';
				switch (L) {
					case pr::level::expr:
						std::cout << "[\x1b[32mlevel expression\x1b[0m]";
						break;

					case pr::level::seq:
						std::cout << "[\x1b[31mlevel   sequence\x1b[0m]";
						break;
				}
				((std::cout << ' ' << args), ...);
				std::cout << "\r\n";
			}


			auto _parse(void) -> mx::usz;
			auto _parse_identifiers(void) -> bool;


			/* parse expression */
			template <pr::level>
			auto parse_expr(const mx::uint/*pr::precedence*/
					) -> mx::usz;



			template <pr::level>
			auto parse_tempo(const mx::usz) -> mx::usz;

			template <pr::level>
			auto parse_modulo(const mx::usz) -> mx::usz;

			auto parse_value(const mx::usz) -> mx::usz;

			auto parse_atomics(const mx::usz) -> mx::usz;


			auto parse_references(const mx::usz) -> mx::usz;


			template <pr::level L>
			auto parse_group(const mx::usz) -> mx::usz;

			auto parse_parameter(const mx::usz) -> mx::usz;

			auto parse_track_separator(const mx::usz) -> mx::usz;

			template <pr::level L>
			auto parse_permutation(const mx::usz) -> mx::usz;



			template <pr::level>
			auto parse_parallel(const mx::usz) -> mx::usz;

			template <pr::level>
			auto parse_crossfade(const mx::usz) -> mx::usz;



			auto lookahead(tk::iterator) const noexcept -> bool;
			auto lookahead_op(tk::iterator) const -> bool;

			auto abord(void) noexcept -> void {
				_it = _end;
			}

	}; // class parser

} // namespace pr

#endif // language_parser_hpp
