

class MIDISequence8Mode : public MagicShifterBaseMode {
private:
	static int countdownTicks;

public:
	MIDISequence8Mode() {
		modeName = "Sequi8";
	}

	void start() {
        for (size_t sequ_i=0;sequ_i<8;sequ_i++) {
            msSystem.slogln(modeName + " sequence step: " + String( sequ_i ));
			msSystem.slogln(modeName + "          note: " + String( msGlobals.pbuf.applications.sequi.sequence.steps[sequ_i].step ));
			msSystem.slogln(modeName + "        octave: " + String( msGlobals.pbuf.applications.sequi.sequence.steps[sequ_i].octave.v ));
        }
	}

	void stop(void) {
            msSystem.slogln(modeName + " stopped");
	}

	bool step(void) {

		return true;
	}
};

