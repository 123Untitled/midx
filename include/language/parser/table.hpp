#ifndef language_parser_table_hpp
#define language_parser_table_hpp

#include "language/tokens/def.hpp"
#include "language/parser/rules.hpp"
#include "language/parser.hpp"
#include "language/parser/levels.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	/* number */
	consteval auto rule_number(void) -> pr::rule {
		return pr::rule {
			.nud = &pr::parser::nud_value,
			.led = nullptr,
			.pre = pr::precedence::none,
			.can_start = true,
				//L == pr::level::sequence,
			.is_infix   = false,
			.is_prefix  = false,
			.is_postfix = false
		};
	}

	/* empty */
	consteval auto rule_empty(void) -> pr::rule {
		return pr::rule {
			.nud = nullptr,
			.led = nullptr,
			.pre = pr::precedence::none,
			.can_start  = false,
			.is_infix   = false,
			.is_prefix  = false,
			.is_postfix = false
		};
	}

	/* tempo */
	consteval auto rule_tempo(void) -> pr::rule {
		return pr::rule {
			//.nud = &pr::parser::nud_tempo,
			.nud = &pr::parser::nud_value,
			.led = nullptr,
			.pre = pr::precedence::tempo,
			.can_start  = true,
			.is_infix   = false,
			.is_prefix  = true,
			.is_postfix = false
		};
	}

	/* parameter */
	consteval auto rule_parameter(void) -> pr::rule {

		return pr::rule {
			// nud
			.nud = nullptr,
			.led = &pr::parser::led_parameter,
			// precedence
			pr::precedence::parameter,
			// can start
			false,
			// is prefix
			true,
			// is infix
			true,
			// is postfix
			false
		};
	}

	// -- R U L E S -----------------------------------------------------------

	constexpr pr::rule rules[tk::max_tokens] {

		// identifier
		{
			// nud
			&pr::parser::nud_value,
			//nullptr,
			// led
			nullptr,
			// precedence
			pr::precedence::none,
			// can start
			true, // maybe false because identifiers are at global level only
			// is prefix
			false,
			// is infix
			false,
			// is postfix
			false
		},

		// assignment =
		{
			// nud
			nullptr,
			// led
			nullptr,
			// precedence
			pr::precedence::assignment,
			// can start
			false,
			// is prefix
			false,
			// is infix
			true,
			// is postfix
			false
		},

		// separator ;
		pr::rule_empty(),


		// tempo ^2 \2
		pr::rule_tempo(),
		pr::rule_tempo(),

		// parallel
		{
			// nud
			nullptr,
			// led
			&pr::parser::led_parallel,
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
			&pr::parser::led_crossfade,
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

		// block start
		{
			// nud
			//nullptr,
			.nud = &pr::parser::nud_value,
			// led
			//&pr::parser::led_tracksep,
			nullptr,
			// precedence
			//pr::precedence::tracksep,
			pr::precedence::none,
			// can start
			false,
			// is prefix
			false,
			// is infix
			true,
			// is postfix
			false
		},

		// parameter
		pr::rule_parameter(),

		// block reference
		{
			// nud
			&pr::parser::nud_value,
			// led
			nullptr,
			// precedence
			pr::precedence::none,
			// can start
			true,
			// is prefix
			false,
			// is infix
			false,
			// is postfix
			false
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
			true,
			// is prefix
			false,
			// is infix
			false,
			// is postfix
			false
		},


		// -- numbers ---------------------------------------------------------
		// note,
		rule_number(),
		// binary,
		rule_number(),
		// octal,
		rule_number(),
		// decimal,
		rule_number(),
		// hexadecimal,
		rule_number(),


		// () [] {}
		// priority_open,
		{
			// nud
			&pr::parser::nud_group,
			// led
			nullptr,
			// precedence
			pr::precedence::grouping,
			// can start
			true,
			//L == pr::level::block,
			// is prefix
			false,
			// is infix
			false,
			// is postfix
			false
		},
		// priority_close,
		pr::rule_empty(),

		// permutation_open,
		{
			// nud
			&pr::parser::nud_permutation,
			// led
			nullptr,
			// precedence
			pr::precedence::grouping,
			// can start
			true,
			// is prefix
			false,
			// is infix
			false,
			// is postfix
			false
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
			// can start
			true,
			// is prefix
			false,
			// is infix
			false,
			// is postfix
			false
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
	inline auto nud_of(const tk::token& token) noexcept -> pr::rule::nud_type {
		return rules[token.id].nud;
	}

	/* led of */
	inline auto led_of(const tk::token& token) noexcept -> pr::rule::led_type {
		return rules[token.id].led;
	}

	/* precedence of */
	inline auto pre_of(const tk::token& token) noexcept -> pr::precedence {
		return rules[token.id].pre;
	}

	/* can start */
	inline auto can_start(const tk::token& token) noexcept -> bool {
		return rules[token.id].can_start;
	}

} // namespace pr

#endif // language_parser_table_hpp
