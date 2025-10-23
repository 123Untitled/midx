#include <iostream>
#include "application.hpp"

namespace ml {


}


auto main(int ac, char** av) -> int {

	try {
		std::cout << "\x1b[31m--- program started ---\x1b[0m\n";

		ml::unix_descriptor fd{ml::open("./hello.msh", O_RDONLY)};
		ml::mapped_file mf{fd};
		ml::project p;
		p.update(mf);

		return 0;


		if (ac != 2) {
			std::cerr << "Usage: " << av[0] << " <file-path>\n";
			return 1;
		}
		ml::application::run(av[1U]);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
