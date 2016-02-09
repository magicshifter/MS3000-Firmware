#ifndef _MAGICLIGHT_MODE_H
#define _MAGICLIGHT_MODE_H

//
// render the magnetometer sensor data
// 

class MagicLightMode : public MagicShifterBaseMode {

  private:
	int frame = 0;
	int lMode = 0;
	int xx = 0;

  public:
	void start() {
	} 

	void stop(void) {
	}

	bool step() {
		if (msSystem.msButtons.msBtnAHit) {
			lMode--;
			msSystem.msButtons.msBtnAHit = false;
		}
		if (msSystem.msButtons.msBtnBHit) {
			lMode++;
			msSystem.msButtons.msBtnBHit = false;
		}
		if (lMode < 0)
			lMode = 2;
		if (lMode > 2)
			lMode = 0;

		
		if (lMode == 0) {
			msSystem.msLEDs.fillLEDs(0, 0, 0);
			msSystem.msLEDs.setLED((xx + 0 * 3) & 0xF, 255, 0, 0, msGlobals.ggBrightness);

			msSystem.msLEDs.setLED((xx + 1 * 3) & 0xF, 255, 255, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.setLED((xx + 2 * 3) & 0xF, 0, 255, 0, msGlobals.ggBrightness);

			msSystem.msLEDs.setLED((xx + 3 * 3) & 0xF, 0, 255, 255, msGlobals.ggBrightness);
			msSystem.msLEDs.setLED((xx + 4 * 3) & 0xF, 0, 0, 255, msGlobals.ggBrightness);

			xx++;
			msSystem.msLEDs.updateLEDs();
		}

		delay(55);

	}

	void update() {
	};
	void reset() {
	};

};

#endif
