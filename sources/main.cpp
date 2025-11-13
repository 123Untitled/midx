#include <iostream>
#include "application.hpp"



auto main(int ac, char** av) -> int {

	try {
		mx::application::run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
