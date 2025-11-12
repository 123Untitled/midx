#include "system/file.hpp"

// -- F I L E -----------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------


// -- public conversion operators ---------------------------------------------

/* int conversion operator */
mx::file::operator int(void) const noexcept {
	return mx::unix_descriptor::operator int();
}
