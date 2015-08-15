/*
 * MIDIMode.cpp
 *
 * Created: Wed Jul  9 22:19:46 CEST 2014
 *  Author: ibisum  <ibisum@gmail.com
 *
 *  NOTE: camelBack for types and functions, snake_case for vars, SCREAMING_SNAKE_CASE for defs
 *
 */

// #include "../firmware.h"

#include "list.h"

// MIDI-specific definitions
#include "midi_defs.h"

#include "miby.cc"

// LAB/HAK ZONE:
#define SEQ_LEN 10
uint8_t midi_sequence[SEQ_LEN] =
	{ 0xF0, 0x00, 0x00, 0x50, 0x22, 0x03, 0x40, 0x09, 0x00, 0xF7 };

#define CSEQ_LEN (2 * 3)
uint8_t midi_complex[CSEQ_LEN] = {
	0x90, 0x40, 0x40,			//Start of E3 note, pitch = 64)
// 0x90, 0x43, 0x40, //Start of G3 note, pitch= 67)
// 0x80, 0x43, 0x00, //End of G3 note, pitch=67)
// 0x90, 0x45, 0x40, //Start of A3 note, pitch=69)
// 0x80, 0x45, 0x00, //End of A3 note, pitch=69)
	0x80, 0x40, 0x00,			//End of E3 note, pitch=64)
// 0x90, 0x3C, 0x40, //Start of C3 note, pitch = 60)
// 0x90, 0x47, 0x40, //Start of B3 note, pitch= 71)
// 0x80, 0x47, 0x00, //End of B3 note, pitch= 71)
// 0x90, 0x48, 0x40, //Start of C4 note, pitch= 72)
// 0x80, 0x48, 0x00, //End of C4 note, pitch= 72)
// 0x80, 0x3C, 0x40, //End of C3 note, pitch = 60)
};

// END OF LAB/HAK ZONE


// from TEPIS - time divisors
#define DEFAULT_DIVISOR 976
#define DEFAULT_TIME_BASE 1000000
// up to NUM_MIDI_SEQS can be recorded, per NUM_CHANNELS
#define NUM_MIDI_SEQS 8
#define NUM_CHANNELS 16

// current state of the MIDI processing system
static uint16_t midi_state = 0;

// what we are looking at - the current View
struct tMIDIView {
	uint8_t channel;			// MIDI loved MTV
	uint16_t time_base;			// time lords too
	void *v_arg;				// this ones for you
} ;

struct tMIDIView curr_midiview;

// a MIDI record
struct tMIDIPacket {
	uint8_t buffer[4];			// raw midi bytes
	uint16_t pkt_time;			// time of arrival
	uint8_t is_used;			// is currently in use
	uint8_t r_stat;				// read status
	uint8_t w_stat;				// write status
} ;

// head of MIDI_sequences
struct tMIDISequence {
	uint16_t in_time;			//
	struct list_head list;		// for larger MIDI seqs (sysex)
	struct tMIDIPacket midi_seqs[NUM_MIDI_SEQS];	// set of midi_seqs
} ;

// MIDI Sequences
struct tMIDISequence MIDI_sequences[NUM_CHANNELS];

// current packets per frame
struct tMIDIPacket *midi_in_pkt;
struct tMIDIPacket *midi_out_pkt;

// inbox/outbox for packets per frame
uint8_t midi_inbox;
uint8_t midi_outbox;

// uint8_t d_period=50;
uint8_t sync_count;				// sync counter
uint16_t midi_frame = 0;

// Send a MIDI message
static uint16_t MIDIPut(uint8_t * data, uint16_t cnt)
{
	// setPixel(15, 100, 100, 0);
	// Serial.print(data[ret], HEX);
	// blink(STAT2, d_period);
// setPixel(15, 0, 100, 0);
// delay(10);
	return (Serial1.write(data, cnt));
}

// Receive a MIDI message if its available
static uint16_t MIDIGet(uint8_t * data, uint16_t cnt)
{
	uint16_t ret;
	ret = 0;
	while ((cnt > 0) && (data[ret] = Serial1.read()) != -1) {
// Serial.println(data[ret], HEX);
		ret++;
		cnt--;
	}
	// blink(STAT1, d_period);
// setPixel(0, 100, 0, 0);
// delay(10);
	return ret;
}

void MIDIFrame()
{
// ANALYZE:
// PROCESS I/O:
	midi_in_pkt =
		&MIDI_sequences[curr_midiview.channel].midi_seqs[midi_inbox];

	midi_out_pkt =
		&MIDI_sequences[curr_midiview.channel].midi_seqs[midi_outbox];

	// pull midi_inbox
	if (Serial1.available() > 0) {
		setPixel(4, 100, 0, 0);
		setPixel(5, 0,0,0);
		if (midi_in_pkt->is_used == 0) {
			midi_in_pkt->r_stat = MIDIGet(&midi_in_pkt->buffer[0], 4);
			midi_in_pkt->pkt_time = micros();	///  time
			midi_in_pkt->is_used = 1;
			// inbox wrap
			midi_inbox++;
			if (midi_inbox >= NUM_MIDI_SEQS)
				midi_inbox = 0;
		}
	}

	// push midi_outbox
	if (midi_out_pkt->pkt_time <= (micros() - curr_midiview.time_base)) {
		if (midi_out_pkt->is_used == 1) {
			setPixel(4, 0,0,0);
			setPixel(5, 0, 0, 100);
			midi_out_pkt->w_stat = MIDIPut(&midi_out_pkt->buffer[0], 4);
			midi_out_pkt->is_used = 0;

			// if (midi_state==1)
			// {
			//  midi_out_pkt->pkt_time += (micros() + curr_midiview.time_base * 2);
			//  midi_out_pkt->is_used = 1;
			//        // blink(STAT2, d_period * 2);
			// }
			// else
			//  midi_out_pkt->is_used = 0;

			// outbox wrap
			midi_outbox++;
			if (midi_outbox >= NUM_MIDI_SEQS)
				midi_outbox = 0;
		}
	}


	midi_frame++;
	// if (midi_outbox == midi_inbox) midi_inbox=0;
}

// TODO: MIDI Sync
void MIDISync()
{
	if (sync_count < 24) {
		sync_count = sync_count + 1;
		// blink(STAT2, d_period);
	} else {
		sync_count = 0;
		//MIDIPut(0x80, 60, 0x00);
		//MIDIPut(0x90, 60, 0x40);
		// blink(STAT1);
	}
}


  int currentTimeStamp = 0;
  int lastTimeStamp = 0;
  int microsSinceLast = 0;

// !J! TODO : There should be a MagicShifter API for this
// TODO: private state
// state for button timing
  int buttonAPressedTime = 0;
  int buttonPowerPressedTime = 0;
  int buttonBPressedTime = 0;

// state for double click timing
  int timeToLastClickedButtonA = 0;
  int timeToLastClickedButtonPower = 0;
  int timeToLastClickedButtonB = 0;

  bool m_enableLongClicks = true;

  bool clickedButtonA = false;
  bool clickedButtonPower = false;
  bool clickedButtonB = false;

  bool longClickedButtonA = false;
  bool longClickedButtonPower = false;
  bool longClickedButtonB = false;
  
  bool doubleClickedButtonA = false;
  bool doubleClickedButtonPower = false;
  bool doubleClickedButtonB = false;

  void handleButtons()
  {
    // reset public btton state
    clickedButtonA = longClickedButtonA = false;
    if (!digitalRead(PIN_BUTTON_A))
    {
      if (buttonAPressedTime)
        buttonAPressedTime += microsSinceLast;
      else
        buttonAPressedTime = 1;
    }
    else
    {
      if (m_enableLongClicks && buttonAPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonA = true;
        log("longClickedButtonA", INFO);
      }
      else if (buttonAPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonA = true;
        log("clickedButtonA", INFO);
      }

      buttonAPressedTime = 0;
    }


    // reset public btton state
    clickedButtonB = longClickedButtonB = false;
    if (!digitalRead(PIN_BUTTON_B))
    {
      if (buttonBPressedTime)
        buttonBPressedTime += microsSinceLast;
      else
        buttonBPressedTime = 1;
    }
    else
    {
      if (m_enableLongClicks && buttonBPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonB = true;
        log("longClickedButtonB", INFO);

      }
      else if (buttonBPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonB = true;
        log("clickedButtonB", INFO);
      }

      buttonBPressedTime = 0;
    }


    // reset public btton state
    clickedButtonPower = longClickedButtonPower = false;
/*
    if (bFrame++ % 10 == 0)
    if (powerButtonPressed())
    {
      if (buttonPowerPressedTime)
        buttonPowerPressedTime += microsSinceLast;
      else
        buttonPowerPressedTime = 1;
    }
    else
    {
      if (buttonPowerPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonPower = true;
      }
      else if (buttonPowerPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonPower = true;
      }

      buttonPowerPressedTime = 0;
    }
    //*/

// internal button usage
    if (longClickedButtonA)
    {
      // powerDown();	// !J!
    }

    if (clickedButtonB)
    {
      GLOBAL_GS+=2;
      if (GLOBAL_GS > 31)
      {
        GLOBAL_GS = 31;
      }

      //shifterMode = (shifterMode+1)%MODES;
    }
    if (longClickedButtonB)
    {
      GLOBAL_GS-=6;
      if (GLOBAL_GS < 1)
      {
        GLOBAL_GS = 1;
      }

      //shifterMode = (shifterMode+1)%MODES;
    }
  }


void MIDIMode()
{
	fillPixels(0, 0, 0);
	setPixel(3, 0, 100, 0);

	curr_midiview.time_base = DEFAULT_DIVISOR;	//knob_1 * DEFAULT_DIVISOR;
	curr_midiview.channel = 0;
	midi_inbox = 0;
	midi_outbox = 0;

	while (1) {

		handleButtons();		// !J! TODO: use MS API for this?

// blink(50, 50, 3);
// MIDIPut(midi_complex, CSEQ_LEN);
		MIDIFrame();
		// fillPixels(0,0,0);
		// setPixel(15, centerBtnPressed ? 100 : 0, powerBtnPressed ? 100 : 0, 0);

#if 0
		// updateLedsWithBlank();		// !J! 
		if (!MagicShifter_Poll())
			break;
#endif

		// Debug:
		if (clickedButtonA) {
			fillPixels(100, 0, 0);
			// updateLedsWithBlank();
			delay(10);
			fillPixels(0, 0, 0);
			clickedButtonA = false;
		}
		if (clickedButtonB) {
			fillPixels(0, 0, 100);
			// updateLedsWithBlank();
			delay(10);
			fillPixels(0, 0, 0);
			clickedButtonB = false;
		}
	}
}
