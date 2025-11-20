#ifndef language_parser_levels_hpp
#define language_parser_levels_hpp

#include "core/types.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	/* level */
	enum level : mx::uint {
		expression,
		sequence,
		max_levels
	};

} // namespace pr

#endif // language_parser_levels_hpp
