

class MIDISequence8Mode : public MagicShifterBaseMode {
private:
	static int countdownTicks;

public:
	MIDISequence8Mode() {
		modeName = "Sequi8";
	}

	void dump_sequence() {
		for (size_t sequ_i=0;sequ_i<8;sequ_i++) {
			msSystem.slogln(modeName + " sequence step: " + String( sequ_i ));
			msSystem.slogln(modeName + "          note: " + String( msGlobals.pbuf.applications.sequi.sequence.steps[sequ_i].interval.v ));
			msSystem.slogln(modeName + "        octave: " + String( msGlobals.pbuf.applications.sequi.sequence.steps[sequ_i].octave.o ));
			msSystem.slogln(modeName + "          mode: " + String( msGlobals.pbuf.applications.sequi.sequence.steps[sequ_i].mode ));
		}

	}

	void start() {
		dump_sequence();
	}

	void stop(void) {
            msSystem.slogln(modeName + " stopped");
	}

	bool step(void) {

		if (msGlobals.pbuf.applications.sequi.has_sequence) {
			msSystem.slogln(modeName + "has_sequence!");
			dump_sequence();

		}

		return true;
	}
};

