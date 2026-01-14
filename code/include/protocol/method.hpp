#ifndef protocol_method_hpp
#define protocol_method_hpp

#include "core/types.hpp"
#include "core/string/string_view.hpp"


// -- P C  N A M E S P A C E --------------------------------------------------

namespace pc {


	// -- M E T H O D ---------------------------------------------------------

	enum class method : mx::u32 {
		update,
		play,
		stop,
		toggle,
		state,
		unknown,
		max_types
	};


	/* method name */
	inline auto method_name(const pc::method m) noexcept -> const mx::string_view& {

		static constexpr mx::string_view methods[] {
			mx::string_view{"update"},
			mx::string_view{"play"},
			mx::string_view{"stop"},
			mx::string_view{"toggle"},
			mx::string_view{"state"},
			mx::string_view{"unknown"}
		};

		return methods[static_cast<mx::usz>(m)];
	}

} // namespace pc

#endif // protocol_method_hpp
