#include <iostream>
#include "application.hpp"


#include <CoreMIDI/CoreMIDI.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>

auto handle_midi_message(const mx::u8* data, const mx::usz length) -> void {
	std::cout << "MIDI Message: ";
	for (mx::usz i = 0; i < length; ++i) {
		std::cout << std::hex << static_cast<mx::u32>(data[i]) << " ";
	}
	std::cout << std::dec << "\n";
}

// Callback appelé quand un message MIDI arrive
void midiReadCallback(const MIDIPacketList *packetList, void *readProcRefCon, void *srcConnRefCon)
{
	const ::MIDIPacket *packet = &packetList->packet[0];
    for (cm::u32 i = 0; i < packetList->numPackets; i++) {

		handle_midi_message(packet->data, packet->length);

        packet = ::MIDIPacketNext(packet);
    }
}

int main2()
{
    MIDIClientRef client;
    MIDIClientCreate(CFSTR("MIDI Client"), NULL, NULL, &client);

    MIDIEndpointRef source = MIDIGetSource(0);
    if (source == 0) {
        printf("Aucune source MIDI trouvée.\n");
        return 1;
    }

    MIDIPortRef inputPort;
    MIDIInputPortCreate(client,
                        CFSTR("Input Port"),
                        midiReadCallback,
                        NULL,
                        &inputPort);

    MIDIPortConnectSource(inputPort, source, NULL);

    printf("Lecture MIDI… (Ctrl+C pour quitter)\n");

    // Boucle principale
	while (true) {
		::sleep(1);
	}

    return 0;
}


#include "core/string/string.hpp"
#include "core/string/strcmp.hpp"

auto main(int ac, char** av) -> int {


	//const char* ptr = "Hello";
	//
	//mx::string str{ptr, ' ', "World", 42};
	//std::cout << str.data() << "\n";
	//str.append(' ', 3.14159f, "!", '\n');
	//
	//if (str < 'a') {
	//}
	const char* s1 = "hello";
	const char* s2 = "hello2";

	if (s1 < s2) {
		std::cout << "hello < hello2\n";
	}

	//return 0;

	//mx::frac time{0, MIDI_PPQN};
	//const mx::frac tick{1U, MIDI_PPQN};
	//const mx::frac prev = time - tick;
	//
	//std::cout << "Time: " << time << "\n";
	//std::cout << "Prev: " << prev << "\n";
	//std::cout << "Tick: " << tick << "\n";

	try {
		mx::application::run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
