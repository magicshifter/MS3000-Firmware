/*
 * MIDIMode.cpp
 * Created: Wed Jul  9 22:19:46 CEST 2014
 * Author: Jay Vaughan <ibisum@gmail.com
 *
 * A MIDI Processing mode for the MagicShifter
 *
 * Utilizes miby for MIDI Byte Parsing, from Neil Johnson  <http://www.njohnson.co.uk>
 * Arpeggiator originally based on inspiration from RobG @ 43oh
 *
 * NOTE: camelBack for types and functions, snake_case for vars, SCREAMING_SNAKE_CASE for
 * constant definitions
 *
 */

//#include "../firmware.h"

//#include "list.h"				// light list library (future-use)

// Neil Johnsons' tight MIDI Byte parser:
// #include "miby.h"
#include "miby.cpp"	// included like this because Arduino

// NOTE: the configuration for the miby callbacks is done in MidiModeMibyConfig.h
// .. which refers to functions declared in this module.  -D is used at build
// time for configuration - see Makefile

// Standard MIDI 1.0 definitions and other constants
#include "midi_defs.h"

// ADSR Envelope calculator
#include "envelope.h"

// Status Indicator LED's - for the Arpeggiator, etc.
#define LED_ARP_COUNTER (13)
#define LED_MEASURE_COUNTER (14)
#define LED_BEAT_COUNTER (15)
#define LED_CONTROL_CHANGE (10)
#define LED_NOTE_EVENT (8)

// Current View per MIDI input
typedef struct {
	uint8_t midi_channel;		// MIDI channel of View
	uint16_t time_base;			// Base Time for sequencer-Put
	void *v_arg;				// user data
} MIDIViewT ;

MIDIViewT curr_midiview;

// MIDI housekeeping
uint8_t midi_mode = 0;			// Mode of this module (future-use)
uint8_t sync_count;				// sync counter
uint16_t midi_frame = 0;		// current MIDI Processing frame

static uint16_t MIDI_Put(uint8_t * data, uint16_t cnt);
static uint16_t MIDI_Get(uint8_t * data, uint16_t cnt);

// MIDI event handlers
void MIDI_Start(miby_this_t a_miby);
void MIDI_Stop(miby_this_t a_miby);
void MIDI_Program_Change(miby_this_t a_miby);
void MIDI_Control_Change(miby_this_t a_miby);
void MIDI_Note_On(miby_this_t a_miby);
void MIDI_Note_Off(miby_this_t a_miby);

// main entry point:
void MIDIMode();

// Frame Processor:
void MIDIFrame(miby_t *miby);
void MIDISync();

// Arpeggiator constants -----------------------------------------------------------------
#define BUFFER_SIZE (64)
#define BUFFER_MASK (0x3F)
#define ARP_DURATION_FOR_BPM(v) (600000/v/2)
#define LOWEST_ARP_TEMPO (20)

// Arpeggiator function declarations:
void arpFrame(); // Arpeggiator processing is done per-frame
void arpPlayNote(uint8_t noteNumber, uint8_t on_off);
void arpSoundOff();
void arpNoteOn(uint8_t noteChannel, uint8_t noteNumber, uint8_t velocity);
void arpPushMIDI(uint8_t data);
void arpSendMIDI();
void arpProgramChange(uint8_t pcChannel, uint8_t number);
uint8_t getNextNaturalNote(uint8_t note);
uint8_t getNextNaturalNoteAdd(uint8_t naturalNote, uint8_t offset);

// Arpeggiator structures:

// ring buffer for queued events
uint8_t fifo[BUFFER_SIZE];
uint8_t fifo_in_index = 0;
uint8_t fifo_out_index = 0;

// An arpeggiator event
typedef struct {
	uint8_t beat;
	uint8_t note;
	uint8_t on_off;
} ArpEventT;

// A list of arpeggiator events (fixed size)
ArpEventT arp_events[8];

// Arpeggiator configuration state
uint8_t  arp_play_state = 0;
uint8_t arp_bpm = LOWEST_ARP_TEMPO;
uint32_t arp_beat_duration = 0;
uint8_t arp_frame = 0; // internal beat, which is 1/16th of the actual beat (see arp_bpm)
uint8_t current_pattern = 1; // this will trigger loading for the first time.
uint8_t new_pattern = 6;
uint8_t current_note = 0;
uint8_t new_note = 0;
uint8_t current_velocity = 64;
uint8_t new_velocity = 64;

const uint8_t note_offset[12] = { 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0};

const uint8_t note_LUT[7][7] = {
	{ 0, 1, 2, 2, 3, 4, 5 },
	{ 0, 1, 1, 2, 3, 4, 4 },
	{ 0, 0, 1, 2, 3, 3, 4 },
	{ 0, 1, 2, 3, 3, 4, 5 },
	{ 0, 1, 2, 2, 3, 4, 4 },
	{ 0, 1, 1, 2, 3, 3, 4 },
	{ 0, 0, 1, 2, 2, 3, 4 }
};

const uint8_t note_to_natural[12] = { 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6 };

// A pattern of arpeggiator units
typedef struct {
	uint8_t note_offset;
	uint8_t beat;
	uint8_t note_duration;
} ArpPatternT;

const ArpPatternT arp_patterns[][4] = { // arp_patterns 4/4
		{ { 0, BEAT1, EIGHTH },
		  { 2, BEAT2, EIGHTH },
		  { 0, BEAT3, EIGHTH },
		  { 2, BEAT4, EIGHTH } }, //
		{ { 0, BEAT1, EIGHTH },
		  { 0, BEAT2, EIGHTH },
  		  { 2, BEAT3, EIGHTH },
  		  { 2, BEAT4, EIGHTH } }, //
		{ { 0, BEAT1, EIGHTH },
		  { 0, BEAT2, EIGHTH },
		  { 2, BEAT3, EIGHTH },
		  { 4, BEAT4, EIGHTH } }, //
		{ { 0, BEAT1, EIGHTH },
		  { 2, BEAT2, EIGHTH },
		  { 4, BEAT3, EIGHTH },
		  { 6, BEAT4, EIGHTH } }, //
		{ { 0, BEAT1, EIGHTH },
		  { 12, BEAT2, EIGHTH },
		  { 0, BEAT3, EIGHTH },
		  { 12, BEAT4, EIGHTH } }, //
		{ { 0, BEAT1, EIGHTH },
		  { 0, BEAT2, EIGHTH },
		  { 12, BEAT3, EIGHTH },
		  { 12, BEAT4, EIGHTH } }, //
		{ { 0, BEAT1, EIGHTH },
		  { 0, BEAT2, EIGHTH },
		  { 4, BEAT3, EIGHTH },
		  { 12, BEAT4, EIGHTH } }, //
		{ { 0, BEAT1, EIGHTH },
		  { 2, BEAT1 + 8, EIGHTH },
		  { 4, BEAT2, QUARTER },
		  { 6, BEAT4, EIGHTH } }, //
		};

// Envelopes - used for the LED's
adsr_envelope anEnvelope;



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


// ------------------------------------------------------------------------------ MIDI I/O
// Send a MIDI message
// returns the # of bytes sent
static uint16_t MIDI_Put(uint8_t * data, uint16_t length)
{
	return (Serial1.write(data, length));
}
// Receive a MIDI message if its available
// returns the # of bytes received in the message (count should be length)
static uint16_t MIDI_Get(uint8_t * data, uint16_t length)
{
	int8_t count;
	count = 0;
	while ((length > 0) && (data[count] = Serial1.read()) != -1) {
		count++;
		length--;
	}
	return count;
}

// ------------------------------------------------------------------- MIDI Event Handlers
// MIDI Event handlers, configured for use by miby
void MIDI_Start(miby_this_t a_miby)
{
	if (arp_play_state == 0) {
		arp_play_state = 1;
		arp_frame = 0;
	}
}

void MIDI_Stop(miby_this_t a_miby)
{
	if (arp_play_state == 1) {
		arp_play_state = 0;
		// arpSoundOff();
	}
}

void MIDI_Program_Change(miby_this_t a_miby)
{
	arpProgramChange(curr_midiview.midi_channel,  MIBY_ARG0(a_miby));
}

void MIDI_Control_Change(miby_this_t a_miby)
{
	msSystem.msLEDs.setPixels(LED_CONTROL_CHANGE, 0, 100, 100);
	// TODO: Channel processing
	// if (curr_midiview.midi_channel == MIBY_CHAN(a_miby)) {
		// any controller can be used, cc#1 for now (modwheel)
		if (MIBY_ARG0(a_miby) == 1) {
			arp_bpm = LOWEST_ARP_TEMPO + (MIBY_ARG1(a_miby));
			arp_beat_duration = ARP_DURATION_FOR_BPM(arp_bpm);
		}
	// }
}

void MIDI_Note_On( miby_this_t a_miby)
{
	arpNoteOn(curr_midiview.midi_channel,  MIBY_ARG0(a_miby),  MIBY_ARG1(a_miby));
	msSystem.msLEDs.setPixels(LED_NOTE_EVENT, 0, 0, 100);
}

void MIDI_Note_Off( miby_this_t a_miby)
{
	uint8_t note_msg[3];
	note_msg[0] = MIBY_STATUSBYTE(a_miby);
	note_msg[1] = MIBY_ARG0(a_miby);
	note_msg[2] = MIBY_ARG1(a_miby);

	MIDI_Put(note_msg, 3);

	msSystem.msLEDs.setPixels(LED_NOTE_EVENT, 0, 0, 0);
}

void LEDFrame()
{
	// Blink Arpeggiator LED's
	if (arp_frame == 0) {
		msSystem.msLEDs.setPixels(LED_BEAT_COUNTER, 100, 0, 0);
		msSystem.msLEDs.setPixels(LED_MEASURE_COUNTER, 0, 100, 0);
	}
	else if ((arp_frame & 0x0F) == 0) {
		msSystem.msLEDs.setPixels(LED_BEAT_COUNTER, 0, 100, 0);
	}
	else { // LEDs off
		msSystem.msLEDs.setPixels(LED_ARP_COUNTER, 100, 0, 0);
		msSystem.msLEDs.setPixels(LED_BEAT_COUNTER, 0, 0, 0);
		msSystem.msLEDs.setPixels(LED_MEASURE_COUNTER, 0, 0, 0);
	}

#if 0
		// updateLedsWithBlank();		// !J! 
		if (!MagicShifter_Poll())
			break;
#endif

	// Debug:
	if (msBtnAHit) {
		msSystem.msLEDs.fillPixels(100, 0, 0);
		// updateLedsWithBlank();
		delay(10);
		msSystem.msLEDs.fillPixels(0, 0, 0);
		msBtnAHit = false;
	}
	if (msBtnBHit) {
		msSystem.msLEDs.fillPixels(0, 0, 100);
		// updateLedsWithBlank();
		delay(10);
		msSystem.msLEDs.fillPixels(0, 0, 0);
		msBtnBHit = false;
	}

}


void envDump()
{
	int c;

	msSystem.logln("Current Dump:");

	msSystem.logln(" T: ");
	msSystem.logln(anEnvelope.timer, DEC);

	msSystem.logln(" L: ");
	msSystem.logln(anEnvelope.level, DEC);

	msSystem.logln(" C: ");
	msSystem.logln((unsigned int)anEnvelope.current, HEX);

	msSystem.logln(" IDLE: ");
	msSystem.logln(anEnvelope.is_idle, DEC);

	msSystem.logln("Envelope Dump:");

	for (c=0;c<=ENV_RELEASE;c++){
		msSystem.logln("Stage:");
		msSystem.logln((unsigned int)&anEnvelope.stages[c], HEX);
		msSystem.logln(" ");
		msSystem.logln(c, DEC);
		msSystem.logln(" L:");
		msSystem.logln(anEnvelope.stages[c].level, DEC);
		msSystem.logln(" / D:");
		msSystem.logln(anEnvelope.stages[c].duration, DEC);
		msSystem.logln(" @ C:");
		msSystem.logln(anEnvelope.stages[c].coeff, DEC);

	}
}

// Initialize the envelopes
void envInit()
{
	adsr_envelope_init(&anEnvelope);
	anEnvelope.stages[ENV_START].level = 1;
	anEnvelope.stages[ENV_START].duration = 1;
	anEnvelope.stages[ENV_START].coeff = 1;

	anEnvelope.stages[ENV_ATTACK].level = 10;
	anEnvelope.stages[ENV_ATTACK].duration = 2;
	anEnvelope.stages[ENV_ATTACK].coeff = -1;

	anEnvelope.stages[ENV_DECAY].level = 20;
	anEnvelope.stages[ENV_DECAY].duration = 3;
	anEnvelope.stages[ENV_DECAY].coeff = -1;

	anEnvelope.stages[ENV_SUSTAIN].level = 30;
	anEnvelope.stages[ENV_SUSTAIN].duration = 4;
	anEnvelope.stages[ENV_SUSTAIN].coeff = -1;

	anEnvelope.stages[ENV_RELEASE].level = 40;
	anEnvelope.stages[ENV_RELEASE].duration = 5;
	anEnvelope.stages[ENV_RELEASE].coeff = -1;

	anEnvelope.current = &anEnvelope.stages[ENV_START]; // active

}

// calculate the envelopes
void envFrame()
{
	static int8_t dbg;
	adsr_envelope_tick(&anEnvelope);



	envDump();


	dbg++;
	if (dbg>=10)
		exit;
}



  int currentTimeStamp = 0;
  int lastTimeStamp = 0;
  int microsSinceLast = 0;

  void handleButtons()
  {
    // reset public btton state
    msBtnAHit = msBtnALongHit = false;
    if (!digitalRead(PIN_BUTTON_A))
    {
      if (msBtnAPressTime)
        msBtnAPressTime += microsSinceLast;
      else
        msBtnAPressTime = 1;
    }
    else
    {
      if (msLongClickOK && msBtnAPressTime >= MIN_TIME_LONG_CLICK)
      {
        msBtnALongHit = true;
        msSystem.logln("msBtnALongHit");
      }
      else if (msBtnAPressTime >= MIN_TIME_CLICK)
      {
        msBtnAHit = true;
        msSystem.logln("msBtnAHit");
      }

      msBtnAPressTime = 0;
    }


    // reset public btton state
    msBtnBHit = msBtnBLongHit = false;
    if (!digitalRead(PIN_BUTTON_B))
    {
      if (msBtnBPressTime)
        msBtnBPressTime += microsSinceLast;
      else
        msBtnBPressTime = 1;
    }
    else
    {
      if (msLongClickOK && msBtnBPressTime >= MIN_TIME_LONG_CLICK)
      {
        msBtnBLongHit = true;
        msSystem.logln("msBtnBLongHit");

      }
      else if (msBtnBPressTime >= MIN_TIME_CLICK)
      {
        msBtnBHit = true;
        msSystem.logln("msBtnBHit");
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

// internal button usage
    if (msBtnALongHit)
    {
      // powerDown();	// !J!
    }

    if (msBtnBHit)
    {
      msGlobals.ggFactoryIntensity+=2;
      if (msGlobals.ggFactoryIntensity > 31)
      {
        msGlobals.ggFactoryIntensity = 31;
      }

      msGlobals.ggCurrentMode = (msGlobals.ggCurrentMode+1)%NUM_MS_MODES;
    }
    if (msBtnBLongHit)
    {
      msGlobals.ggFactoryIntensity-=6;
      if (msGlobals.ggFactoryIntensity < 1)
      {
        msGlobals.ggFactoryIntensity = 1;
      }

      msGlobals.ggCurrentMode = (msGlobals.ggCurrentMode+1)%NUM_MS_MODES;
    }
  }


// ---------------------------------------------------------------------- Main Entry Point
void MIDIMode()
{
	// Debug - set an LED so we know we made it ..
	msSystem.msLEDs.fillPixels(0, 0, 0);
	msSystem.msLEDs.setPixels(0, 0, 100, 0);

	// Initial view
	curr_midiview.midi_channel = 0;

	// The MIDI byte parser, provided by the miby module ..
	miby_t 	miby;
	miby_init(&miby, NULL);

	// prime the Arp
	arp_beat_duration = ARP_DURATION_FOR_BPM(arp_bpm);
	//
	uint32_t arp_frame_time = 0;
	arp_frame_time = micros();
	arpFrame();

	// prime the envelopes
	envInit();

	arpSoundOff();

	while (1) {
		MIDIFrame(&miby);

  		// minimize latency introduced by the Arp frame
		if (!((micros() - arp_frame_time) < arp_beat_duration))
			arpFrame();
		arp_frame_time = micros();

		envFrame();



#if 0
		if (!MagicShifter_Poll())
			break;
#endif

	}
}

// ------------------------------------------------------------- Main MIDI Frame Processor
void MIDIFrame(miby_t *miby)
{
	uint8_t	midi_inb;
// ANALYZE:
// PROCESS I/O:
	// pull midi_inbox
	if (Serial1.available()) {
		MIDI_Get(&midi_inb, 1);
		miby_parse(miby, midi_inb);
	}
	else  // Soft-thru, etc.
	{
		// MIDI_Put(&midi_buf[0], 4);
	}
	midi_frame++;

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


// --------------------------------------------------------------------------- ARPEGGIATOR
// Process a frame of the Arpeggiator
void arpFrame() {

	uint8_t event_idx = 0;

	// end of arp measure, go to the beginning
	if (arp_frame == 64) {
		arp_frame = 0;

		// finish current pattern before stopping
		if (arp_play_state == 0)
			return;
	}

	// handle changes in arp inputs - at the beginning of the measure...
	if (arp_frame == 0 && (new_note != current_note || new_pattern != current_pattern
					       || new_velocity != current_velocity)) {
		// ...update arp_events if anything has changed

		// 8 arp_events, 4 pairs of notes, one on and one off
		while (event_idx < 4) {
			// if you don't use getNextNaturalNote in arpNoteOn, use getNextNaturalNote
			// here instead
			uint8_t note = getNextNaturalNoteAdd(new_note,
								arp_patterns[new_pattern][event_idx].note_offset);
			// on
			arp_events[event_idx].beat = arp_patterns[new_pattern][event_idx].beat;
			arp_events[event_idx].note = note;
			arp_events[event_idx].on_off = 1;

			// off
			// assuming that the beat and the duration is correctly calculated, sum
			// must be < 64
			arp_events[event_idx + 4].beat = arp_patterns[new_pattern][event_idx].beat
					+ arp_patterns[new_pattern][event_idx].note_duration;

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
		if (arp_events[event_idx].beat == arp_frame) {
			arpPlayNote(arp_events[event_idx].note, arp_events[event_idx].on_off);
		}
		event_idx++;
	}

	LEDFrame();

	// gather time data for next arp Frame
	arp_frame++;
}

void arpPlayNote(uint8_t noteNumber, uint8_t on_off) {
	arpPushMIDI((on_off ? MIDI_NOTE_ON : MIDI_NOTE_OFF) + curr_midiview.midi_channel);
	arpPushMIDI(noteNumber);
	arpPushMIDI(current_velocity);

	arpSendMIDI();
}

void arpSoundOff() {
	arpPushMIDI(MIDI_CONTROLLER + curr_midiview.midi_channel);
	arpPushMIDI(MIDI_ALL_NOTES_OFF);
	arpPushMIDI(0);

	arpSendMIDI();
}

void arpNoteOn(uint8_t noteChannel, uint8_t noteNumber, uint8_t velocity) {
	//new_note = noteNumber;
	new_note = getNextNaturalNote(noteNumber);
	new_velocity = velocity;
	if (arp_play_state == 0) { // start if stopped
		arp_frame = 0;
		arp_play_state = 1;
	}
}

// push a MIDI event into the arpeggiator queue for later processing
void arpPushMIDI(uint8_t data) {
	fifo[++fifo_in_index & BUFFER_MASK] = data;
}

// arpSendMIDI should be called after queueing up all messages and associated data for
// those messages
void arpSendMIDI() {
	while (fifo_in_index != fifo_out_index) { // send whatever is in the buffer
		MIDI_Put(&fifo[++fifo_out_index & BUFFER_MASK], 1);
	}
}

// reeive a Program change to change Arp Pattern
void arpProgramChange(uint8_t pcChannel, uint8_t number) {
	if (curr_midiview.midi_channel == pcChannel) {
		// only 8 preset arp_patterns right now so each program number is masked
		new_pattern = number & 0x07;
	}
}

// If the note is not a natural note, return next natural note
uint8_t getNextNaturalNote(uint8_t note) {
	uint8_t offset = note;
	while (offset > 11) {
		offset -= 12;
	}
	return note + note_offset[offset];
}

// Calculate net natural note + offset
uint8_t getNextNaturalNoteAdd(uint8_t naturalNote, uint8_t offset) {
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
