class MagicRemoteMode : public MagicShifterBaseMode {

  public:
  	MagicRemoteMode() {
  		modeName = "Remote";
  	}

	virtual void start() {
		for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
			msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
			msGlobals.ggRGBLEDBuf[i+1] = i * 4;
			msGlobals.ggRGBLEDBuf[i+2] = 255 - i * 16;
			msGlobals.ggRGBLEDBuf[i+3] = 0;
		}
	}
	virtual void stop(void) {
	}
	virtual bool step(void) {
		msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
		msSystem.msLEDs.updateLEDs();
		delay(10);
	}
};