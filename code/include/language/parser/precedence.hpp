#ifndef language_parser_precedence_hpp
#define language_parser_precedence_hpp

#include "core/types.hpp"
#include "language/parser/levels.hpp"


// -- P R  N A M E S P A C E --------------------------------------------------

namespace pr {


	// -- P R E C E D E N C E -------------------------------------------------

	enum class precedence_default : mx::uint {
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

	constexpr mx::uint precedence_none = 0U;

	template <pr::level>
	struct precedence;

	template <>
	struct precedence<pr::level::expr> {
		enum : mx::uint {
			none      = 0,
			separator = 0,
			track_separator = 7,
			parallel  = 20,
			crossfade = 30,
			tempo     = 60,
			modulo    = 60,
			parameter = 70,
			grouping  = 80,
			value     = 90,
			reference = 95,
		};
	};

	template <>
	struct precedence<pr::level::seq> {
		enum : mx::uint {
			none      = 0,
			separator = 0,
			track_separator = 7,
			parameter = 10,
			parallel  = 20,
			crossfade = 30,
			tempo     = 60,
			modulo    = 60,
			grouping  = 80,
			value     = 90,
			reference = 95,
		};
	};



} // namespace pr

#endif // language_parser_precedence_hpp
