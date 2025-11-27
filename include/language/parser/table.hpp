#ifndef language_parser_table_hpp
#define language_parser_table_hpp

#include "language/tokens/def.hpp"
#include "language/parser/rules.hpp"
#include "language/parser.hpp"
#include "language/parser/levels.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {



	/* number */
	template <pr::level L>
	consteval auto rule_number(void) -> pr::rule {
		return pr::rule {
			.eval = &pr::parser::nud_atomic_value,
			.pre  = pr::precedence::value,
		};
	}

	/* empty */
	consteval auto rule_empty(void) -> pr::rule {
		return pr::rule {
			.eval = nullptr,
			.pre = pr::precedence::none,
		};
	}

	/* tempo */
	template <pr::level L>
	consteval auto rule_tempo(void) -> pr::rule {
		return pr::rule {
			.eval = &pr::parser::nud_tempo<L>,
			.pre = pr::precedence::tempo,
		};
	}

	/* parameter */
	template <pr::level L>
	consteval auto rule_parameter(void) -> pr::rule {
		return pr::rule {
			.eval = L == pr::level::expr ?
						&pr::parser::nud_parameter :
						nullptr,
			.pre = pr::precedence::parameter,
		};
	}


	/* track separator */
	template <pr::level L>
	consteval auto rule_track_separator(void) -> pr::rule {
		return pr::rule {
			// nud
			.eval = L == pr::level::expr ? &pr::parser::nud_track_separator : nullptr,
			// precedence
			.pre = pr::precedence::track_separator,
		};
	}

	/* separator */
	template <pr::level L>
	consteval auto rule_separator(void) -> pr::rule {
		return pr::rule {
			// nud
			.eval = nullptr,
			// precedence
			.pre = pr::precedence::separator,
		};
	}


	// -- R U L E S -----------------------------------------------------------

	template <pr::level L>
	constexpr pr::rule rules[tk::max_tokens] {

		// identifier
		pr::rule_empty(),

		// separator ;
		pr::rule_separator<L>(),


		// tempo ^2 \2
		pr::rule_tempo<L>(),
		pr::rule_tempo<L>(),

		// modulo %
		{
			// led
			&pr::parser::nud_modulo<L>,
			// precedence
			pr::precedence::modulo,
		},

		// parallel
		{
			// led
			&pr::parser::led_parallel<L>,
			// precedence
			pr::precedence::parallel,
		},

		// crossfade
		{
			// led
			&pr::parser::led_crossfade<L>,
			// precedence
			pr::precedence::crossfade,
		},

		// track separator
		pr::rule_track_separator<L>(),

		// parameter
		pr::rule_parameter<L>(),


		// reference
		{
			// nud
			&pr::parser::nud_references,
			// precedence
			pr::precedence::reference,
		},

		// param reference
		{
			// nud
			nullptr,
			// precedence
			pr::precedence::none,
		},


		// -- numbers ---------------------------------------------------------

		// note
		rule_number<L>(),
		// binary
		rule_number<L>(),
		// octal
		rule_number<L>(),
		// decimal
		rule_number<L>(),
		// hexadecimal
		rule_number<L>(),
		// floating
		rule_number<L>(),


		// () [] {}
		// priority_open,
		{
			// nud
			&pr::parser::nud_group<L>,
			// precedence
			pr::precedence::grouping,
		},
		// priority_close,
		pr::rule_empty(),

		// permutation_open,
		{
			// nud
			&pr::parser::nud_permutation<L>,
			// precedence
			pr::precedence::grouping,
		},
		// permutation_close,
		pr::rule_empty(),


		// condition_open,
		{
			// nud
			nullptr,
			// precedence
			pr::precedence::grouping,
		},

		// condition_close,
		pr::rule_empty(),



		// -- non used tokens -------------------------------------------------

		// comment
		pr::rule_empty(),

		// invalid
		pr::rule_empty(),

		// end of tokens
		pr::rule_empty(),
	};


	// -- helpers -------------------------------------------------------------

	/* nud of */
	template <pr::level L>
	inline auto eval_of(const tk::token& token) noexcept -> pr::rule::eval_type {
		return rules<L>[token.id].eval;
	}

	/* precedence of */
	template <pr::level L>
	inline auto pre_of(const tk::token& token) noexcept -> pr::precedence {
		return rules<L>[token.id].pre;
	}

	/* can start */
	template <pr::level L>
	inline auto can_start(const tk::token& token) noexcept -> bool {
		return rules<L>[token.id].can_start;
	}

} // namespace pr

#endif // language_parser_table_hpp
