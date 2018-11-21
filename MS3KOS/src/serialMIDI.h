
// MIDI features can be configured in or out of
// the project according to Serial needs, and can be
// enabled for WiFi.
// e.g. debugging
//

#ifndef _SERIALMIDI_H_
#define  _SERIALMIDI_H_

#ifdef CONFIG_ENABLE_MIDI_SERIAL

#include "msSystem.h"
#include "serialMIBYConfig.h"

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


extern uint16_t SERIAL_MIDI_Put(uint8_t * data, uint16_t length);
extern uint16_t SERIAL_MIDI_Get(uint8_t * data, uint16_t length);
extern void SERIAL_MIDI_init();
extern void SERIAL_MIDI_loop();

extern void SERIAL_MIDI_Start(miby_this_t a_miby);
extern void SERIAL_MIDI_Stop(miby_this_t a_miby);
extern void SERIAL_MIDI_Program_Change(miby_this_t a_miby);
extern void SERIAL_MIDI_Control_Change(miby_this_t a_miby);
extern void SERIAL_MIDI_Note_On(miby_this_t a_miby);
extern  void SERIAL_MIDI_Note_Off(miby_this_t a_miby);

#endif

#endif // _SERIALMIDI_H_
