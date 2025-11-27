#include <iostream>
#include "application.hpp"


// Algorithme de base : crossfade linéaire progressif
std::vector<bool> progressiveCrossfade(int length) {
    std::vector<bool> result;
    result.reserve(length);

    double accumulated = 0.0;
    
    for (int i = 0; i < length; ++i) {
        // Densité linéaire de 0 à 1
        double density = (length > 1) ? static_cast<double>(i) / (length - 1) : 0.0;
        
        // Accumuler la densité
        accumulated += density;
        
        // Placer un 1 si l'accumulateur dépasse 1.0
        if (accumulated >= 1.0) {
            result.push_back(true);  // 1 = jouer séquence B
            accumulated -= 1.0;      // Réinitialiser l'erreur
        } else {
            result.push_back(false); // 0 = jouer séquence A
        }
    }
    
    return result;
}



auto main(int ac, char** av) -> int {

	//auto N = 32;
	//auto result = progressiveCrossfade(N);
	//
	//for (auto n : result) {
	//	std::cout << n;
	//}
	//std::cout << '\n';
	//
	//return 0;

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
