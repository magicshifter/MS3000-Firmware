//
// MS3000 MIDI functionality
// Author: seclorum (Jay Vaughan)
// Copyright 2016 Jay Vaughan
// License: AGPL
// Intended for inclusion in the MagicShifter3000 Firmware ("MS3KOS") project.
//
// The MagicShifter3000 is capable of two kinds of MIDI I/O:
//
// a) directly from the onboard UART hardware
// b) over WiFi, using RTP-MIDI (Also known as 'AppleMIDI')
//
// Refs:  https://en.wikipedia.org/wiki/MIDI
//        https://en.wikipedia.org/wiki/RTP-MIDI
//
// Note about Debugging MIDI on the MS3000:
//
// MIDI features can be configured in or out of the project according to 
// developer needs - you can use RTP- (but not UART-) MIDI to develop, if you 
// want to use Serial for debug messages (otherwise it'll be set to MIDI 
// specs and MIDI raw mode), or you can turn on UART- and RTP- MIDI features
// and still get debug messages with sysLog.
// note that when UART-based MIDI is desired, its a good idea to use the msSysLog
// logging feature of the MagicShifter3000, so you can still continue to get 
// debug messages to a properly configured syslog daemon somewhere on your local
// network.  (Also don't forget to tell the MS3000 to auto-connect on startup..)
// This way you can debug/develop with or without external MIDI hardware.
// 
// What is MIDI good for?
//
// MIDI is good for many things.  You can use it as a fast, real-time control
// protocol over WiFi, obviating the need for complex cyclomatic text-parsing 
// and other such REST'y things.  MIDI inbound messages are typically 3 bytes
// long, and can nevertheless pack a lot of dense information.  SYSEX can be
// used as a binary encoding method.  NRPN's are available for deeper values.
// 
// The MS3000 has enough processing power for sequencing needs, also.
//
// So, I include an Arpeggiator, which functions over either RTP- or UART- MIDI,
// as a basic module to demonstrate the capabilities.
//
// However, we could also use MIDI as the basis of the single-LED-strip UI
// mechanism - animate colors/LED's using MIDI control messages, and then when
// necessary (for a UI event, for instance), play the notes back in order to
// reproduce the animation.  This can then be used as the basis of encoding
// UI's, while not also calculating LED values - rather playing back a stream, 
// or even rendering arpeggiation with custom render methods, triggered by
// a smaller, compact MIDI stream...
// 
// For now, we include a basic Arpeggiator, and the mechanism to use either
// RTP- or UART- methods of i/o.

// see msConfig.h
#ifdef CONFIG_MIDI_ONBOARD

#include "miby.h"

#include "Modes/MIDI/midi_defs.h"
#include "Modes/MIDI/envelope.h"

//
// RAW MIDI I/O handlers, available for miby
//

// -- MIDI Event Handlers
// MIDI Event handlers, configured for use by miby
void UART_MIDI_Start(miby_this_t a_miby)
{
  if (msMIDIShifter._arp.arp_play_state == 0) {
    msMIDIShifter._arp.arp_play_state = 1;
    msMIDIShifter._arp.arp_frame = 0;
  }
}

void UART_MIDI_Stop(miby_this_t a_miby)
{
  if (msMIDIShifter._arp.arp_play_state == 1) {
    msMIDIShifter._arp.arp_play_state = 0;
    // arpSoundOff();
  }
}

void UART_MIDI_Program_Change(miby_this_t a_miby)
{
  msMIDIShifter._arp.arpProgramChange(curr_midiview.midi_channel, MIBY_ARG0(a_miby));
}

void UART_MIDI_Control_Change(miby_this_t a_miby)
{
  msSystem.msLEDs.setLED(LED_CONTROL_CHANGE, 0, 100, 100);
  // TODO: Channel processing
  // if (curr_midiview.midi_channel == MIBY_CHAN(a_miby)) {
  // any controller can be used, cc#1 for now (modwheel)
  if (MIBY_ARG0(a_miby) == 1) {
    msMIDIShifter._arp.arp_bpm = LOWEST_ARP_TEMPO + (MIBY_ARG1(a_miby));
    msMIDIShifter._arp.arp_beat_duration = ARP_DURATION_FOR_BPM(msMIDIShifter._arp.arp_bpm);
  }
  // }
}

void UART_MIDI_Note_On(miby_this_t a_miby)
{
  msMIDIShifter._arp.arpNoteOn(curr_midiview.midi_channel, MIBY_ARG0(a_miby),
    MIBY_ARG1(a_miby));
  msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 100);
}

void UART_MIDI_Note_Off(miby_this_t a_miby)
{
  uint8_t note_msg[3];
  note_msg[0] = MIBY_STATUSBYTE(a_miby);
  note_msg[1] = MIBY_ARG0(a_miby);
  note_msg[2] = MIBY_ARG1(a_miby);

  UART_MIDI_Put(note_msg, 3);

  msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 0);
}



// RTPMIDI is usable over WiFi
#ifdef CONFIG_MIDI_RTP
//Library code for AppleMIDI protocol
//pio lib install 222
#include "AppleMidi.h"

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

// bool isRTPConnected;

static void RTP_MIDI_Connect(uint32_t ssrc, char* name) {
  // isRTPConnected = true;
  msSystem.slogln("Connected to session...");
  msSystem.slog("Connected to session ");
  msSystem.slogln(name);
}

void RTP_MIDI_Disconnect(uint32_t ssrc) {
  // isRTPConnected = false;
  msSystem.slogln("Disconnected");
}

void RTP_MIDI_Note_On(byte channel, byte note, byte velocity) {
  msSystem.slog("Incoming NoteOn from channel:");
  msSystem.slog(String(channel));
  msSystem.slog(" note:");
  msSystem.slog(String(note));
  msSystem.slog(" velocity:");
  msSystem.slogln(String(velocity));

  msMIDIShifter._arp.arpNoteOn(curr_midiview.midi_channel, note, velocity);
  msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 100);

}

void RTP_MIDI_Note_Off(byte channel, byte note, byte velocity) {
  msSystem.slog("Incoming NoteOff from channel:");
  msSystem.slog(String(channel));
  msSystem.slog(" note:");
  msSystem.slog(String(note));
  msSystem.slog(" velocity:");
  msSystem.slogln(String(velocity));

  uint8_t note_msg[3];
  note_msg[0] = channel;
  note_msg[1] = note;
  note_msg[2] = velocity;
  UART_MIDI_Put(note_msg, 3);

  msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 0);

}
#endif


#endif // CONFIG_MIDI_ONBOARD
