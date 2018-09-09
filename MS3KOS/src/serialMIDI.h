
// MIDI features can be configured in or out of
// the project according to Serial needs, and can be
// enabled for WiFi.
// e.g. debugging
//

#ifdef CONFIG_MIDI_SERIAL_ENABLE

#include "msSystem.h"

#include "serialMIBYConfig.h"

// Current View per MIDI input
typedef struct {
	uint8_t midi_channel;		// MIDI channel of View
	uint16_t time_base;			// Base Time for sequencer-Put
	void *v_arg;				// user data
} MIDIViewT;

MIDIViewT curr_midiview;

miby_t miby_serial;
uint8_t midi_in_byte;

// -- MIDI I/O
// Send a serial MIDI message
// returns the # of bytes sent
static uint16_t SERIAL_MIDI_Put(uint8_t * data, uint16_t length)
{
	return (Serial.write(data, length));
};

// Receive a MIDI message if its available
// returns the # of bytes received in the message (count should be length)
static uint16_t SERIAL_MIDI_Get(uint8_t * data, uint16_t length)
{
	int8_t count;
	count = 0;

	while ( (Serial.available()) && 
		    ((length > 0) && 
		     (data[count] = Serial.read()) != -1) ) {
		
		count++;
		length--;
	}

	return count;
};

static void SERIAL_MIDI_loop()
{
		// this is the per-frame midi byte
	if (Serial.available()) {
		SERIAL_MIDI_Get(&midi_in_byte, 1);
		// msSystem.msLEDs.setLED(LED_NOTE_EVENT, 100, 100, 100, msGlobals.ggBrightness);
		miby_parse(&miby_serial, midi_in_byte);
	} else {
		// msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 0, msGlobals.ggBrightness);
	}

}


// handlers
void SERIAL_MIDI_Start(miby_this_t a_miby)
{
	msSystem.slogln("serialMIDI: Start");
	msGlobals.ggModeList[msGlobals.ggCurrentMode]->MIDI_Start();
}

void SERIAL_MIDI_Stop(miby_this_t a_miby)
{
	msSystem.slogln("serialMIDI: Stop");
	msGlobals.ggModeList[msGlobals.ggCurrentMode]->MIDI_Stop();
}

void SERIAL_MIDI_Program_Change(miby_this_t a_miby)
{
	msSystem.slogln("serialMIDI: ProgramChange");
	msGlobals.ggModeList[msGlobals.ggCurrentMode]->MIDI_Program_Change(
							MIBY_CHAN(a_miby), MIBY_ARG0(a_miby));
}

void SERIAL_MIDI_Control_Change(miby_this_t a_miby)
{
	msSystem.slogln("serialMIDI: ControlChange");
	msGlobals.ggModeList[msGlobals.ggCurrentMode]->MIDI_Control_Change(
		MIBY_CHAN(a_miby), 
		MIBY_ARG0(a_miby),
		MIBY_ARG1(a_miby));
}

void SERIAL_MIDI_Note_On(miby_this_t a_miby)
{
	msSystem.slogln("serialMIDI: NoteOn");
	msGlobals.ggModeList[msGlobals.ggCurrentMode]->MIDI_Note_On(
		MIBY_CHAN(a_miby),
		MIBY_ARG0(a_miby),
		MIBY_ARG1(a_miby));
}

void SERIAL_MIDI_Note_Off(miby_this_t a_miby)
{
	msSystem.slogln("serialMIDI: NoteOff");
	msGlobals.ggModeList[msGlobals.ggCurrentMode]->MIDI_Note_Off(
		MIBY_STATUSBYTE(a_miby),
		MIBY_ARG0(a_miby),
		MIBY_ARG1(a_miby));
}

#endif
