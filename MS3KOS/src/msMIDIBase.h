
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
	msSystem.slog("MIDI(rtp): Connected to session ");
	msSystem.slogln(name);
}

void OnRTPMIDI_Disconnected(uint32_t ssrc) {
	msSystem.slogln("MIDI(rtp): Disconnected from session");
}

void OnRTPMIDI_ReceiveNoteOn(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" noteOn:");
	msSystem.slog(String(note));
	msSystem.slog(" vel.:");
	msSystem.slogln(String(velocity));
}

void OnRTPMIDI_ReceiveNoteOff(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" noteOff:");
	msSystem.slog(String(note));
	msSystem.slog(" vel.:");
	msSystem.slogln(String(velocity));
}

void OnRTPMIDI_ReceiveAfterTouchPoly(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" atouch poly note:");
	msSystem.slog(String(note));
	msSystem.slog(" vel.:");
	msSystem.slogln(String(velocity));
};

void OnRTPMIDI_ReceiveControlChange(byte channel, byte cc1, byte cc2) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" cc1:");
	msSystem.slog(String(cc1));
	msSystem.slog(" cc2:");
	msSystem.slogln(String(cc2));
};

void OnRTPMIDI_ReceiveProgramChange(byte channel, byte program) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" change program:");
	msSystem.slogln(String(program));
};

void OnRTPMIDI_ReceiveAfterTouchChannel(byte channel, byte atouch) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" atouch:");
	msSystem.slogln(String(atouch));
};

void OnRTPMIDI_ReceivePitchBend(byte channel, int bend) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" bend:");
	msSystem.slogln(String(bend));
};

void OnRTPMIDI_ReceiveSysEx(const byte *data, uint16_t size) { 
	msSystem.slog("MIDI(rtp): sysex size:");
	msSystem.slogln(String(size));
};

void OnRTPMIDI_ReceiveTimeCodeQuarterFrame(byte data) {
	msSystem.slog(" TCQF data:");
	msSystem.slogln(String(data));
};

void OnRTPMIDI_ReceiveSongPosition(unsigned short beats) {
	msSystem.slog(" SPP beats:");
	msSystem.slogln(String(beats));
};

void OnRTPMIDI_ReceiveSongSelect(byte songnumber) {
	msSystem.slog("MIDI(rtp): SONG number:");
	msSystem.slogln(String(songnumber));
};

void OnRTPMIDI_ReceiveTuneRequest() {
	msSystem.slogln("MIDI(rtp): Tune Request");
};

void OnRTPMIDI_ReceiveClock() {
	msSystem.slogln("MIDI(rtp): Clock");
};

void OnRTPMIDI_ReceiveStart() {
	msSystem.slogln("MIDI(rtp): START");

};

void OnRTPMIDI_ReceiveContinue() {
	msSystem.slogln("MIDI(rtp): CONTINUE");

};

void OnRTPMIDI_ReceiveStop() {
	msSystem.slogln("MIDI(rtp): STOP");

};

void OnRTPMIDI_ReceiveActiveSensing() {
	msSystem.slogln("MIDI(rtp): ACTIVE SENSE");

};

void OnRTPMIDI_ReceiveReset() {
	msSystem.slogln("MIDI(rtp): RESET");

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
