
// MIDI features can be configured in or out of
// the project according to Serial needs, and can be
// enabled for WiFi.
// e.g. debugging
//

#ifndef _SERIALMIDI_H_
#define  _SERIALMIDI_H_

// mode
void SERIAL_MIDI_init();
void SERIAL_MIDI_loop();

// i/o
uint16_t SERIAL_MIDI_Put(uint8_t * data, uint16_t length);
uint16_t SERIAL_MIDI_Get(uint8_t * data, uint16_t length);

//  events
void SERIAL_MIDI_Start(miby_this_t a_miby);
void SERIAL_MIDI_Stop(miby_this_t a_miby);
void SERIAL_MIDI_Program_Change(miby_this_t a_miby);
void SERIAL_MIDI_Control_Change(miby_this_t a_miby);
void SERIAL_MIDI_Note_On(miby_this_t a_miby);
void SERIAL_MIDI_Note_Off(miby_this_t a_miby);

#endif // _SERIALMIDI_H_
