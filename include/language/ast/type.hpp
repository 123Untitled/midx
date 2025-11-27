#ifndef language_ast_type_hpp
#define language_ast_type_hpp

#include "core/types.hpp"

// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- T Y P E -------------------------------------------------------------

	enum class type : mx::u32 {

		program,

		group,

		track,
		parameter,
		parallel,
		crossfade,
		permutation,

		tempo,
		modulo,

		references,
		atomic_values,
	};


	// -- C A T E G O R Y -----------------------------------------------------

	enum class category : mx::u8 {
		none,
		generic,
		semantic
	};

	enum class shape : mx::u8 {
		none,
		unary,
		binary,
		ternary,
		n_ary
	};

} // namespace as

#endif // language_ast_type_hpp
