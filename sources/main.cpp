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

// 5. Dithering avec Floyd-Steinberg (diffusion d'erreur)

std::vector<bool> floydSteinbergCrossfade(int length) {
	std::vector<bool> result;
	result.reserve(length);

	double error = 0.0;

	for (int i = 0; i < length; ++i) {
		// Densité cible à cette position
		double target = static_cast<double>(i) / (length - 1);

		// Ajouter l'erreur propagée
		double value = target + error;

		if (value >= 0.5) {
			result.push_back(true);
			error = value - 1.0;  // Propager l'erreur
		} else {
			result.push_back(false);
			error = value;
		}
	}

	return result;
}

// 6. Euclidean Rhythm (Bjorklund's Algorithm)

// Distribue k événements sur n steps de façon maximalement équilibrée :

std::vector<bool> euclideanCrossfade(int length) {
	std::vector<bool> result(length, false);

	// Nombre de B augmente progressivement
	// On fait plusieurs passes avec Bjorklund
	std::vector<int> counts;
	std::vector<int> remainders;

	int divisor = length;
	int k = (length + 1) / 2;  // environ la moitié

	remainders.push_back(k);
	int level = 0;

	do {
		counts.push_back(divisor / remainders[level]);
		remainders.push_back(divisor % remainders[level]);
		divisor = remainders[level];
		level++;
	} while (remainders[level] > 1);

	counts.push_back(divisor);

	// Construire le pattern
	int index = 0;
	auto build = [&](auto& self, int level) -> void {
		if (level == -1) {
			result[index++] = false;
		} else if (level == -2) {
			result[index++] = true;
		} else {
			for (int i = 0; i < counts[level]; i++) {
				self(self, level - 1);
			}
			if (remainders[level] != 0) {
				self(self, level - 2);
			}
		}
	};

	build(build, level - 1);

	return result;
}

// 7. Blue Noise / Low-Discrepancy Sequence

// Basé sur la suite de van der Corput :

std::vector<bool> blueNoiseCrossfade(int length) {
	std::vector<bool> result;
	result.reserve(length);

	auto vanDerCorput = [](int n, int base = 2) -> double {
		double q = 0.0;
		double bk = 1.0 / base;
		while (n > 0) {
			q += (n % base) * bk;
			n /= base;
			bk /= base;
		}
		return q;
	};

	for (int i = 0; i < length; ++i) {
		double threshold = static_cast<double>(i) / (length - 1);
		double noise = vanDerCorput(i);

		result.push_back(noise < threshold);
	}

	return result;
}

// 8. Stochastic Rounding (arrondi stochastique)

#include <random>
std::vector<bool> stochasticCrossfade(int length, unsigned seed = 42) {
	std::vector<bool> result;
	result.reserve(length);

	std::mt19937 gen(seed);
	std::uniform_real_distribution<> dis(0.0, 1.0);

	for (int i = 0; i < length; ++i) {
		double target = static_cast<double>(i) / (length - 1);
		double random = dis(gen);

		result.push_back(random < target);
	}

	return result;
}

// 9. Poisson Disk Sampling (spacing contraint)

// Force un espacement minimum entre les transitions :

std::vector<bool> poissonDiskCrossfade(int length, int min_spacing = 2) {
	std::vector<bool> result(length, false);

	int last_b_position = -min_spacing - 1;
	double accumulated = 0.0;

	for (int i = 0; i < length; ++i) {
		double density = static_cast<double>(i) / (length - 1);
		accumulated += density;

		// Vérifier l'espacement minimum
		bool can_place = (i - last_b_position) > min_spacing;

		if (accumulated >= 1.0 && can_place) {
			result[i] = true;
			accumulated -= 1.0;
			last_b_position = i;
		}
	}

	return result;
}

// 10. Wavefront / Sigmoid Transition

// Transition en forme de S (smooth step) :

std::vector<bool> sigmoidCrossfade(int length) {
	std::vector<bool> result;
	result.reserve(length);

	double accumulated = 0.0;

	for (int i = 0; i < length; ++i) {
		double t = static_cast<double>(i) / (length - 1);

		// Smooth step: 3t² - 2t³
		double smoothstep = t * t * (3.0 - 2.0 * t);

		// Ou smoother step: 6t⁵ - 15t⁴ + 10t³
		// double smootherstep = t * t * t * (t * (t * 6.0 - 15.0) + 10.0);

		accumulated += smoothstep;

		if (accumulated >= 1.0) {
			result.push_back(true);
			accumulated -= 1.0;
		} else {
			result.push_back(false);
		}
	}

	return result;
}

// 11. Golden Ratio / Fibonacci Spiral

std::vector<bool> goldenRatioCrossfade(int length) {
	std::vector<bool> result;
	result.reserve(length);

	const double PHI = 1.618033988749895;  // golden ratio
	const double INV_PHI = 1.0 / PHI;

	double accumulated = 0.0;

	for (int i = 0; i < length; ++i) {
		double target = static_cast<double>(i) / (length - 1);

		// Modulation par le ratio d'or
		accumulated += target * INV_PHI;

		if (accumulated >= INV_PHI) {
			result.push_back(true);
			accumulated -= INV_PHI;
		} else {
			result.push_back(false);
		}
	}

	return result;
}

// 12. Perlin Noise Threshold

std::vector<bool> perlinCrossfade(int length) {
	std::vector<bool> result;
	result.reserve(length);

	// Simplified 1D Perlin-like interpolation
	auto smoothNoise = [](double x) -> double {
		int xi = static_cast<int>(x);
		double xf = x - xi;

		// Cubic interpolation
		return xf * xf * (3.0 - 2.0 * xf);
	};

	for (int i = 0; i < length; ++i) {
		double t = static_cast<double>(i) / (length - 1);
		double noise = smoothNoise(t * 3.0);  // frequency control

		double threshold = t * 0.7 + noise * 0.3;  // 70% linear, 30% noise

		result.push_back(threshold > 0.5);
	}

	return result;
}



auto main(int ac, char** av) -> int {

	//auto N = 32;
	////auto result = progressiveCrossfade(N);
	//auto result = floydSteinbergCrossfade(N);
	//
	//for (auto n : result) {
	//	if (!n)
	//		std::cout << "\x1b[32mA \x1b[0m";
	//	else
	//		std::cout << "\x1b[31mB \x1b[0m";
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
