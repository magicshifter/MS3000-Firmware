

class MIDISequence8Mode : public MagicShifterBaseMode {
private:
	static int countdownTicks;

public:
	MIDISequence8Mode() {
		modeName = "Sequi8";
	}

	void start() {

		msSystem.slogln("sequi steps: " + String( (void *) msGlobals.pbuf.applications.sequi.sequence.steps[0] ));

        for (size_t step=0;step<8;step++)
            msSystem.slogln(modeName + " step " + String(step) + " note: " + String());
	}

	void stop(void) {
            msSystem.slogln(modeName + " stopped");
	}

	bool step(void) {

		return true;
	}
};

