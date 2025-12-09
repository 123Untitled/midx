#include <iostream>
#include "application.hpp"

#include "midi/midi.hpp"



auto main(int ac, char** av) -> int {


	//mx::midi md;
	//
	//while (true) {
	//	::sleep(1);
	//}
	//return 0;


	try {
		mx::application::run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
