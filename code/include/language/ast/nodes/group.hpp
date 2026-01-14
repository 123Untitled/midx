#ifndef language_ast_nodes_group_hpp
#define language_ast_nodes_group_hpp

#include "language/ast/header.hpp"
#include "language/ast/remap_range.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	struct play_ctx;


	// -- G R O U P -----------------------------------------------------------

	struct group final {


		public:

			// -- public members ----------------------------------------------

			/* header */
			as::header header;

			/* range */
			as::remap_range range;


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			group(void) = delete;

			/* constructor */
			group(const as::remap_range& range,
				  const mx::frac& dur) noexcept
			: header{as::type::group, dur},
			  range{range} {
			}

	}; // struct group


} // namespace as

#endif // language_ast_nodes_group_hpp
