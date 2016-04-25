
//
// RAW MIDI I/O handlers, available for miby
//

// -- MIDI Event Handlers
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
	msMIDIShifter._arp.arpProgramChange(curr_midiview.midi_channel, MIBY_ARG0(a_miby));
}

void MIDI_Control_Change(miby_this_t a_miby)
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

void MIDI_Note_On(miby_this_t a_miby)
{
	msMIDIShifter._arp.arpNoteOn(curr_midiview.midi_channel, MIBY_ARG0(a_miby),
			  MIBY_ARG1(a_miby));
	msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 100);
}

void MIDI_Note_Off(miby_this_t a_miby)
{
	uint8_t note_msg[3];
	note_msg[0] = MIBY_STATUSBYTE(a_miby);
	note_msg[1] = MIBY_ARG0(a_miby);
	note_msg[2] = MIBY_ARG1(a_miby);

	MIDI_Put(note_msg, 3);

	msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 0);
}
