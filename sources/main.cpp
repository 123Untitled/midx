#include <iostream>
#include "application.hpp"


// NOTE, TRIG, GATE, OCTAVE, VELOCITY, PROB, SEMITONE, SWING, CONDITION, CHANNEL
// sequence, repeat, linked

// TRACK
// trig, note, gate, octave, velocity, prob, semitone, swing, condition, channel

// PATTERN
// section, length

// SONG
// timeline, start, loop

// A (B (C _ D)) E F _ _ _


auto main(int ac, char** av) -> int {

	try {
		// clear screen
		std::cout << "\x1b[2J\x1b[H\x1b[31m--- program started ---\x1b[0m\n";

		ml::application::run(av[1U]);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
