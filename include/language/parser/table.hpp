#ifndef language_parser_table_hpp
#define language_parser_table_hpp

#include "language/tokens/def.hpp"
#include "language/parser/rules.hpp"
#include "language/parser.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	/* level */
	enum class level : mx::uint {
		scope,
		block,
		param,
		value
	};



	/* number */
	template <pr::level L>
	consteval auto rule_number(void) -> pr::rule {
		return pr::rule {
			.nud = &pr::parser::nud_value<L>,
			.led = nullptr,
			.pre = pr::precedence::none,
			.can_start = true, //L == pr::level::value,
			.is_infix   = false,
			.is_prefix  = false,
			.is_postfix = false
		};
	}

	/* empty */
	template <pr::level L>
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
	template <pr::level L>
	consteval auto rule_tempo(void) -> pr::rule {
		return pr::rule {
			.nud = &pr::parser::nud_tempo<L>,
			.led = nullptr,
			.pre = pr::precedence::tempo,
			.can_start  = L != pr::level::scope,
			.is_infix   = false,
			.is_prefix  = true,
			.is_postfix = false
		};
	}


	// -- R U L E S -----------------------------------------------------------

	template <pr::level L>
	constexpr pr::rule rules[tk::max_tokens] {


		// identifier
		{
			// nud
			nullptr,
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
		pr::rule_empty<L>(),

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

		// block start
		{
			// nud
			nullptr,
			// led
			&pr::parser::led_tracksep<L>,
			// precedence
			pr::precedence::tracksep,
			// can start
			true,
			// is prefix
			false,
			// is infix
			true,
			// is postfix
			false
		},

		// parameter
		{
			// nud
			&pr::parser::nud_parameter<L>,
			// led
			nullptr,
			// precedence
			pr::precedence::parameter,
			// can start
			true,
			// is prefix
			true,
			// is infix
			false,
			// is postfix
			false
		},

		// block reference
		{
			// nud
			&pr::parser::nud_value<L>,
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
			&pr::parser::nud_value<L>,
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
			// can start
			L == pr::level::block,
			// is prefix
			false,
			// is infix
			false,
			// is postfix
			false
		},
		// priority_close,
		pr::rule_empty<L>(),

		// permutation_open,
		{
			// nud
			&pr::parser::nud_permutation<L>,
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
		pr::rule_empty<L>(),

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
