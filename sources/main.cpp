#include <iostream>
#include "application.hpp"



auto main(int ac, char** av) -> int {

	//auto bpm = bpm_to_frac(122.33);
	//std::cout << "BPM frac: " << bpm << '\n';
	//return 0;
	//cm::sources();
	//return 0;

	const auto& tb = mx::timebase::info();

	UInt64 num = tb.numer * 1'000'000'000ULL; // nanoseconds per second
	UInt64 den = tb.denom * 48000ULL;         // samples per second

	std::cout << "Timebase info:\n";
	std::cout << "  Numerator:   " << tb.numer << '\n';
	std::cout << "  Denominator: " << tb.denom << '\n';
	std::cout << "  Frac num:    " << num << '\n';
	std::cout << "  Frac den:    " << den << '\n';

    //return mx::frac{
    //    static_cast<uint64_t>(tb._timebase.denom) * 1'000'000'000ULL,
    //    static_cast<uint64_t>(tb._timebase.numer) * sample_rate
    //};
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
