
// MIDI features can be configured in or out of
// the project according to Serial needs, and can be
// enabled for WiFi.
// e.g. debugging
//
#ifdef CONFIG_ENABLE_MIDI

#ifdef CONFIG_MIDI_RTP_MIDI

// !J! 
#define APPLEMIDI_DEBUG 1
#include "AppleMidi.h"
// RTPMIDI is usable over WiFi
#endif

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

#warning "RTP MIDI Enabled."

// Debug RTP MIDI event handlers:
// -----------------------------------------------------------------------------
void OnRTPMIDI_Connected(uint32_t ssrc, char* name) {
	Serial.print("MIDI(rtp): Connected to session ");
	Serial.println(name);
}

void OnRTPMIDI_Disconnected(uint32_t ssrc) {
	Serial.println("MIDI(rtp): Disconnected from session");
	Serial.print("ssrc:");
	Serial.println(ssrc);
}

void OnRTPMIDI_ReceiveNoteOn(byte channel, byte note, byte velocity) {
	Serial.print("MIDI(rtp): channel:");
	Serial.print(String(channel));
	Serial.print(" noteOn:");
	Serial.print(String(note));
	Serial.print(" vel.:");
	Serial.println(String(velocity));
}

void OnRTPMIDI_ReceiveNoteOff(byte channel, byte note, byte velocity) {
	Serial.print("MIDI(rtp): channel:");
	Serial.print(String(channel));
	Serial.print(" noteOff:");
	Serial.print(String(note));
	Serial.print(" vel.:");
	Serial.println(String(velocity));
}

void OnRTPMIDI_ReceiveAfterTouchPoly(byte channel, byte note, byte velocity) {
	Serial.print("MIDI(rtp): channel:");
	Serial.print(String(channel));
	Serial.print(" atouch poly note:");
	Serial.print(String(note));
	Serial.print(" vel.:");
	Serial.println(String(velocity));
};

void OnRTPMIDI_ReceiveControlChange(byte channel, byte cc1, byte cc2) {
	Serial.print("MIDI(rtp): channel:");
	Serial.print(String(channel));
	Serial.print(" cc1:");
	Serial.print(String(cc1));
	Serial.print(" cc2:");
	Serial.println(String(cc2));
};

void OnRTPMIDI_ReceiveProgramChange(byte channel, byte program) {
	Serial.print("MIDI(rtp): channel:");
	Serial.print(String(channel));
	Serial.print(" change program:");
	Serial.println(String(program));
};

void OnRTPMIDI_ReceiveAfterTouchChannel(byte channel, byte atouch) {
	Serial.print("MIDI(rtp): channel:");
	Serial.print(String(channel));
	Serial.print(" atouch:");
	Serial.println(String(atouch));
};

void OnRTPMIDI_ReceivePitchBend(byte channel, int bend) {
	Serial.print("MIDI(rtp): channel:");
	Serial.print(String(channel));
	Serial.print(" bend:");
	Serial.println(String(bend));
};

void OnRTPMIDI_ReceiveSysEx(const byte *data, uint16_t size) { 
	Serial.print("MIDI(rtp): sysex size:");
	Serial.println(String(size));
};

void OnRTPMIDI_ReceiveTimeCodeQuarterFrame(byte data) {
	Serial.print(" TCQF data:");
	Serial.println(String(data));
};

void OnRTPMIDI_ReceiveSongPosition(unsigned short beats) {
	Serial.print(" SPP beats:");
	Serial.println(String(beats));
};

void OnRTPMIDI_ReceiveSongSelect(byte songnumber) {
	Serial.print("MIDI(rtp): SONG number:");
	Serial.println(String(songnumber));
};

void OnRTPMIDI_ReceiveTuneRequest() {
	Serial.println("MIDI(rtp): Tune Request");
};

void OnRTPMIDI_ReceiveClock() {
	Serial.println("MIDI(rtp): Clock");
};

void OnRTPMIDI_ReceiveStart() {
	Serial.println("MIDI(rtp): START");

};

void OnRTPMIDI_ReceiveContinue() {
	Serial.println("MIDI(rtp): CONTINUE");

};

void OnRTPMIDI_ReceiveStop() {
	Serial.println("MIDI(rtp): STOP");

};

void OnRTPMIDI_ReceiveActiveSensing() {
	Serial.println("MIDI(rtp): ACTIVE SENSE");

};

void OnRTPMIDI_ReceiveReset() {
	Serial.println("MIDI(rtp): RESET");

};

// void OnInvitation(void* sender, AppleMIDI_Invitation&) {};
// void OnEndSession(void* sender, AppleMIDI_EndSession&) {};
// void OnReceiverFeedback(void* sender, AppleMIDI_ReceiverFeedback&) {};

// void OnInvitationAccepted(void* sender, AppleMIDI_InvitationAccepted&) {};
// void OnControlInvitationAccepted(void* sender, AppleMIDI_InvitationAccepted&) {};
// void OnContentInvitationAccepted(void* sender, AppleMIDI_InvitationAccepted&) {};

// void OnSyncronization(void* sender, AppleMIDI_Syncronization&) {};
// void OnBitrateReceiveLimit(void* sender, AppleMIDI_BitrateReceiveLimit&) {};
// void OnControlInvitation(void* sender, AppleMIDI_Invitation&) {};
// void OnContentInvitation(void* sender, AppleMIDI_Invitation&) {};


void setupRTPDebugHandlers() {
	AppleMIDI.OnConnected(OnRTPMIDI_Connected);
	AppleMIDI.OnDisconnected(OnRTPMIDI_Disconnected);
	AppleMIDI.OnReceiveNoteOff(OnRTPMIDI_ReceiveNoteOff);
	AppleMIDI.OnReceiveNoteOn(OnRTPMIDI_ReceiveNoteOn);
	AppleMIDI.OnReceiveAfterTouchPoly(OnRTPMIDI_ReceiveAfterTouchPoly);
	AppleMIDI.OnReceiveControlChange(OnRTPMIDI_ReceiveControlChange);
	AppleMIDI.OnReceiveProgramChange(OnRTPMIDI_ReceiveProgramChange);
	AppleMIDI.OnReceiveAfterTouchChannel(OnRTPMIDI_ReceiveAfterTouchChannel);
	AppleMIDI.OnReceivePitchBend(OnRTPMIDI_ReceivePitchBend);
	AppleMIDI.OnReceiveSysEx(OnRTPMIDI_ReceiveSysEx);
	AppleMIDI.OnReceiveTimeCodeQuarterFrame(OnRTPMIDI_ReceiveTimeCodeQuarterFrame);
	AppleMIDI.OnReceiveSongPosition(OnRTPMIDI_ReceiveSongPosition);
	AppleMIDI.OnReceiveSongSelect(OnRTPMIDI_ReceiveSongSelect);
	AppleMIDI.OnReceiveTuneRequest(OnRTPMIDI_ReceiveTuneRequest);
	AppleMIDI.OnReceiveClock(OnRTPMIDI_ReceiveClock);
	AppleMIDI.OnReceiveStart(OnRTPMIDI_ReceiveStart);
	AppleMIDI.OnReceiveContinue(OnRTPMIDI_ReceiveContinue);
	AppleMIDI.OnReceiveStop(OnRTPMIDI_ReceiveStop);
	AppleMIDI.OnReceiveActiveSensing(OnRTPMIDI_ReceiveActiveSensing);
	AppleMIDI.OnReceiveReset(OnRTPMIDI_ReceiveReset);
}




#endif // CONFIG_MIDI_RTP_MIDI
