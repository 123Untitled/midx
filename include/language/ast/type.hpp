#ifndef language_ast_type_hpp
#define language_ast_type_hpp

#include "core/types.hpp"

// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- T Y P E -------------------------------------------------------------

	enum class type : mx::uint {
		dummy,
		track,
		parameter,
		sequence,
		leaf,
		parallel,
		crossfade,
		permutation
	};

} // namespace as

#endif // language_ast_type_hpp
