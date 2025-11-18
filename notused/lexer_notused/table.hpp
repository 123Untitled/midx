#ifndef language_lexer_table_hpp
#define language_lexer_table_hpp

#include "language/char_map.hpp"
#include "language/lexer.hpp"


// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {


	// -- R U L E -------------------------------------------------------------

	template <typename, lx::action...>
	struct rule;

	template <mx::u8... Is, lx::action... As>
	struct rule<cc::char_class<Is...>, As...> final {
		non_instantiable_class(rule);
	};

	namespace impl {

		template <typename T>
		struct is_rule final {
			static constexpr bool value = false;
			non_instantiable_class(is_rule);
		}; // struct is_rule

		template <typename C, lx::action... As>
		struct is_rule<lx::rule<C, As...>> final {
			static constexpr bool value = true;
			non_instantiable_class(is_rule);
		}; // struct is_rule
	} // namespace impl

	/* is rule */
	template <typename T>
	concept is_rule = lx::impl::is_rule<T>::value;



	// -- T A B L E -----------------------------------------------------------

	class table final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = lx::table;


			// -- private members ---------------------------------------------

			/* action table */
			lx::action _table[cc::char_map::size];


			template <typename>
			struct config;

			template <mx::u8... Is, lx::action... As>
			struct config<lx::rule<cc::char_class<Is...>, As...>> final {
				non_instantiable_class(config);
				static consteval auto apply(self& tbl) noexcept -> void {
					((tbl._table[Is] = &lx::lexer::call<As...>), ...);
				}
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* variadic constructor */
			consteval explicit table(void) noexcept
			/* uninitialized */ {

				// initialize table
				for (auto& a : _table)
					a = &lx::lexer::call<>;
			}


			auto execute(lx::lexer& lxr, const mx::u8 byte) const /* noexcept */-> void {
				(lxr.*_table[cc::char_map::table[byte]])();
			}

			auto end_of_file(lx::lexer& lxr) const /* noexcept */ -> void {
				(lxr.*_table[cc::char_map::eof])();
			}

			template <lx::is_rule...>
			friend consteval auto make_table(void) noexcept -> self;

	}; // class table

	/* make table */
	template <lx::is_rule... Tp>
	consteval auto make_table(void) noexcept -> lx::table {
		lx::table tbl;
		((lx::table::config<Tp>::apply(tbl)), ...);
		return tbl;
	}


} // namespace lx

#endif // language_lexer_table_hpp













	//// -- R O U T E -----------------------------------------------------------
	//
	//template <lx::action... As>
	//struct begin final {
	//	non_instantiable_class(begin);
	//}; // struct begin
	//
	//
	//template <typename, lx::action...>
	//struct process;
	//
	//template <mx::u8... Is, lx::action... As>
	//struct process<cc::char_class<Is...>, As...> final {
	//	non_instantiable_class(process);
	//}; // struct in_state
	//
	//
	//template <typename... Ps>
	//struct process_list final {
	//	non_instantiable_class(process_list);
	//};
	//
	//
	//
	//
	//template <lx::action... As>
	//struct end final {
	//	non_instantiable_class(end);
	//}; // struct end
	//
	//template <typename C, typename S, lx::action... As>
	//struct route final {
	//	non_instantiable_class(route);
	//}; // struct route


	//// -- T A B L E -----------------------------------------------------------
	//
	//class table final {
	//
	//
	//	private:
	//
	//		// -- private types -----------------------------------------------
	//
	//		/* self type */
	//		using self = lx::table;
	//
	//
	//		// -- private members ---------------------------------------------
	//
	//		/* action table */
	//		lx::action _table[cc::char_map::size];
	//
	//
	//	public:
	//
	//		// -- public lifecycle --------------------------------------------
	//
	//		/* variadic constructor */
	//		consteval explicit table(void) noexcept
	//		/* uninitialized */ {
	//
	//			// initialize table
	//			for (auto& a : _table)
	//				a = &lx::lexer::call<>;
	//		}
	//
	//
	//		auto execute(lx::lexer& driver, const mx::u8 byte) const /* noexcept */-> void {
	//			(driver.*_table[cc::char_map::table[byte]])();
	//		}
	//
	//		auto end_of_file(lx::lexer& driver) const /* noexcept */ -> void {
	//			(driver.*_table[cc::char_map::eof])();
	//		}
	//
	//
	//
	//	  // lx::on_enter<&lx::lexer::checkpoint>,
	//	  // lx::in_state<cc::lower>,
	//	  // lx::on_leave<&lx::lexer::push_token<tk::identifier>>,
	//	  // lx::route<cc::digit,   lx::digits_state>,
	//	  // lx::route<cc::control, lx::initial_state>
	//
	//		template <typename BEGIN, typename END, typename PROCESS, typename... ROUTES>
	//		class builder final {
	//
	//
	//
	//			private:
	//
	//				template <typename...>
	//				struct config_route;
	//
	//				template <mx::u8... Is, typename STATE>
	//				struct config_route<lx::route<cc::char_class<Is...>, STATE>> final {
	//
	//
	//					template <typename...>
	//					struct merge;
	//
	//					template <lx::action... EAs, lx::action... BAs>
	//					struct merge<lx::end<EAs...>, lx::begin<BAs...>> final {
	//
	//						static constexpr lx::action actions =
	//							&lx::lexer::call<
	//							EAs...,
	//							&lx::lexer::switch_state<STATE>, 
	//							BAs...
	//							>;
	//
	//						non_instantiable_class(merge);
	//					};
	//
	//
	//					static consteval auto fill(lx::action (&out)[cc::char_map::size]) noexcept -> void {
	//
	//
	//						((out[Is] = merge<END, typename STATE::begin>::actions
	//						), ...);
	//					}
	//				};
	//
	//
	//
	//				template <typename...>
	//				struct config_process;
	//
	//				template <typename... Ps>
	//				struct config_process<lx::process_list<Ps...>> final {
	//					static consteval auto fill(lx::action (&out)[cc::char_map::size]) noexcept -> void {
	//						((config_process<Ps>::fill(out)), ...);
	//					}
	//				};
	//
	//				template <mx::u8... Is, lx::action... As>
	//				struct config_process<lx::process<cc::char_class<Is...>, As...>> final {
	//					static consteval auto fill(lx::action (&out)[cc::char_map::size]) noexcept -> void {
	//						((out[Is] = &lx::lexer::call<As...>), ...);
	//					}
	//				};
	//
	//
	//
	//			public:
	//
	//				static consteval auto build(void) noexcept -> lx::table {
	//					lx::table t;
	//					config_process<PROCESS>::fill(t._table);
	//					((config_route<ROUTES>::fill(t._table)), ...);
	//					return t;
	//				}
	//
	//		}; // class table_builder
	//
	//}; // class action_table
	//
	///* make table */
	//template <typename... Tp>
	//consteval auto make_table(void) noexcept -> lx::table {
	//	return lx::table::builder<Tp...>::build();
	//}



	//template <mx::literal NAME,
	//		  typename BEGIN, typename END, typename PROCESS,
	//		  typename... ROUTES>
	//class state {
	//	public:
	//		using begin   = BEGIN;
	//		using end     = END;
	//		using process = PROCESS;
	//		non_instantiable_class(state);
	//		static constexpr const char* name = NAME.data;
	//		static constexpr lx::table  table = lx::make_table<
	//			begin,
	//			end,
	//			process,
	//			ROUTES...
	//		>();
	//}; // class state
	//
	//
	//
	//
	//class digits_state;
	//class initial_state;
	//class identifier_state;
	//class comment_state;
	//
	//
	//class return_state;
	//
	//
	//class initial_state final : public lx::state<"initial",
	//	lx::begin<>,
	//	lx::end<>,
	//	lx::process_list<
	//		// will be overridden by other processes
	//		lx::process<cc::all,           &lxr::push_error<"invalid character">>,
	//		lx::process<cc::line_feed,     &lxr::newline>,
	//		lx::process<cc::bracket_open,  &lxr::push_token_byte<tk::bracket_open>>,
	//		lx::process<cc::bracket_close, &lxr::push_token_byte<tk::bracket_close>>,
	//		lx::process<cc::at_sign,       &lxr::push_token_byte<tk::at_sign>>,
	//		lx::process<cc::slash,         &lxr::push_token_byte<tk::slash>>,
	//		lx::process<cc::dot,           &lxr::push_token_byte<tk::dot>>
	//	>,
	//	lx::route<cc::carriage_return, lx::return_state>,
	//	lx::route<cc::lower,           lx::identifier_state>,
	//	lx::route<cc::number_sign,     lx::comment_state>
	//>{};
	//
	//
	//class identifier_state final : public lx::state<"indentifier",
	//	lx::begin<&lxr::checkpoint>,
	//	lx::end<&lxr::push_token<tk::identifier>>,
	//	lx::process_list<>,
	//	lx::route<cc::line_feed, lx::initial_state /* newline*/>,
	//	lx::route<cc::carriage_return, lx::return_state>
	//>{};
	//
	//class return_state final : public lx::state<"return",
	//	lx::begin<&lxr::newline>,
	//	lx::end<>,
	//	lx::process_list<
	//			lx::process<cc::carriage_return, &lx::lexer::newline>
	//	>,
	//	lx::route<cc::line_feed, lx::initial_state>
	//>{};
	//
	//
	//class comment_state final : public lx::state<"comment",
	//
	//	lx::begin<&lx::lexer::checkpoint>,
	//	  lx::end<&lx::lexer::push_token<tk::comment>>,
	//
	//	lx::process_list<>,
	//
	//	lx::route<cc::line_feed, lx::initial_state /* newline*/>,
	//	lx::route<cc::carriage_return, lx::return_state>
	//>{};

	// -- forward declarations ------------------------------------------------

	//class initial;
	//class invalid;
	//
	//class on_single_quote;
	//class in_string;
	//class in_identifier;
	//
	//class in_comment;
	//class on_return;
	//
	//class on_zero;
	//class on_digit;
	//class on_note;


	// test prototype
	//using identifier_state = lx::state<"identifier",
	//	lx::on_enter<&lx::lexer::checkpoint>,
	//	lx::in_state<&lx::count>,
	//	lx::on_leave<&lx::lexer::push_token<tk::identifier>>,
	//	lx::routes<
	//		lx::route<cc::digit, lx::lexer::switch_state<lx::in_digit>,
	//		lx::route<cc::other, lx::lexer::switch_state<lx::initial>
	//	>
	//>;



	// -- I N I T I A L -------------------------------------------------------

	//class initial final : public lx::state<"initial",
	//
	//
	//	// control characters
	//	lx::add<cc::all,
	//		&lx::lexer::push_error<"invalid character">
	//	>,
	//
	//	// line feed
	//	lx::add<
	//		cc::line_feed,
	//		&lx::lexer::newline
	//	>,
	//
	//	// carriage return
	//	lx::add<
	//		cc::carriage_return,
	//		&lx::lexer::newline,
	//		&lx::lexer::switch_state<lx::on_return>
	//	>,
	//
	//	// lowercase letters
	//	lx::add<
	//		cc::lower,
	//		&lx::lexer::checkpoint,
	//		&lx::lexer::switch_state<lx::in_identifier>
	//	>,
	//
	//	// space
	//	lx::add<cc::blank>,
	//
	//
	//	// string
	//	lx::add<
	//		cc::single_quote,
	//		&lx::lexer::checkpoint,
	//		&lx::lexer::switch_state<lx::in_string>
	//	>,
	//
	//	// comment
	//	lx::add<
	//		cc::number_sign,
	//		&lx::lexer::checkpoint,
	//		&lx::lexer::switch_state<lx::in_comment>
	//	>,
	//
	//
	//	// -- punctuation -----------------------------------------------------
	//
	//	// open bracket
	//	lx::add<
	//		cc::bracket_open,
	//		&lx::lexer::push_token_byte<tk::bracket_open>
	//	>,
	//	// close bracket
	//	lx::add<
	//		cc::bracket_close,
	//		&lx::lexer::push_token_byte<tk::bracket_open>
	//	>,
	//
	//	// at sign
	//	lx::add<
	//		cc::at_sign,
	//		&lx::lexer::push_token_byte<tk::at_sign>
	//	>,
	//
	//	// slash
	//	lx::add<
	//		cc::slash,
	//		&lx::lexer::push_token_byte<tk::slash>
	//	>,
	//	// dot
	//	lx::add<
	//		cc::dot,
	//		&lx::lexer::push_token_byte<tk::dot>
	//	>,
	//
	//
	//
	//	// digits
	//	lx::add<
	//		cc::digit,
	//		&lx::lexer::checkpoint,
	//		&lx::lexer::switch_state<lx::on_digit>
	//	>,
	//
	//	// zero
	//	lx::add<
	//		cc::digit_zero,
	//		&lx::lexer::checkpoint,
	//		&lx::lexer::switch_state<lx::on_zero>
	//	>,
	//
	//	// note
	//	lx::add<
	//		cc::note,
	//		&lx::lexer::checkpoint,
	//		&lx::lexer::switch_state<lx::on_note>
	//	>
	//> {};
	//
	//
	//class in_identifier final : public lx::state<"initial",
	//
	//
	//	// control characters
	//	lx::add<
	//		cc::control,
	//		&lx::lexer::push_error<"invalid character">,
	//		&lx::lexer::push_token<tk::identifier>
	//	>,
	//
	//	// line feed
	//	lx::add<
	//		cc::line_feed,
	//		&lx::lexer::push_token<tk::identifier>,
	//		&lx::lexer::newline
	//	>,
	//
	//	// carriage return
	//	lx::add<
	//		cc::carriage_return,
	//		&lx::lexer::push_token<tk::identifier>,
	//		&lx::lexer::newline,
	//		&lx::lexer::switch_state<lx::on_return>
	//	>,
	//
	//
	//	// other characters
	//	lx::add<
	//		cc::char_set<cc::horizontal_tab,
	//					 cc::space,
	//					 cc::punct,
	//					 cc::digit,
	//					 cc::upper,
	//					 cc::extended>,
	//		&lx::lexer::push_token<tk::identifier>,
	//		&lx::lexer::switch_state<lx::initial>
	//	>
	//
	//> {}; // class in_identifier
	//
	//
	//
	//
	//class on_return final : public lx::extend_state<lx::initial::base,
	//												"on_return",
	//
	//	// line feed
	//	lx::add<
	//		cc::line_feed,
	//		&lx::lexer::switch_state<lx::initial>
	//	>,
	//
	//	// carriage return
	//	lx::add<
	//		cc::carriage_return,
	//		&lx::lexer::newline
	//	>
	//
	//> {}; // class on_return
	//
	//
	//// -- I N  S T R I N G ----------------------------------------------------
	//
	//class in_string final : public lx::state<"in_string",
	//
	//
	//	// valid string characters
	//	lx::add<
	//		cc::char_set<cc::control,
	//					 cc::horizontal_tab,
	//					 cc::space,
	//					 cc::graph,
	//					 cc::extended>,
	//		&lx::lexer::count
	//	>,
	//
	//	// single quote
	//	lx::add<
	//		cc::single_quote,
	//		&lx::lexer::push_token<tk::string, 1U>, // maybe +1 to include quote
	//		&lx::lexer::switch_state<lx::initial>
	//	>,
	//
	//	// line feed
	//	lx::add<
	//		cc::line_feed,
	//		&lx::lexer::push_token<tk::string>,
	//		&lx::lexer::push_error<"unterminated string">,
	//		&lx::lexer::newline,
	//		&lx::lexer::switch_state<lx::initial>
	//	>,
	//
	//	// carriage return
	//	lx::add<
	//		cc::carriage_return,
	//		&lx::lexer::push_token<tk::string>,
	//		&lx::lexer::push_error<"unterminated string">,
	//		&lx::lexer::newline,
	//		&lx::lexer::switch_state<lx::on_return>
	//	>
	//
	//> {}; // class on_single_quote
	//
	//
	//
	//class in_comment final : public lx::state<"in_comment",
	//
	//	// valid comment characters
	//	lx::add<
	//		cc::char_set<cc::control,
	//					 cc::horizontal_tab,
	//					 cc::space,
	//					 cc::graph,
	//					 cc::extended>
	//	>,
	//
	//	// line feed
	//	lx::add<
	//		cc::line_feed,
	//		&lx::lexer::push_token<tk::comment>,
	//		&lx::lexer::newline,
	//		&lx::lexer::switch_state<lx::initial>
	//	>,
	//
	//	// carriage return
	//	lx::add<
	//		cc::carriage_return,
	//		&lx::lexer::push_token<tk::comment>,
	//		&lx::lexer::newline,
	//		&lx::lexer::switch_state<lx::on_return>
	//	>
	//
	//> {}; // class in_comment
	//
	//
	//class on_zero final : public lx::extend_state<lx::initial::base,
	//										  "on_zero"
	//> {};
	//class on_digit final : public lx::extend_state<lx::initial::base,
	//										  "on_zero"
	//> {};
	//class on_note final : public lx::extend_state<lx::initial::base,
	//										  "on_zero"
	//> {};
