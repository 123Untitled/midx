#ifndef language_syntax_block_hpp
#define language_syntax_block_hpp

#include "language/syntax/param_list.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- B L O C K -----------------------------------------------------------

	template <typename T,
			  size_type NBLOCK_ALIASES,
			  size_type NPARAMS,
			  size_type NPARAM_ALIASES>
	struct block final {


		public:

			// -- public members ----------------------------------------------

			/* block specifier */
			sx::entry<T, NBLOCK_ALIASES> specifier;

			/* block params */
			sx::param_list<T, NPARAMS, NPARAM_ALIASES> params;


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			block(void) = delete;

			/* entry / param_list constructor */
			constexpr block(const sx::entry<T, NBLOCK_ALIASES>& spec,
							const sx::param_list<T, NPARAMS, NPARAM_ALIASES>& ps) noexcept
			: specifier{spec}, params{ps} {
			}

	}; // class block


	// -- deduction guides ----------------------------------------------------

	template <typename T, size_type NBA, size_type NP, size_type NPA>
	block(const sx::entry<T, NBA>&,
		  const sx::param_list<T, NP, NPA>&) -> block<T, NBA, NP, NPA>;

} // namespace sx

#endif // language_syntax_block_hpp
