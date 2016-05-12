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
#include "MIDI/midi_defs.h"
#include "MIDI/envelope.h"

// RTPMIDI is usable over WiFi
#ifdef CONFIG_MIDI_RTP
//Library code for AppleMIDI protocol
//pio lib install 222
#include "AppleMidi.h"

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h
bool isRTPConnected = false;

// RTP MIDI event handlers:
// -----------------------------------------------------------------------------
void OnRTPMIDI_Connect(uint32_t ssrc, char* name) {
  isRTPConnected = true;
  Serial.print("Connected to session ");
  Serial.println(name);
}

void OnRTPMIDI_Disconnect(uint32_t ssrc) {
  isRTPConnected = false;
  Serial.println("Disconnected");
}

void OnRTPMIDI_NoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOn from channel:");
  Serial.print(String(channel));
  Serial.print(" note:");
  Serial.print(String(note));
  Serial.print(" velocity:");
  Serial.println(String(velocity));
}

void OnRTPMIDI_NoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOff from channel:");
  Serial.print(String(channel));
  Serial.print(" note:");
  Serial.print(String(note));
  Serial.print(" velocity:");
  Serial.println(String(velocity));
}
#endif // CONFIG_MIDI_RTP


#endif // CONFIG_MIDI_ONBOARD
