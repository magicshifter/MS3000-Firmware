

#ifdef CONFIG_ENABLE_MIDI_RTP

#include <WiFiUdp.h>

// !J! 
#define APPLEMIDI_DEBUG 1
#include "msSystem.h"
#include "AppleMIDI.h"

extern MagicShifterSystem msSystem;

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

#warning "RTP MIDI Enabled."

// Debug RTP MIDI event handlers:
// -----------------------------------------------------------------------------
void _RTP_MIDI_Connected(uint32_t ssrc, char* name) {
	msSystem.slog("MIDI(rtp): Connected to session " + String(name));
}

void _RTP_MIDI_Disconnected(uint32_t ssrc) {
	msSystem.slogln("MIDI(rtp): Disconnected from session");
}

void _RTP_MIDI_Note_On(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): Note channel:" + String(channel) +
						  		  " noteOn:" + String(note) + 
								    " vel.:" + String(velocity));
}

void _RTP_MIDI_Note_Off(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): Note channel:" + String(channel) + 
							     " noteOff:" + String(note) + 
			  			   	        " vel.:" + String(velocity));
}

void _RTP_MIDI_ReceiveAfterTouchPoly(byte channel, byte note, byte velocity) {
	msSystem.slog("MIDI(rtp): channel:" + String(channel) +
				   " atouch poly note:" + String(note) +
							   " vel.:" + String(velocity));
};

void _RTP_MIDI_ReceiveControlChange(byte channel, byte cc1, byte cc2) {
	msSystem.slog("MIDI(rtp): channel:" + String(channel) +
								" cc1:" + String(cc1) +
								" cc2:" + String(cc2));
};

void _RTP_MIDI_ReceiveProgramChange(byte channel, byte program) {
	msSystem.slog("MIDI(rtp): channel:" + String(channel) +
					 " change program:" + String(program));
};

void _RTP_MIDI_ReceiveAfterTouchChannel(byte channel, byte atouch) {
	msSystem.slog("MIDI(rtp): channel:" + String(channel) +
							 " atouch:" + String(atouch));
};

void _RTP_MIDI_ReceivePitchBend(byte channel, int bend) {
	msSystem.slog("MIDI(rtp): channel:" + String(channel) + 
				               " bend:" + String(bend));
};

void _RTP_MIDI_ReceiveSysEx(const byte *data, uint16_t size) { 
	msSystem.slog("MIDI(rtp): sysex size:" + String(size));
};

void _RTP_MIDI_ReceiveTimeCodeQuarterFrame(byte data) {
	msSystem.slog(" TCQF data:" + String(data));
};

void _RTP_MIDI_ReceiveSongPosition(unsigned short beats) {
	msSystem.slog(" SPP beats:" + String(beats));
};

void _RTP_MIDI_ReceiveSongSelect(byte songnumber) {
	msSystem.slog("MIDI(rtp): SONG number:" + String(songnumber));
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

// !J! TODO: additional rtpMIDI handlers:
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


#endif // CONFIG_ENABLE_MIDI_RTP
