#ifndef language_syntax_param_list_hpp
#define language_syntax_param_list_hpp

#include "language/syntax/entry.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- P A R A M  L I S T --------------------------------------------------

	template <size_type NPARAMS, size_type NALIASES>
	class param_list final {


		public:

			const char* _aliases[NALIASES];
			pr::action  _actions[NPARAMS];
			size_type   _offsets[NPARAMS + 1U];


			template <size_type NEA>
			constexpr auto flatten(const sx::entry<NEA>& e,
								   size_type& alias_index,
								   size_type& param_index
								   ) noexcept -> void {

				_offsets[param_index] = alias_index;
				_actions[param_index] = e.action;
				++param_index;

				for (size_type i = 0U; i < NEA; ++i, ++alias_index)
					_aliases[alias_index] = e.aliases[i];
			}

			template <size_type... NEA>
			constexpr param_list(const sx::entry<NEA>&... es) noexcept
			: _aliases{}, _actions{}, _offsets{} {

				size_type alias_index = 0U;
				size_type param_index = 0U;
				((flatten<NEA>(es, alias_index, param_index)), ...);
				_offsets[NPARAMS] = alias_index;
			}


	}; // class param_list


	// -- deduction guides ----------------------------------------------------

	template <size_type... NEA>
	param_list(const sx::entry<NEA>&...) -> param_list<sizeof...(NEA), (NEA + ... )>;

} // namespace sx

#endif // language_syntax_param_list_hpp
