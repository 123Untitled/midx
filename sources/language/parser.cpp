#include "language/parser.hpp"


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::parser::parser(void)
: _context{} {
}


// -- public modifiers --------------------------------------------------------

auto ml::parser::parse(const ml::mapped_file& mf, ml::project& data) -> void {

	// initialize context
	_context.initialize(mf, data);

	// parse !
	_driver.parse(_context);
}
