#include "system/file.hpp"

// -- F I L E -----------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------


// -- public conversion operators ---------------------------------------------

/* int conversion operator */
ml::file::operator int(void) const noexcept {
	return ml::unix_descriptor::operator int();
}
