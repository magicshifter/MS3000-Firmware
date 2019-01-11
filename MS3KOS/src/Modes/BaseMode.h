#ifndef _MAGICSHIFTERBASEMODE_H
#define _MAGICSHIFTERBASEMODE_H


// #include "msSystem.h"

// A MagicShifter mode can be activated by the user through system-events, i.e. buttons, shaking, etc.
// start happens when the user selects the mode
// stop happens when the user switches to another mode, before any other modes are started
// step allows the mode time to itself to compute, do and schedule things

class MagicShifterBaseMode {

  private:

  public:
  	// a unique name for the  mode
	String modeName = "BaseMode";
	// start the mode

	// void start(MagicShifterSystem &msSystem)
	virtual void start() {};

	// stop the mode 
	virtual void stop(void) {};

	// step through a frame of the mode 
	virtual bool step() { return false; };

	// emit the current UI of the mode 
	//virtual bool emit() {};

	// MIDI virtual methods


#ifdef CONFIG_MIDI
	virtual void MIDI_Reset() {};
	virtual void MIDI_Start() {};
	virtual void MIDI_Stop() {};
	virtual void MIDI_Continue() {};
	virtual void MIDI_ActiveSensing() {};
	virtual void MIDI_Program_Change(byte channel, byte program) {};
	virtual void MIDI_Control_Change(byte channel, byte cc1, byte cc2) {};
	virtual void MIDI_Note_On(byte channel, byte note, byte velocity) {};
	virtual void MIDI_Note_Off(byte channel, byte note, byte velocity) {};
	virtual void MIDI_Aftertouch_Poly(byte channel, byte note, byte velocity) {};
	virtual void MIDI_Aftertouch_Channel(byte channel, byte program) {};
	virtual void MIDI_PitchBend(byte channel, byte bend) {};
	virtual void MIDI_SYSEX(const byte *data, uint16_t size) {};
	virtual void MIDI_TimeCodeQuarterFrame(byte data) {};
	virtual void MIDI_SongPosition(unsigned short beats) {};
	virtual void MIDI_SongSelect(byte songnumber) {};
	virtual void MIDI_TuneRequest() {};
	virtual void MIDI_Clock() {};
#endif
	
	// !J! TODO: Other MIDI fn''s..

};

#endif
