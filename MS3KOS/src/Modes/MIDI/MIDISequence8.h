

class MIDISequence8Mode : public MagicShifterBaseMode {
private:
	static int sync_count;

	MS3KG_App_Sequi &_sequi = msGlobals.pbuf.applications.sequi;

public:
	MIDISequence8Mode() {
		modeName = "Sequi8";
	}

	void start() {
        for (size_t step=0;step<8;step++)
            msSystem.slogln(modeName + " step " + String(step) + " note: " + String());
	}

	void stop(void) {
            msSystem.slogln(modeName + " stopped");
	}

	bool step(void) {

		return true;
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
	}

	void MIDI_Stop()
	{
		msSystem.slogln(modeName + " MIDI Stop");
	}

	void MIDI_Program_Change(byte channel, byte program)
	{
		msSystem.slogln(modeName + " MIDI PC - channel: " + 
									String(channel) + 
									" program: " + 
									String(program));
	}

	void MIDI_Control_Change(byte channel, byte cc1, byte cc2)
	{
		msSystem.slogln(modeName + " MIDI CC - channel: " + 
									String(channel) + 
									" cc1: " + 
									String(cc1) +
									" cc2: " + 
									String(cc2));

	}

	void MIDI_Note_On(byte channel, byte note, byte velocity)
	{
		msSystem.slogln(modeName + " MIDI Note On - channel: " + 
									String(channel) + 
									" note: " + 
									String(note) +
									" vel: " + 
									String(velocity));

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

		msSystem.msLEDs.setLED(LED_NOTE_EVENT, 0, 0, 0);
	}

};

