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
			.eval = L == pr::level::seq ?
				&pr::parser::eval_atomic_value
				: nullptr,
			.pre  = pr::precedence<L>::value,
		};
	}

	/* empty */
	template <pr::level L>
	consteval auto rule_empty(void) -> pr::rule {
		return pr::rule {
			.eval = nullptr,
			.pre = pr::precedence<L>::none,
		};
	}

	/* tempo */
	template <pr::level L>
	consteval auto rule_tempo(void) -> pr::rule {
		return pr::rule {
			.eval = &pr::parser::eval_tempo<L>,
			.pre = pr::precedence<L>::tempo,
		};
	}

	/* parameter */
	template <pr::level L>
	consteval auto rule_parameter(void) -> pr::rule {
		return pr::rule {
			.eval = L == pr::level::expr ?
						&pr::parser::eval_parameter :
						nullptr,
			.pre = pr::precedence<L>::parameter,
		};
	}


	/* track separator */
	template <pr::level L>
	consteval auto rule_track_separator(void) -> pr::rule {
		return pr::rule {
			// nud
			.eval = L == pr::level::expr ? &pr::parser::eval_track_separator : nullptr,
			// precedence
			.pre = pr::precedence<L>::track_separator,
		};
	}

	/* separator */
	template <pr::level L>
	consteval auto rule_separator(void) -> pr::rule {
		return pr::rule {
			// nud
			.eval = nullptr,
			// precedence
			.pre = pr::precedence<L>::separator,
		};
	}


	// -- R U L E S -----------------------------------------------------------

	template <pr::level L>
	constexpr pr::rule rules[tk::max_tokens] {

		// identifier
		pr::rule_empty<L>(),

		// separator ;
		pr::rule_separator<L>(),


		// tempo ^2 \2
		pr::rule_tempo<L>(),
		pr::rule_tempo<L>(),

		// modulo %
		{
			// led
			&pr::parser::eval_modulo<L>,
			// precedence
			pr::precedence<L>::modulo,
		},

		// parallel
		{
			// led
			&pr::parser::eval_parallel<L>,
			// precedence
			pr::precedence<L>::parallel,
		},

		// crossfade
		{
			// led
			&pr::parser::eval_crossfade<L>,
			// precedence
			pr::precedence<L>::crossfade,
		},

		// track separator
		pr::rule_track_separator<L>(),

		// parameter
		pr::rule_parameter<L>(),


		// reference
		{
			// nud
			&pr::parser::eval_references,
			// precedence
			pr::precedence<L>::reference,
		},

		// param reference
		{
			// nud
			nullptr,
			// precedence
			pr::precedence<L>::none,
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
			&pr::parser::eval_group<L>,
			// precedence
			pr::precedence<L>::grouping,
		},
		// priority_close,
		pr::rule_empty<L>(),

		// permutation_open,
		{
			// nud
			&pr::parser::eval_permutation<L>,
			// precedence
			pr::precedence<L>::grouping,
		},
		// permutation_close,
		pr::rule_empty<L>(),


		// condition_open,
		{
			// nud
			nullptr,
			// precedence
			pr::precedence<L>::grouping,
		},

		// condition_close,
		pr::rule_empty<L>(),



		// -- non used tokens -------------------------------------------------

		// comment
		pr::rule_empty<L>(),

		// invalid
		pr::rule_empty<L>(),

		// end of tokens
		pr::rule_empty<L>(),
	};


	// -- helpers -------------------------------------------------------------

	/* nud of */
	template <pr::level L>
	inline auto eval_of(const tk::token& token) noexcept -> pr::rule::eval_type {
		return rules<L>[token.id].eval;
	}

	/* precedence of */
	template <pr::level L>
	inline auto pre_of(const tk::token& token) noexcept -> mx::uint {
		return rules<L>[token.id].pre;
	}

	/* can start */
	template <pr::level L>
	inline auto can_start(const tk::token& token) noexcept -> bool {
		return rules<L>[token.id].can_start;
	}

} // namespace pr

#endif // language_parser_table_hpp
