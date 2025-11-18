#include "language/parser/rules.hpp"
#include "language/parser.hpp"


// -- R U L E S ---------------------------------------------------------------

//template <pr::level L>
//constinit pr::rule<L> pr::rule<L>::_rules[tk::max_tokens] {
//
//
//	// identifier
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		true, // maybe false because identifiers are at global level only
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//
//	// assignment =
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::assignment,
//		// can start
//		false,
//		// is prefix
//		false,
//		// is infix
//		true,
//		// is postfix
//		false
//	},
//
//	// separator ;
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		false,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//
//
//	// tempo ^2 \2
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::tempo,
//		// can start
//		true,
//		// is prefix
//		true,
//		// is infix
//		false,
//		// is postfix
//		false,
//	},
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::tempo,
//		// can start
//		true,
//		// is prefix
//		true,
//		// is infix
//		false,
//		// is postfix
//		false,
//	},
//
//	// parallel
//	{
//		// nud
//		nullptr,
//		// led
//		&pr::parser::led_parallel,
//		// precedence
//		pr::precedence::parallel,
//		// can start
//		false,
//		// is prefix
//		false,
//		// is infix
//		true,
//		// is postfix
//		false
//	},
//
//	// crossfade
//	{
//		// nud
//		nullptr,
//		// led
//		&pr::parser::led_crossfade,
//		// precedence
//		pr::precedence::crossfade,
//		// can start
//		false,
//		// is prefix
//		false,
//		// is infix
//		true,
//		// is postfix
//		false
//	},
//
//	// block start
//	{
//		// nud
//		nullptr,
//		// led
//		&pr::parser::led_tracksep,
//		// precedence
//		pr::precedence::tracksep,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		true,
//		// is postfix
//		false
//	},
//
//	// parameter
//	{
//		// nud
//		&pr::parser::nud_parameter,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::parameter,
//		// can start
//		true,
//		// is prefix
//		true,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//
//	// block reference
//	{
//		// nud
//		&pr::parser::nud_value,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//
//	// param reference
//	{
//		// nud
//		&pr::parser::nud_value,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//
//
//	// numbers
//	// note,
//	{
//		// nud
//		&pr::parser::nud_value,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// binary,
//	{
//		// nud
//		&pr::parser::nud_value,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// octal,
//	{
//		// nud
//		&pr::parser::nud_value,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// decimal,
//	{
//		// nud
//		&pr::parser::nud_value,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// hexadecimal,
//	{
//		// nud
//		&pr::parser::nud_value,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//
//
//	// () [] {}
//	// priority_open,
//	{
//		// nud
//		&pr::parser::nud_group,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::grouping,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// priority_close,
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		false,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// permutation_open,
//	{
//		// nud
//		&pr::parser::nud_permutation,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::grouping,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// permutation_close,
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		false,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// condition_open,
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::grouping,
//		// can start
//		true,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//	// condition_close,
//	{
//		// nud
//		nullptr,
//		// led
//		nullptr,
//		// precedence
//		pr::precedence::none,
//		// can start
//		false,
//		// is prefix
//		false,
//		// is infix
//		false,
//		// is postfix
//		false
//	},
//
//
//	// comment
//	{
//		nullptr,
//		nullptr,
//		pr::precedence::none,
//		false,
//		false,
//		false,
//		false
//	},
//
//	// invalid
//	{
//		nullptr,
//		nullptr,
//		pr::precedence::none,
//		false,
//		false,
//		false,
//		false
//	},
//
//	// end of tokens
//	{
//		nullptr,
//		nullptr,
//		pr::precedence::none,
//		false,
//		false,
//		false,
//		false
//	}
//};



	// + - * /
	// add
	//{
	//	// parse function
	//	nullptr,
	//	// precedence
	//	pr::precedence::term,
	//	// can start
	//	true,
	//	// is prefix
	//	true,
	//	// is infix
	//	true,
	//	// is postfix
	//	false
	//},
	//// subtract
	//{
	//	// parse function
	//	nullptr,
	//	// precedence
	//	pr::precedence::term,
	//	// can start
	//	true,
	//	// is prefix
	//	true,
	//	// is infix
	//	true,
	//	// is postfix
	//	false
	//},
	//// multiply
	//{
	//	// parse function
	//	nullptr,
	//	// precedence
	//	pr::precedence::factor,
	//	// can start
	//	false,
	//	// is prefix
	//	false,
	//	// is infix
	//	true,
	//	// is postfix
	//	false
	//},
	//// divide
	//{
	//	// parse function
	//	nullptr,
	//	// precedence
	//	pr::precedence::factor,
	//	// can start
	//	false,
	//	// is prefix
	//	false,
	//	// is infix
	//	true,
	//	// is postfix
	//	false
	//},

