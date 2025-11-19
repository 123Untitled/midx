#ifndef language_parser_precedence_hpp
#define language_parser_precedence_hpp

#include "core/types.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- P R E C E D E N C E -------------------------------------------------

	enum precedence : mx::uint {
		none       = 0U,
		assignment = 10U,

		tracksep  = 15U,

		parallel   = 20U,
		crossfade  = 30U,

		parameter  = 40U,

		tempo      = 50U,

		grouping   = 80U
	};

} // namespace pr
		//sequence   = 55U,
		//term       = 60U,
		//factor     = 70U,

#endif // language_parser_precedence_hpp
