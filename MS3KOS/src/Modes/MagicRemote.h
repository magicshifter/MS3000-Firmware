class MagicRemoteMode : public MagicShifterBaseMode {

  public:
  	MagicRemoteMode() {
  		modeName = "Remote";
  	}

	virtual void start() {
	}
	virtual void stop(void) {
	}
	virtual bool step(void) {
		msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
		msSystem.msLEDs.updateLEDs();
		delay(10);
	}
};