/**
 * Magic Bike Mode:
 * 		sub-mode: front
 				bright-white
 					btn-a turn left
 					btn-b turn right
 *		sub-mode; rear
 				if (wifi connected) to front shifter, blink when turning
 *		
**/


class MagicBikeMode : public MagicShifterBaseMode {

  public:
  	MagicBikeMode() {
  		modeName = "Bike";
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
		return true;
	}
};