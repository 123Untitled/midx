#ifndef language_syntax_block_hpp
#define language_syntax_block_hpp

#include "language/syntax/param_list.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- B L O C K -----------------------------------------------------------

	template <size_type NBLOCK_ALIASES,
			  size_type NPARAMS,
			  size_type NPARAM_ALIASES>
	struct block final {


		public:

			// -- public members ----------------------------------------------

			/* block specifier */
			sx::entry<NBLOCK_ALIASES> specifier;

			/* block params */
			sx::param_list<NPARAMS, NPARAM_ALIASES> params;


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			block(void) = delete;

			/* entry / param_list constructor */
			constexpr block(const sx::entry<NBLOCK_ALIASES>& spec,
							const sx::param_list<NPARAMS, NPARAM_ALIASES>& ps) noexcept
			: specifier{spec}, params{ps} {
			}

	}; // class block


	// -- deduction guides ----------------------------------------------------

	template <size_type NBA, size_type NP, size_type NPA>
	block(const sx::entry<NBA>&,
		  const sx::param_list<NP, NPA>&) -> block<NBA, NP, NPA>;

} // namespace sx

#endif // language_syntax_block_hpp
