#include <iostream>
#include "application.hpp"


auto main(int ac, char** av) -> int {

	//constexpr std::size_t N1 = 9;
	//constexpr std::size_t N2 = 9;
	//constexpr std::size_t N  = (N1 > N2) ? N1 : N2;
	//
	//std::array<char, N1> s1;
	//std::array<char, N2> s2;
	//std::array<char, N > s3;
	//
	//// crossfade from s1 to s2
	//// 0 is s1 only
	//// 1 is s2 only
	//// expected output 0 0 1 0 0 1 0 1 1
	//
	//double err = 0.0;
	//
 //   for (std::size_t i = 0; i < N; ++i) {
 //       // progression de 0 à 1
 //       double t = (N > 1)
 //           ? static_cast<double>(i) / static_cast<double>(N - 1)
 //           : 1.0; // cas N == 1
	//
 //       double v = t + err;
	//
 //       if (v >= 0.5) {
 //           s3[i] = 1;
 //           err   = v - 1.0;
 //       } else {
 //           s3[i] = 0;
 //           err   = v;
 //       }
 //   }


	//// print result
	//for (std::size_t i = 0; i < N; ++i) {
	//	std::cout << static_cast<int>(s3[i]) << " ";
	//}
	//std::cout << "\n";
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
