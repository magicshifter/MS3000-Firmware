/*
 * MIDIMode.cpp
 * Created: Wed Jul  9 22:19:46 CEST 2014
 * Author: Jay Vaughan <ibisum@gmail.com
 *
 * A MIDI Processing mode for the MagicShifter
 *
 * Utilizes miby for MIDI Byte Parsing, from Neil Johnson  <http://www.njohnson.co.uk>
 *
 * NOTE: camelBack for types and functions, snake_case for vars, SCREAMING_SNAKE_CASE for
 * constant definitions
 *
 */

#ifdef CONFIG_ENABLE_MIDI_SERIAL
#include "serialMIDI.h"
#endif

// Status Indicator LED's - for the Arpeggiator, etc.
#define LED_BUTTON_EVENT 0
#define LED_MIDI1_EVENT 3
#define LED_MIDI2_EVENT 4
#define LED_NOTE_EVENT (8)
#define LED_CONTROL_CHANGE (10)
#define LED_ARP_COUNTER (13)
#define LED_MEASURE_COUNTER (14)
#define LED_BEAT_COUNTER (15)

// Arpeggiator constants -----------------------------------------------------------------
#define BUFFER_SIZE (64)
#define BUFFER_MASK (0x3F)
#define ARP_DURATION_FOR_BPM(v) (600000/v/2)
#define LOWEST_ARP_TEMPO (20)

#define MIDI_ARP_FEEDBACK 1

// A Basic Arpeggiator class for use by the MIDI Module:
// inspired by RobG @ 43oh
class MIDIArpeggiator {

private:
	// Arpeggiator structures:


	// ring buffer for queued events
	uint8_t arp_fifo[BUFFER_SIZE];
	uint8_t arp_fifo_in_index = 0;
	uint8_t arp_fifo_out_index = 0;

	// An arpeggiator event
	typedef struct {
		uint8_t beat;
		uint8_t note;
		uint8_t on_off;
	} ArpEventT;

	// A list of arpeggiator events (fixed size)
	ArpEventT arp_events[8];

	// Arpeggiator configuration state
	uint8_t current_pattern = 1;	// this will trigger loading for the first time.
	uint8_t current_note = 0;
	uint8_t new_note = 0;
	uint8_t current_velocity = 64;
	uint8_t new_velocity = 64;

	const uint8_t note_offset[12] = { 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0 };

	const uint8_t note_LUT[7][7] = {
		{0, 1, 2, 2, 3, 4, 5},
		{0, 1, 1, 2, 3, 4, 4},
		{0, 0, 1, 2, 3, 3, 4},
		{0, 1, 2, 3, 3, 4, 5},
		{0, 1, 2, 2, 3, 4, 4},
		{0, 1, 1, 2, 3, 3, 4},
		{0, 0, 1, 2, 2, 3, 4}
	};

	const uint8_t note_to_natural[12] = { 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6 };

	// A pattern of arpeggiator units
	typedef struct {
		uint8_t note_offset;
		uint8_t beat;
		uint8_t note_duration;
	} ArpPatternT;

#define NUM_ARP_PATTERNS 8
	const ArpPatternT arp_patterns[NUM_ARP_PATTERNS][4] = {	// arp_patterns 4/4
		{{0, BEAT1, EIGHTH}, {2, BEAT2, EIGHTH}, {0, BEAT3, EIGHTH}, {2, BEAT4, EIGHTH}},		//
		{{0, BEAT1, EIGHTH}, {0, BEAT2, EIGHTH}, {2, BEAT3, EIGHTH}, {2, BEAT4, EIGHTH}},		//
		{{0, BEAT1, EIGHTH}, {0, BEAT2, EIGHTH}, {2, BEAT3, EIGHTH}, {4, BEAT4, EIGHTH}},		//
		{{0, BEAT1, EIGHTH}, {2, BEAT2, EIGHTH}, {4, BEAT3, EIGHTH}, {6, BEAT4, EIGHTH}},		//
		{{0, BEAT1, EIGHTH}, {12, BEAT2, EIGHTH}, {0, BEAT3, EIGHTH}, {12, BEAT4, EIGHTH}},		//
		{{0, BEAT1, EIGHTH}, {0, BEAT2, EIGHTH}, {12, BEAT3, EIGHTH}, {12, BEAT4, EIGHTH}},		//
		{{0, BEAT1, EIGHTH}, {0, BEAT2, EIGHTH}, {4, BEAT3, EIGHTH}, {12, BEAT4, EIGHTH}},		//
		{{0, BEAT1, EIGHTH}, {2, BEAT1 + 8, EIGHTH}, {4, BEAT2, QUARTER}, {6, BEAT4, EIGHTH}},		//
	};


public:

	MIDIViewT curr_midiview;

	uint8_t new_pattern = 4;

	uint8_t arp_play_state = 0;
	uint8_t arp_frame = 0;			// internal beat, which is 1/16th of the actual beat (see arp_bpm)
	
	uint8_t arp_bpm = LOWEST_ARP_TEMPO;
	unsigned long arp_beat_duration = 0;
	unsigned long last_arp_frame_time = 0;


	void arpUIupdate()
	{
		// Blink Arpeggiator LED's
		if (arp_frame == 0) {
			msSystem.msLEDs.setLED(LED_BEAT_COUNTER, 100, 0, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.setLED(LED_MEASURE_COUNTER, 0, 100, 0, msGlobals.ggBrightness);
		} else if ((arp_frame & 0x0F) == 0) {
			msSystem.msLEDs.setLED(LED_BEAT_COUNTER, 0, 100, 0, msGlobals.ggBrightness);
		} else {					// LEDs off
			msSystem.msLEDs.setLED(LED_ARP_COUNTER, 100, 0, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.setLED(LED_BEAT_COUNTER, 0, 0, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.setLED(LED_MEASURE_COUNTER, 0, 0, 0, msGlobals.ggBrightness);
		}

	}

	void arpStart()
	{
		if (arp_play_state == 0) {
			arp_play_state = 1;
			arp_frame = 0;
		}
	}

	void arpStop()
	{
		if (arp_play_state == 1) {
			arp_play_state = 0;
			// arpSoundOff();
		}
	}

	// --------------------------------------------------------------------------- ARPEGGIATOR
	// Process a frame of the Arpeggiator
	void arpFrame()
	{

		uint8_t event_idx = 0;

// msSystem.slogln("MIDI: arpFrame!");

		// end of arp measure, go to the beginning
		if (arp_frame == 64) {
			arp_frame = 0;

			// finish current pattern before stopping
			if (arp_play_state == 0)
				return;
		}
		// handle changes in arp inputs - at the beginning of the measure...
		if (arp_frame == 0
			&& (new_note != current_note || new_pattern != current_pattern
				|| new_velocity != current_velocity)) {
			// ...update arp_events if anything has changed

			// 8 arp_events, 4 pairs of notes, one on and one off
			while (event_idx < 4) {
				// if you don't use getNextNaturalNote in arpNoteOn, use getNextNaturalNote
				// here instead
				uint8_t note = getNextNaturalNoteAdd(new_note,
													 arp_patterns[new_pattern]
													 [event_idx].note_offset);
				// on
				arp_events[event_idx].beat =
					arp_patterns[new_pattern][event_idx].beat;
				arp_events[event_idx].note = note;
				arp_events[event_idx].on_off = 1;

				// off
				// assuming that the beat and the duration is correctly calculated, sum
				// must be < 64
				arp_events[event_idx + 4].beat =
					arp_patterns[new_pattern][event_idx].beat +
					arp_patterns[new_pattern][event_idx].note_duration;

				arp_events[event_idx + 4].note = note;
				arp_events[event_idx + 4].on_off = 0;
				event_idx++;
			}

			// note any potential new pattern change
			current_pattern = new_pattern;
			current_note = new_note;
			current_velocity = new_velocity;

		}
		// process arp_events
		event_idx = 0;

		// go through all arp_events and play when event beat matches arp_frame
		while (event_idx < 8) {
// msSystem.slogln("arp event:" + String(event_idx));			
			if (arp_events[event_idx].beat == arp_frame) {
				arpPlayNote(arp_events[event_idx].note,
							arp_events[event_idx].on_off);
			}
			event_idx++;
		}


		// gather time data for next arp Frame
		arp_frame++;

		arpUIupdate();

	}

	void arpPlayNote(uint8_t noteNumber, uint8_t on_off)
	{
		arpPushMIDI((on_off ? MIDI_NOTE_ON : MIDI_NOTE_OFF) +
					curr_midiview.midi_channel);
		arpPushMIDI(noteNumber);
		arpPushMIDI(current_velocity);

msSystem.slogln("ARP: Note: " + String(noteNumber) + 
				" vel: " + String(current_velocity) + 
				" ON/OFF: " + String((on_off ? MIDI_NOTE_ON : MIDI_NOTE_OFF) + curr_midiview.midi_channel) );

		arpSendMIDI();
	}

	void arpSoundOff()
	{
		arpPushMIDI(MIDI_CONTROLLER + curr_midiview.midi_channel);
		arpPushMIDI(MIDI_ALL_NOTES_OFF);
		arpPushMIDI(0);

		arpSendMIDI();
	}

	void arpNoteOn(uint8_t noteChannel, uint8_t noteNumber, uint8_t velocity)
	{
		//new_note = noteNumber;
		new_note = getNextNaturalNote(noteNumber);
		new_velocity = velocity;
		if (arp_play_state == 0) {	// start if stopped
			arp_frame = 0;
			arp_play_state = 1;
		}
	}

	// push a MIDI event into the arpeggiator queue for later processing
	void arpPushMIDI(uint8_t data)
	{
		arp_fifo[++arp_fifo_in_index & BUFFER_MASK] = data;
	}

	// arpSendMIDI should be called after queueing up all messages and associated data for
	// those messages
	void arpSendMIDI()
	{
#ifdef CONFIG_ENABLE_MIDI_SERIAL
		while (arp_fifo_in_index != arp_fifo_out_index) {	// send whatever is in the buffer
			SERIAL_MIDI_Put(&arp_fifo[++arp_fifo_out_index & BUFFER_MASK], 1);
		}
#endif
	}

	// reeive a Program change to change Arp Pattern
	void arpProgramChange(uint8_t pcChannel, uint8_t number)
	{
		if (curr_midiview.midi_channel == pcChannel) {
			// only 8 preset arp_patterns right now so each program number is masked
			new_pattern = number & 0x07;
		}
	}

	// If the note is not a natural note, return next natural note
	uint8_t getNextNaturalNote(uint8_t note)
	{
		uint8_t offset = note;
		while (offset > 11) {
			offset -= 12;
		}
		return note + note_offset[offset];
	}

	// Calculate net natural note + offset
	uint8_t getNextNaturalNoteAdd(uint8_t naturalNote, uint8_t offset)
	{
		uint8_t n = naturalNote;
		while (n > 11) {
			n -= 12;
		}
		uint8_t o = offset;
		while (o > 6) {
			o -= 7;
		}
		return naturalNote + offset + note_LUT[note_to_natural[n]][o];
	}

};

class MIDIArpeggiatorMode : public MagicShifterBaseMode {

private:
	// miby parser is used


	// MIDI housekeeping
	uint8_t midi_mode = 0;			// Mode of this module (future-use)
	uint8_t sync_count;				// sync counter
	uint16_t midi_frame = 0;		// current MIDI Processing frame


	// !J! TODO : There should be a MagicShifter API for this
	// TODO: private state
	// state for button timing
	int msBtnAPressTime = 0;
	int msBtnPwrPressTime = 0;
	int msBtnBPressTime = 0;

	// state for double click timing
	int msBtnATTL = 0;
	int msBtnPwrTTL = 0;
	int msBtnBTTL = 0;

	bool msLongClickOK = true;

	bool msBtnAHit = false;
	bool msBtnPwrHit = false;
	bool msBtnBHit = false;

	bool msBtnALongHit = false;
	bool msBtnPwrLongHit = false;
	bool msBtnBLongHit = false;

	bool msBtnADoubleHit = false;
	bool msBtnPwrDoubleHit = false;
	bool msBtnBDoubleHit = false;

	int microsSinceLast = 0;

public:

	MIDIArpeggiator _arp;

	MIDIArpeggiatorMode() {
		modeName = "Arpi";
	}

	void handleButtons()
	{
		// reset public btton state
		msBtnAHit = msBtnALongHit = false;
		if (!digitalRead(PIN_BUTTON_A)) {
			if (msBtnAPressTime)
				msBtnAPressTime += microsSinceLast;
			else
				msBtnAPressTime = 1;
		} else {
			if (msLongClickOK && msBtnAPressTime >= MIN_TIME_LONG_CLICK) {
				msBtnALongHit = true;
				msSystem.slogln("msBtnALongHit");
			} else if (msBtnAPressTime >= MIN_TIME_CLICK) {
				msBtnAHit = true;
				msSystem.slogln("msBtnAHit");
			}

			msBtnAPressTime = 0;
		}


		// reset public btton state
		msBtnBHit = msBtnBLongHit = false;
		if (!digitalRead(PIN_BUTTON_B)) {
			if (msBtnBPressTime)
				msBtnBPressTime += microsSinceLast;
			else
				msBtnBPressTime = 1;
		} else {
			if (msLongClickOK && msBtnBPressTime >= MIN_TIME_LONG_CLICK) {
				msBtnBLongHit = true;
				msSystem.slogln("msBtnBLongHit");

			} else if (msBtnBPressTime >= MIN_TIME_CLICK) {
				msBtnBHit = true;
				msSystem.slogln("msBtnBHit");
			}

			msBtnBPressTime = 0;
		}


		// reset public btton state
		msBtnPwrHit = msBtnPwrLongHit = false;
	/*
	    if (msFrame++ % 10 == 0)
	    if (powerButtonPressed())
	    {
	      if (msBtnPwrPressTime)
	        msBtnPwrPressTime += microsSinceLast;
	      else
	        msBtnPwrPressTime = 1;
	    }
	    else
	    {
	      if (msBtnPwrPressTime >= MIN_TIME_LONG_CLICK)
	      {
	        msBtnPwrLongHit = true;
	      }
	      else if (msBtnPwrPressTime >= MIN_TIME_CLICK)
	      {
	        msBtnPwrHit = true;
	      }

	      msBtnPwrPressTime = 0;
	    }
	    //*/

	}

	void stop()
	{
		// !J! todo: all notes off
		_arp.arpSoundOff();
	}


	// ---------------------------------------------------------------------- Main Entry Point
	void start()
	{

		msSystem.slog("MIDIArpeggiator STARTED!");

		// Initial view
		_arp.curr_midiview.midi_channel = 0;


		// prime the Arp
		_arp.arp_beat_duration = ARP_DURATION_FOR_BPM(_arp.arp_bpm);
		_arp.last_arp_frame_time = micros();

		msSystem.slogln("MIDIArpeggiator START last_arp_frame_time:" + 
			String(_arp.last_arp_frame_time));

		_arp.arpFrame();

		_arp.arpSoundOff();

		_arp.arpStop();


	}

	void incPattern() {
		msSystem.msLEDs.setLED(_arp.new_pattern, 0, 0, 0, msGlobals.ggBrightness); 

		_arp.new_pattern ++;
		if (_arp.new_pattern > NUM_ARP_PATTERNS)
			_arp.new_pattern = NUM_ARP_PATTERNS;

		msSystem.msLEDs.setLED(_arp.new_pattern, 0, 100, 100, msGlobals.ggBrightness); 

		_arp.arpProgramChange(_arp.curr_midiview.midi_channel, _arp.new_pattern);
	}

	void decPattern() {
		msSystem.msLEDs.setLED(_arp.new_pattern, 0, 0, 0, msGlobals.ggBrightness); 

		_arp.new_pattern --;
		if (_arp.new_pattern < 0)
			_arp.new_pattern = 0;

		msSystem.msLEDs.setLED(_arp.new_pattern, 0, 100, 100, msGlobals.ggBrightness); 

		_arp.arpProgramChange(_arp.curr_midiview.midi_channel, _arp.new_pattern);

	}


	// main MIDI frame processor
	// 
	bool step()
	{

		handleButtons();

		// consume button events if necessary ..
		if (msSystem.msButtons.msBtnPwrHit) {
		}

		if (msSystem.msButtons.msBtnAHit) {
			decPattern();
		}
		if (msSystem.msButtons.msBtnBHit) {
			incPattern();
		}

		midi_frame++;

		// minimize latency introduced by the Arp frame
		unsigned long nowT = micros();

		if (!((nowT - _arp.last_arp_frame_time) < _arp.arp_beat_duration)) {
			_arp.arpFrame();
			_arp.last_arp_frame_time = nowT;
		} else {
			// msSystem.slogln("MIDI: nowT - last_arp_frame_time:" + String((nowT - _arp.last_arp_frame_time)));
		}

		msSystem.msLEDs.updateLEDs();

		return false;
	}


	// TODO: MIDI Sync
	void MIDISync()
	{
		if (sync_count < 24) {
			sync_count = sync_count + 1;
		} else {
			sync_count = 0;
		}
	}

	void MIDI_Start()
	{
		msSystem.slogln(modeName + " MIDI Start");
		_arp.arpStart();
	}

	void MIDI_Stop()
	{
		msSystem.slogln(modeName + " MIDI Stop");
		_arp.arpStop();
	}

	void MIDI_Program_Change(byte channel, byte program)
	{
		msSystem.slogln(modeName + " MIDI PC - channel: " + 
									String(channel) + 
									" program: " + 
									String(program));

		_arp.arpProgramChange(channel, program);
	}

	void MIDI_Control_Change(byte channel, byte cc1, byte cc2)
	{
		msSystem.slogln(modeName + " MIDI CC - channel: " + 
									String(channel) + 
									" cc1: " + 
									String(cc1) +
									" cc2: " + 
									String(cc2));

		msSystem.msLEDs.setLED(LED_CONTROL_CHANGE, 0, 100, 100);
		if (channel == 1) {
			_arp.arp_bpm = LOWEST_ARP_TEMPO + (cc1);
			_arp.arp_beat_duration = ARP_DURATION_FOR_BPM(_arp.arp_bpm);
		}
	}

	void MIDI_Note_On(byte channel, byte note, byte velocity)
	{
		msSystem.slogln(modeName + " MIDI Note On - channel: " + 
									String(channel) + 
									" note: " + 
									String(note) +
									" vel: " + 
									String(velocity));

		_arp.arpNoteOn(channel, note, velocity);
		msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 100);
	}

	void MIDI_Note_Off(byte status, byte note, byte velocity)
	{
		msSystem.slogln(modeName + " MIDI Note Off - status: " + 
									String(status) + 
									" note: " + 
									String(note) +
									" vel: " + 
									String(velocity));

		uint8_t note_msg[3];
		note_msg[0] = status;
		note_msg[1] = note;
		note_msg[2] = velocity;

#ifdef CONFIG_ENABLE_MIDI_SERIAL
		SERIAL_MIDI_Put(&note_msg[0], (uint16_t)3);
#endif

		msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 0);
	}

};
