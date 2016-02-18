class MagicRemoteMode : public MagicShifterBaseMode {

  public:
  	MagicRemoteMode() {
  		modeName = "Rmt";
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