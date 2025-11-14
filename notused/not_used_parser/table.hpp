#ifndef language_parser_table_hpp
#define language_parser_table_hpp

#include "language/tokens.hpp"
#include "language/parser.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- R U L E -------------------------------------------------------------

	template <typename, pr::action...>
	struct rule;

	template <mx::u8... Is, pr::action... As>
	struct rule<tk::raw::token_class<Is...>, As...> final {
		non_instantiable_class(rule);
	};

	namespace impl {

		template <typename T>
		struct is_rule final {
			static constexpr bool value = false;
			non_instantiable_class(is_rule);
		}; // struct is_rule

		template <typename T, pr::action... As>
		struct is_rule<pr::rule<T, As...>> final {
			static constexpr bool value = true;
			non_instantiable_class(is_rule);
		}; // struct is_rule
	} // namespace impl

	/* is rule */
	template <typename T>
	concept is_rule = pr::impl::is_rule<T>::value;



	// -- T A B L E -----------------------------------------------------------

	class table final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = pr::table;


			// -- private members ---------------------------------------------

			/* action table */
			pr::action _table[tk::parsing_token_count];


			template <typename>
			struct config;

			template <mx::u8... Is, pr::action... As>
			struct config<pr::rule<tk::raw::token_class<Is...>, As...>> final {
				non_instantiable_class(config);
				static consteval auto apply(self& tbl) noexcept -> void {
					((tbl._table[Is] = &pr::parser::call<As...>), ...);
				}
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* variadic constructor */
			consteval explicit table(void) noexcept
			/* uninitialized */ {

				// initialize table
				for (auto& a : _table)
					a = &pr::parser::call<>;
			}


			auto execute(pr::parser& psr, const tk::raw::token& to) const /* noexcept */-> void {
				(psr.*_table[to.type])();
			}

			auto end_of_tokens(pr::parser& psr) const /* noexcept */ -> void {
				(psr.*_table[tk::end_of_tokens::id])();
			}

			template <pr::is_rule...>
			friend consteval auto make_table(void) noexcept -> self;

	}; // class table

	/* make table */
	template <pr::is_rule... Rs>
	consteval auto make_table(void) noexcept -> pr::table {
		pr::table tbl;
		((pr::table::config<Rs>::apply(tbl)), ...);
		return tbl;
	}

} // namespace pr

#endif // language_parser_table_hpp
