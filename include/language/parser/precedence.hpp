#ifndef language_parser_precedence_hpp
#define language_parser_precedence_hpp

#include "core/types.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- P R E C E D E N C E -------------------------------------------------

	enum precedence : mx::uint {
		none            = 0U,

		separator       = 5U,
		track_separator = 7U,

		parameter       = 10U,

		parallel        = 20U,
		crossfade       = 30U,

		tempo           = 50U,
		modulo          = 50U,

		grouping        = 80U,

		value           = 90U,
		reference       = 95U,
	};

} // namespace pr

#endif // language_parser_precedence_hpp
