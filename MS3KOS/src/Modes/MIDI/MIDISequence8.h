

class MIDISequence8Mode : public MagicShifterBaseMode {
private:
	static int countdownTicks;
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
};

