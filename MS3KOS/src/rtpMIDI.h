

#ifdef CONFIG_MIDI_RTP_ENABLE

#include <WiFiUdp.h>

// !J! 
#define APPLEMIDI_DEBUG 1
#include "msSystem.h"
#include "AppleMidi.h"

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

#warning "RTP MIDI Enabled."

// Debug RTP MIDI event handlers:
// -----------------------------------------------------------------------------
void _RTP_MIDI_Connected(uint32_t ssrc, char* name) {
	msSystem.slog("MIDI(rtp): Connected to session ");
	msSystem.slogln(name);
}

void _RTP_MIDI_Disconnected(uint32_t ssrc) {
	msSystem.slogln("MIDI(rtp): Disconnected from session");
}

void _RTP_MIDI_Note_On(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" noteOn:");
	msSystem.slog(String(note));
	msSystem.slog(" vel.:");
	msSystem.slogln(String(velocity));
}

void _RTP_MIDI_Note_Off(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" noteOff:");
	msSystem.slog(String(note));
	msSystem.slog(" vel.:");
	msSystem.slogln(String(velocity));
}

void _RTP_MIDI_ReceiveAfterTouchPoly(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" atouch poly note:");
	msSystem.slog(String(note));
	msSystem.slog(" vel.:");
	msSystem.slogln(String(velocity));
};

void _RTP_MIDI_ReceiveControlChange(byte channel, byte cc1, byte cc2) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" cc1:");
	msSystem.slog(String(cc1));
	msSystem.slog(" cc2:");
	msSystem.slogln(String(cc2));
};

void _RTP_MIDI_ReceiveProgramChange(byte channel, byte program) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" change program:");
	msSystem.slogln(String(program));
};

void _RTP_MIDI_ReceiveAfterTouchChannel(byte channel, byte atouch) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" atouch:");
	msSystem.slogln(String(atouch));
};

void _RTP_MIDI_ReceivePitchBend(byte channel, int bend) {
	msSystem.slog("MIDI(rtp): channel:");
	msSystem.slog(String(channel));
	msSystem.slog(" bend:");
	msSystem.slogln(String(bend));
};

void _RTP_MIDI_ReceiveSysEx(const byte *data, uint16_t size) { 
	msSystem.slog("MIDI(rtp): sysex size:");
	msSystem.slogln(String(size));
};

void _RTP_MIDI_ReceiveTimeCodeQuarterFrame(byte data) {
	msSystem.slog(" TCQF data:");
	msSystem.slogln(String(data));
};

void _RTP_MIDI_ReceiveSongPosition(unsigned short beats) {
	msSystem.slog(" SPP beats:");
	msSystem.slogln(String(beats));
};

void _RTP_MIDI_ReceiveSongSelect(byte songnumber) {
	msSystem.slog("MIDI(rtp): SONG number:");
	msSystem.slogln(String(songnumber));
};

void _RTP_MIDI_ReceiveTuneRequest() {
	msSystem.slogln("MIDI(rtp): Tune Request");
};

void _RTP_MIDI_ReceiveClock() {
	msSystem.slogln("MIDI(rtp): Clock");
};

void _RTP_MIDI_ReceiveStart() {
	msSystem.slogln("MIDI(rtp): START");

};

void _RTP_MIDI_ReceiveContinue() {
	msSystem.slogln("MIDI(rtp): CONTINUE");

};

void _RTP_MIDI_ReceiveStop() {
	msSystem.slogln("MIDI(rtp): STOP");

};

void _RTP_MIDI_ReceiveActiveSensing() {
	msSystem.slogln("MIDI(rtp): ACTIVE SENSE");

};

void _RTP_MIDI_ReceiveReset() {
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
	AppleMIDI.OnConnected(_RTP_MIDI_Connected);
	AppleMIDI.OnDisconnected(_RTP_MIDI_Disconnected);
	AppleMIDI.OnReceiveNoteOff(_RTP_MIDI_Note_Off);
	AppleMIDI.OnReceiveNoteOn(_RTP_MIDI_Note_On);
	AppleMIDI.OnReceiveAfterTouchPoly(_RTP_MIDI_ReceiveAfterTouchPoly);
	AppleMIDI.OnReceiveControlChange(_RTP_MIDI_ReceiveControlChange);
	AppleMIDI.OnReceiveProgramChange(_RTP_MIDI_ReceiveProgramChange);
	AppleMIDI.OnReceiveAfterTouchChannel(_RTP_MIDI_ReceiveAfterTouchChannel);
	AppleMIDI.OnReceivePitchBend(_RTP_MIDI_ReceivePitchBend);
	AppleMIDI.OnReceiveSysEx(_RTP_MIDI_ReceiveSysEx);
	AppleMIDI.OnReceiveTimeCodeQuarterFrame(_RTP_MIDI_ReceiveTimeCodeQuarterFrame);
	AppleMIDI.OnReceiveSongPosition(_RTP_MIDI_ReceiveSongPosition);
	AppleMIDI.OnReceiveSongSelect(_RTP_MIDI_ReceiveSongSelect);
	AppleMIDI.OnReceiveTuneRequest(_RTP_MIDI_ReceiveTuneRequest);
	AppleMIDI.OnReceiveClock(_RTP_MIDI_ReceiveClock);
	AppleMIDI.OnReceiveStart(_RTP_MIDI_ReceiveStart);
	AppleMIDI.OnReceiveContinue(_RTP_MIDI_ReceiveContinue);
	AppleMIDI.OnReceiveStop(_RTP_MIDI_ReceiveStop);
	AppleMIDI.OnReceiveActiveSensing(_RTP_MIDI_ReceiveActiveSensing);
	AppleMIDI.OnReceiveReset(_RTP_MIDI_ReceiveReset);
}


#endif // CONFIG_MIDI_RTP_ENABLE
