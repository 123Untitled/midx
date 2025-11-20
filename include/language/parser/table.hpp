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
			.nud = &pr::parser::nud_value,
			//.nud = &pr::parser::nud_atomic_value,
			.led = nullptr,
			.pre = pr::precedence::none,
			.can_start = L == pr::level::sequence,
			// is infix, is prefix, is postfix
			false, false, false
		};
	}

	/* empty */
	consteval auto rule_empty(void) -> pr::rule {
		return pr::rule {
			.nud = nullptr,
			.led = nullptr,
			.pre = pr::precedence::none,
			// can start, is infix, is prefix, is postfix
			false, false, false, false
		};
	}

	/* tempo */
	template <pr::level L>
	consteval auto rule_tempo(void) -> pr::rule {
		return pr::rule {
			.nud = nullptr,
			.led = nullptr,
			.pre = pr::precedence::tempo,
			// can start, is infix, is prefix, is postfix
			true, false, true, false
		};
	}

	/* parameter */
	template <pr::level L>
	consteval auto rule_parameter(void) -> pr::rule {
		return pr::rule {
			// nud
			.nud = L == pr::level::expression ?
						&pr::parser::nud_parameter :
						nullptr,
			.led = nullptr,
			// precedence
			.pre = pr::precedence::none,
			// can start, is prefix, is infix, is postfix
			true, true, false, false
		};
	}

	/* identifier */
	template <pr::level L>
	consteval auto rule_identifier(void) -> pr::rule {
		return pr::rule {
			// nud
			nullptr,
			// led
			nullptr,
			// precedence
			pr::precedence::none,
			// can start, is prefix, is infix, is postfix
			true, false, false, false
		};
	}

	/* assignment */
	template <pr::level L>
	consteval auto rule_assignment(void) -> pr::rule {
		return pr::rule {
			// nud
			nullptr,
			// led
			nullptr,
			// precedence
			pr::precedence::none,
			// can start, is prefix, is infix, is postfix
			false, false, false, false
		};
	}

	/* track separator */
	template <pr::level L>
	consteval auto rule_track_separator(void) -> pr::rule {
		return pr::rule {
			// nud
			.nud = L == pr::level::expression ? &pr::parser::nud_track_separator : nullptr,
			// led
			.led = nullptr,
			// precedence
			.pre = pr::precedence::none,
			// can start, is prefix, is infix, is postfix
			true, false, false, false
		};
	}


	// -- R U L E S -----------------------------------------------------------

	template <pr::level L>
	constexpr pr::rule rules[tk::max_tokens] {

		// identifier
		pr::rule_identifier<L>(),

		// assignment =
		pr::rule_assignment<L>(),

		// separator ;
		pr::rule_empty(),


		// tempo ^2 \2
		pr::rule_tempo<L>(),
		pr::rule_tempo<L>(),

		// parallel
		{
			// nud
			nullptr,
			// led
			&pr::parser::led_parallel<L>,
			// precedence
			pr::precedence::parallel,
			// can start
			false,
			// is prefix
			false,
			// is infix
			true,
			// is postfix
			false
		},

		// crossfade
		{
			// nud
			nullptr,
			// led
			&pr::parser::led_crossfade<L>,
			// precedence
			pr::precedence::crossfade,
			// can start
			false,
			// is prefix
			false,
			// is infix
			true,
			// is postfix
			false
		},

		// track separator
		pr::rule_track_separator<L>(),

		// parameter
		pr::rule_parameter<L>(),

		// track reference
		{
			// nud
			&pr::parser::nud_value,
			// led
			nullptr,
			// precedence
			pr::precedence::none,
			// can start, is prefix, is infix, is postfix
			true, false, false, false
		},

		// param reference
		{
			// nud
			&pr::parser::nud_value,
			// led
			nullptr,
			// precedence
			pr::precedence::none,
			// can start
			.can_start = L == pr::level::sequence,
			// is prefix, is infix, is postfix
			false, false, false
		},


		// -- numbers ---------------------------------------------------------
		// note,
		rule_number<L>(),
		// binary,
		rule_number<L>(),
		// octal,
		rule_number<L>(),
		// decimal,
		rule_number<L>(),
		// hexadecimal,
		rule_number<L>(),


		// () [] {}
		// priority_open,
		{
			// nud
			&pr::parser::nud_group<L>,
			// led
			nullptr,
			// precedence
			pr::precedence::grouping,
			// can start, is prefix, is infix, is postfix
			true, false, false, false
		},
		// priority_close,
		pr::rule_empty(),

		// permutation_open,
		{
			// nud
			&pr::parser::nud_permutation<L>,
			// led
			nullptr,
			// precedence
			pr::precedence::grouping,
			// can start, is prefix, is infix, is postfix
			true, false, false, false
		},
		// permutation_close,
		pr::rule_empty(),


		// condition_open,
		{
			// nud
			nullptr,
			// led
			nullptr,
			// precedence
			pr::precedence::grouping,
			// can start, is prefix, is infix, is postfix
			true, false, false, false
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
	inline auto nud_of(const tk::token& token) noexcept -> pr::rule::nud_type {
		return rules<L>[token.id].nud;
	}

	/* led of */
	template <pr::level L>
	inline auto led_of(const tk::token& token) noexcept -> pr::rule::led_type {
		return rules<L>[token.id].led;
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
