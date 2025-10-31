#ifndef language_lexer_states_hpp
#define language_lexer_states_hpp

#include "language/lexer/table.hpp"
#include "macros.hpp"


// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {



	// -- S T A T E -----------------------------------------------------------

	template <ml::literal N, lx::is_rule... Rs>
	class state {
		public:
			non_instantiable_class(state);
			static constexpr const char* name = N.data;
			static constexpr lx::table  table = lx::make_table<Rs...>();
	}; // class state





	class on_default;

	class on_return;
	class on_comment;

	class on_identifier;
	class on_string;

	class on_digit;
	class on_zero;
	class on_note;



	class on_default : public lx::state<"on_default",

		// invalid
		lx::rule<cc::char_set<cc::control,
							  cc::upper_other,
							  cc::extended,
							  cc::punct>,
					&lx::lexer::push_error<"invalid character">>,

		// blank
		lx::rule<cc::blank>,

		// line feed
		lx::rule<cc::line_feed,
					&lx::lexer::newline>,

		// carriage return
		lx::rule<cc::carriage_return,
					&lx::lexer::newline,
					&lx::lexer::switch_state<lx::on_return>>,


		// comment
		lx::rule<cc::number_sign,
					&lx::lexer::checkpoint,
					&lx::lexer::switch_state<lx::on_comment>>,

		// lowercase letters
		lx::rule<cc::lower,
					&lx::lexer::checkpoint,
					&lx::lexer::switch_state<lx::on_identifier>>,

		// single quote
		lx::rule<cc::single_quote,
					&lx::lexer::checkpoint,
					&lx::lexer::switch_state<lx::on_string>>,


		// digits
		lx::rule<cc::digit,
					// add digit
					&lx::lexer::switch_state<lx::on_digit>>,

		// zero digit
		lx::rule<cc::digit_zero,
					&lx::lexer::switch_state<lx::on_zero>>,

		// note
		lx::rule<cc::note,
					&lx::lexer::switch_state<lx::on_note>>,



		// bracket open
		lx::rule<cc::bracket_open,
					&lx::lexer::push_token_byte<tk::bracket_open>>,

		// bracket close
		lx::rule<cc::bracket_close,
					&lx::lexer::push_token_byte<tk::bracket_close>>,

		// at sign
		lx::rule<cc::at_sign,
					&lx::lexer::push_token_byte<tk::at_sign>>,

		// slash
		lx::rule<cc::slash,
					&lx::lexer::push_token_byte<tk::slash>>,

		// dot
		lx::rule<cc::dot,
					&lx::lexer::push_token_byte<tk::dot>>

	>{};


	class on_return : public lx::state<"on_return",

		// invalid
		lx::rule<cc::char_set<cc::control,
							  cc::upper_other,
							  cc::extended,
							  cc::punct>,
					&lx::lexer::push_error<"invalid character">>,

		// blank
		lx::rule<cc::blank,
					&lx::lexer::switch_state<lx::on_default>>,

		// line feed
		lx::rule<cc::line_feed,
					&lx::lexer::switch_state<lx::on_default>>,

		// carriage return
		lx::rule<cc::carriage_return,
					&lx::lexer::newline>,

		// comment
		lx::rule<cc::number_sign,
					&lx::lexer::checkpoint,
					&lx::lexer::switch_state<lx::on_comment>>,


		// lowercase letters
		lx::rule<cc::lower,
					&lx::lexer::checkpoint,
					&lx::lexer::switch_state<lx::on_identifier>>,


		// digits
		lx::rule<cc::digit,
					// add digit
					&lx::lexer::switch_state<lx::on_digit>>,

		// zero digit
		lx::rule<cc::digit_zero,
					&lx::lexer::switch_state<lx::on_zero>>,

		// note
		lx::rule<cc::note,
					&lx::lexer::switch_state<lx::on_note>>,


		// bracket open
		lx::rule<cc::bracket_open,
					&lx::lexer::push_token_byte<tk::bracket_open>,
					&lx::lexer::switch_state<lx::on_default>>,

		// bracket close
		lx::rule<cc::bracket_close,
					&lx::lexer::push_token_byte<tk::bracket_close>,
					&lx::lexer::switch_state<lx::on_default>>,

		// at sign
		lx::rule<cc::at_sign,
					&lx::lexer::push_token_byte<tk::at_sign>,
					&lx::lexer::switch_state<lx::on_default>>,

		// slash
		lx::rule<cc::slash,
					&lx::lexer::push_token_byte<tk::slash>,
					&lx::lexer::switch_state<lx::on_default>>,

		// dot
		lx::rule<cc::dot,
					&lx::lexer::push_token_byte<tk::dot>,
					&lx::lexer::switch_state<lx::on_default>>

	>{};


	// -- O N  C O M M E N T --------------------------------------------------

	class on_comment : public lx::state<"on_comment",

		// invalid
		lx::rule<cc::char_set<cc::control>,
					&lx::lexer::push_error<"invalid character">>,

		// line feed
		lx::rule<cc::line_feed,
					&lx::lexer::push_token<tk::comment>,
					&lx::lexer::newline,
					&lx::lexer::switch_state<lx::on_default>>,

		// carriage return
		lx::rule<cc::carriage_return,
					&lx::lexer::push_token<tk::comment>,
					&lx::lexer::newline,
					&lx::lexer::switch_state<lx::on_return>>
	>{};


	class on_identifier : public lx::state<"on_identifier",

		// invalid
		lx::rule<cc::char_set<cc::control,
							  cc::upper_other,
							  cc::extended,
							  cc::punct>,
					&lx::lexer::push_error<"invalid character">>,

		// blank
		lx::rule<cc::blank,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_default>>,

		// line feed
		lx::rule<cc::line_feed,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::newline,
					&lx::lexer::switch_state<lx::on_default>>,

		// carriage return
		lx::rule<cc::carriage_return,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::newline,
					&lx::lexer::switch_state<lx::on_return>>,


		// comment
		lx::rule<cc::number_sign,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_comment>,
					&lx::lexer::checkpoint>,

		// single quote
		lx::rule<cc::single_quote,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_string>,
					&lx::lexer::checkpoint>,


		// digits
		lx::rule<cc::digit,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_digit>,
					// add digit
					&lx::lexer::checkpoint>,

		// zero digit
		lx::rule<cc::digit_zero,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_zero>,
					&lx::lexer::checkpoint>,

		// note
		lx::rule<cc::note,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_note>,
					&lx::lexer::checkpoint>,



		// bracket open
		lx::rule<cc::bracket_open,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::bracket_open>,
					&lx::lexer::switch_state<lx::on_default>>,

		// bracket close
		lx::rule<cc::bracket_close,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::bracket_close>,
					&lx::lexer::switch_state<lx::on_default>>,

		// at sign
		lx::rule<cc::at_sign,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::at_sign>,
					&lx::lexer::switch_state<lx::on_default>>,

		// slash
		lx::rule<cc::slash,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::slash>,
					&lx::lexer::switch_state<lx::on_default>>,

		// dot
		lx::rule<cc::dot,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::dot>,
					&lx::lexer::switch_state<lx::on_default>>

	>{};

	class on_zero : public lx::state<"on_zero"
	>{};

	class on_digit : public lx::state<"on_digit"
	>{};

	class on_note : public lx::state<"on_note",

		// invalid
		lx::rule<cc::char_set<cc::control,
							  cc::upper_other,
							  cc::extended,
							  cc::punct>,
					&lx::lexer::push_error<"invalid character">>,

		// blank
		lx::rule<cc::blank,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_default>>,

		// line feed
		lx::rule<cc::line_feed,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::newline,
					&lx::lexer::switch_state<lx::on_default>>,

		// carriage return
		lx::rule<cc::carriage_return,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::newline,
					&lx::lexer::switch_state<lx::on_return>>,


		// comment
		lx::rule<cc::number_sign,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_comment>,
					&lx::lexer::checkpoint>,

		// single quote
		lx::rule<cc::single_quote,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_string>,
					&lx::lexer::checkpoint>,


		// digits
		lx::rule<cc::digit,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_digit>,
					// add digit
					&lx::lexer::checkpoint>,

		// zero digit
		lx::rule<cc::digit_zero,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_zero>,
					&lx::lexer::checkpoint>,

		// note
		lx::rule<cc::note,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::switch_state<lx::on_note>,
					&lx::lexer::checkpoint>,



		// bracket open
		lx::rule<cc::bracket_open,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::bracket_open>,
					&lx::lexer::switch_state<lx::on_default>>,

		// bracket close
		lx::rule<cc::bracket_close,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::bracket_close>,
					&lx::lexer::switch_state<lx::on_default>>,

		// at sign
		lx::rule<cc::at_sign,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::at_sign>,
					&lx::lexer::switch_state<lx::on_default>>,

		// slash
		lx::rule<cc::slash,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::slash>,
					&lx::lexer::switch_state<lx::on_default>>,

		// dot
		lx::rule<cc::dot,
					&lx::lexer::push_token<tk::identifier>,
					&lx::lexer::push_token_byte<tk::dot>,
					&lx::lexer::switch_state<lx::on_default>>
	>{};


	// -- O N  S T R I N G ----------------------------------------------------

	class on_string : public lx::state<"on_string",


		// invalid
		lx::rule<cc::control,
					&lx::lexer::push_error<"invalid character">>,


		// line feed
		lx::rule<cc::line_feed,
					&lx::lexer::push_error<"unterminated string">,
					&lx::lexer::push_token<tk::string>,
					&lx::lexer::switch_state<lx::on_default>,
					&lx::lexer::newline>,

		// carriage return
		lx::rule<cc::carriage_return,
					&lx::lexer::push_error<"unterminated string">,
					&lx::lexer::push_token<tk::string>,
					&lx::lexer::switch_state<lx::on_return>,
					&lx::lexer::newline>,

		// single quote
		lx::rule<cc::single_quote,
					&lx::lexer::push_token<tk::string, 1>,
					&lx::lexer::switch_state<lx::on_default>>
	>{};









	//template <lx::action... As>
	//struct action_list final {
	//	non_instantiable_class(action_list);
	//}; // struct action_list
	//
	//
	//struct initial_state final {
	//	public:
	//		non_instantiable_class(initial_state);
	//		using lead_set = lx::rule_list<
	//								lx::rule<cc::bracket_open, &lx::lexer::push_token_byte<tk::bracket_open>>,
	//								lx::rule<cc::bracket_close, &lx::lexer::push_token_byte<tk::bracket_close>>
	//									>;
	//		using body_set = cc::empty;
	//		using stop_set = cc::empty;
	//		using on_lead  = lx::action_list<&lx::lexer::switch_state<lx::initial_state>>;
	//		using on_body  = lx::action_list<>;
	//		using on_stop  = lx::action_list<>;
	//}; // struct initial_state
	//
	//struct identifier_state final {
	//	public:
	//		non_instantiable_class(initial_state);
	//		using lead_set = lx::rule_list<lx::rule<cc::lower>>>;
	//		using body_set = cc::lower;
	//		using stop_set = cc::empty;
	//		using on_lead  = lx::action_list<&lx::lexer::switch_state<lx::identifier_state>>;
	//		using on_body  = lx::action_list<>;
	//		using on_stop  = lx::action_list<>;
	//}; // struct initial_state
	//
	//struct string_state final {
	//	public:
	//		non_instantiable_class(initial_state);
	//		using lead_set = lx::rule_list<lx::rule<cc::single_quote>>>;
	//		using body_set = cc::all;
	//		using stop_set = cc::char_set<cc::single_quote, cc::line_feed, cc::carriage_return>;
	//		using on_lead  = lx::action_list<&lx::lexer::switch_state<lx::string_state>>;
	//		using on_body  = lx::action_list<>;
	//		using on_stop  = lx::action_list<>;
	//}; // struct initial_state
	//
	//
	//struct identifier_state final {
	//	public:
	//		non_instantiable_class(identifier_state);
	//		using lead_set = cc::lower;
	//		using body_set = cc::lower;
	//		using stop_set = cc::empty;
	//		using on_lead  = lx::action_list<&lx::lexer::switch_state<lx::initial_state>,
	//										 &lx::lexer::checkpoint>;
	//		using on_body  = lx::action_list<>;
	//		using on_stop  = lx::action_list<&lx::lexer::push_token<tk::identifier>>;
	//}; // struct initial_state
	//
	//
	//template <typename S>
	//struct state_table {
	//	lx::action table[cc::char_map::size];
	//
	//	auto execute(lx::lexer& lxr, const ml::u8 byte) const /* noexcept */-> void {
	//		(lxr.*table[cc::char_map::table[byte]])();
	//	}
	//
	//}; // struct state_table
	//
	//
	//template <typename... Ss>
	//class state_machine final : public lx::state_table<Ss>... {
	//	public:
	//		constexpr state_machine(void) noexcept
	//		: lx::state_table<Ss>{}... {
	//		}
	//
	//
	//		template <typename S>
	//		auto get(void) const noexcept -> const lx::state_table<S>& {
	//			return static_cast<const lx::state_table<S>&>(*this);
	//		}
	//
	//}; // class state_machine
	//
	//
	//constexpr lx::state_machine<lx::initial_state, lx::identifier_state> sm{};


	//template <typename, lx::action...>
	//struct lead;
	//template <typename, lx::action...>
	//struct body;
	//template <typename, lx::action...>
	//struct stop;
	//
	//template <ml::u8... Is, lx::action... As>
	//struct lead<cc::char_class<Is...>, As...> {
	//	//non_instantiable_class(lead);
	//};
	//template <ml::u8... Is, lx::action... As>
	//struct body<cc::char_class<Is...>, As...> {
	//	//non_instantiable_class(body);
	//};
	//template <ml::u8... Is, lx::action... As>
	//struct stop<cc::char_class<Is...>, As...> {
	//	//non_instantiable_class(stop);
	//};
	//
	//template <typename...>
	//struct add;
	//
	//template <ml::u8... L,      ml::u8... B,       ml::u8... S,
	//		 lx::action... LAs, lx::action... BAs, lx::action... SAs>
	//struct add<lead<cc::char_class<L...>, LAs...>,
	//		   body<cc::char_class<B...>, BAs...>,
	//		   stop<cc::char_class<S...>, SAs...>> {
	//
	//	using lead = lx::lead<cc::char_class<L...>, LAs...>;
	//	using body = lx::body<cc::char_class<B...>, BAs...>;
	//	using stop = lx::stop<cc::char_class<S...>, SAs...>;
	//
	//	//non_instantiable_class(add);
	//};
	//
	//using s1 = lx::add<
	//	lx::lead<cc::lower, &lx::lexer::call<>>,
	//	lx::body<cc::digit, &lx::lexer::call<>>,
	//	lx::stop<cc::upper, &lx::lexer::call<>>
	//		>;
	//
	//using s2 = lx::add<
	//	lx::lead<cc::punct,    &lx::lexer::call<>>,
	//	lx::body<cc::control,  &lx::lexer::call<>>,
	//	lx::stop<cc::extended, &lx::lexer::call<>>
	//		>;
	//
	//
	//template <typename... Rs>
	//class state_machine final {
	//
	//	lx::action _table[sizeof...(Rs)][cc::char_map::size];
	//
	//	using table_type = decltype(_table);
	//
	//
	//	template <typename, lx::action...>
	//	struct config;
	//
	//	template <ml::u8... Ci, lx::action... As>
	//	struct config<lx::body<cc::char_class<Ci...>, As...>> {
	//
	//		template <ml::uint Si>
	//		static constexpr auto test(table_type& tbl) -> void {
	//			((tbl[Si][Ci] = &lx::lexer::call<As...>), ...);
	//		}
	//	};
	//
	//	template <ml::uint... Is>
	//	constexpr auto configure(const std::integer_sequence<ml::uint, Is...>&) noexcept -> void {
	//		(config<typename Rs::body>::template test<Is>(_table), ...);
	//	}
	//
	//	public:
	//
	//	constexpr state_machine(void) noexcept
	//	: _table{} {
	//		configure(std::make_integer_sequence<ml::uint, sizeof...(Rs)>{});
	//	}
	//
	//
	//}; // class state_machine
	//
	//
	//constexpr lx::state_machine<s1, s2> sm{};















} // namespace lx

#endif // language_lexer_states_hpp
