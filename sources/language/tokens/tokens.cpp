#include "language/tokens/tokens.hpp"
#include <iostream>
#include <iomanip>


// -- T O K E N S -------------------------------------------------------------



// -- public methods ----------------------------------------------------------

/* debug */
auto tk::tokens::debug(void) const -> void {

	std::cout << "\x1b[31m----[TOKEN LIST DEBUG]----\x1b[0m\n";
	for_each_filtered(
		[&](const tk::const_token_view& tv) -> void {
			std::cout << tv << '\n';
		}
	);
    std::cout << "\x1b[31m--------------------------\x1b[0m\n";
}





