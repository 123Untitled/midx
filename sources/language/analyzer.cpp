#include "language/analyzer.hpp"


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::analyzer::analyzer(void)
: _context{}, _lexer{} {
}


// -- public modifiers --------------------------------------------------------

auto ml::analyzer::analyze(const ml::mapped_file& mf, ml::project& data) -> void {

	// initialize context
	_context.initialize(mf);

	// lex !
	_lexer.lex2(_context);
}
