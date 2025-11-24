#include <iostream>
#include "application.hpp"


auto main(int ac, char** av) -> int {

	try {
		//mx::frac f1{12,4};
		//mx::frac f2{5,6};
		//
		//auto r = mx::frac_mod(f1, f2);
		//std::cout << "mod: " << r.num << "/" << r.den << "\n";
		//
		//return 0;


		mx::application::run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
