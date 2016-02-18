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
	int pDelay = 230;

	bool dir = false;
	int d = 10;
	uint8_t lookup[6][3] = { {0, 1, 2}, {1, 0, 2}, {2, 0, 1}, {0, 2, 1}, {1, 2, 0}, {2, 1, 0} };
	uint8_t lookupindex = 0;
	bool firstRun = false;
	int centerAction = 0;

	int colorIdx = 6;

  public:

	MagicLightMode() { 
		 	modeName = "Lgt";
	}

    bool hackUIResponsive() {
		int oldlMode = lMode;
		if (firstRun)
			firstRun = false;
		else
			msSystem.loop();

		if (msSystem.msButtons.msBtnPwrHit) {
			msSystem.slogln("btnPwr");
			centerAction++;
			msSystem.msButtons.msBtnPwrHit = false;
		}

		if (msSystem.msButtons.msBtnAHit) {
			msSystem.slogln("btnA");
			lMode--;
			msSystem.msButtons.msBtnAHit = false;
		}
		if (msSystem.msButtons.msBtnBHit) {
			msSystem.slogln("btnB");
			lMode++;
			msSystem.msButtons.msBtnBHit = false;
		}
		if (lMode < 0)
			lMode = 3;
		if (lMode > 3)
			lMode = 0;

		if (lMode != oldlMode) {
			msSystem.slog("lMode: ");
			msSystem.slogln(String(lMode));
			msSystem.msLEDs.fillLEDs(255, 255, 255, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
			delay(10);
			msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
		}


		return msSystem.modeMenuActivated;
	}

	void start() {
	} 

	void stop(void) {
	}

	bool step() {
		frame++;
		firstRun = true;

		hackUIResponsive();

		if (lMode == 0) {
			if (frame % pDelay == 0) {
				msSystem.msLEDs.fillLEDs(0, 0, 0);
				msSystem.msLEDs.setLED((xx + 0 * 3) & 0xF, 255, 0, 0, msGlobals.ggBrightness);

				msSystem.msLEDs.setLED((xx + 1 * 3) & 0xF, 255, 255, 0, msGlobals.ggBrightness);
				msSystem.msLEDs.setLED((xx + 2 * 3) & 0xF, 0, 255, 0, msGlobals.ggBrightness);

				msSystem.msLEDs.setLED((xx + 3 * 3) & 0xF, 0, 255, 255, msGlobals.ggBrightness);
				msSystem.msLEDs.setLED((xx + 4 * 3) & 0xF, 0, 0, 255, msGlobals.ggBrightness);

				xx++;
				msSystem.msLEDs.updateLEDs();
			}

			if (centerAction > 0) {
				centerAction--;
				pDelay += 1 + pDelay / 2;
				if (pDelay > 250 && pDelay < 500) {
					pDelay = 1000;
				} else if (pDelay >= 500) {
					pDelay = 1;
				}
			}
		}
		if (lMode == 1) {
			int r=0,g=0,b=0;
			int ii = colorIdx+1;
			if (ii & 1) r = 255;
			if (ii & 2) g = 255;
			if (ii & 4) b = 255;

			msSystem.msLEDs.fillLEDs(r, g, b, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();

			if (centerAction > 0) {
				centerAction--;
				colorIdx = (colorIdx + 1) % 7;
			}
		}
		if (lMode >= 2) {
			int start, end;
			if (dir)
			{
				start = 0;
				end = 15;
			}
			else
			{
				start = 15;
				end = 0;
			}

			if (centerAction > 0) {
				centerAction--;
	
				if (lMode == 2)
				{
					for (int index = 0; index < 3; index++) {
						startToEndChannel(start, end, d, lookup[lookupindex][index], 255);
					}
				}
				else if (lMode == 3)
				{
					startToEndZigZag(start, end, 1, 255, 255, 255);
				}

				lookupindex = (lookupindex + 1) % 6;
				dir = (dir + 1) % 2;
			}
			else {
				msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
				msSystem.msLEDs.setLED(start, 255, 255, 255, msGlobals.ggBrightness);
				msSystem.msLEDs.updateLEDs();
			}
		}
	}

	void update() {
	};
	void reset() {
	};

	void startToEndChannel(uint8_t start, uint8_t end, int d, int channel, int color)
	{
		int i;

		i = start;
		do {
			if (hackUIResponsive()) {
				return;
			}

			msSystem.msLEDs.setAllChannel(channel, 0);
			msSystem.msLEDs.setChannel(i, channel, color);
			msSystem.msLEDs.updateLEDs();
			if (d)
				delay(d);
			if (i < end)
				i++;
			else
				i--;
		} while (i != end);
		msSystem.msLEDs.setAllChannel(channel, 0);
		msSystem.msLEDs.setChannel(i, channel, color);
		msSystem.msLEDs.updateLEDs();
	}

	void startToEndZigZag(uint8_t start, uint8_t end, int d, uint8_t r, uint8_t g, uint8_t b)
	{
		int i;
		uint8_t lastEnd = end;
		uint8_t currentStart = start;
		uint8_t currentEnd = start;

		if (end < start)
			end = start-1;
		else
			end = start+1;

		for (;;)
		{
			i = currentStart;
			do {
				if (hackUIResponsive()) {
									return;
				}

				msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
				if (i >= 0 && i < MAX_LEDS)
				msSystem.msLEDs.setLED(i, r, g, b, msGlobals.ggBrightness);
				msSystem.msLEDs.updateLEDs();
				if (d)
					delay(d);
				if (i < currentEnd)
					i++;
				else
					i--;
			} while (i != currentEnd);

			if (i == end)
			{
				if (end == lastEnd)
				{
					if (start < end)
						start++;
					else
						start--;
					if (start == end)
						break;
				}		
				currentStart = end;
				currentEnd = start;
			}
			else
			{
				if (end != lastEnd)
				{
					if (end < lastEnd)
						end++;
					else
						end--;
				}
				currentStart = start;
				currentEnd = end;		
			}
		}

		// last
		msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
		msSystem.msLEDs.setLED(i, r, g, b, msGlobals.ggBrightness);
		msSystem.msLEDs.updateLEDs();
	}

};

#endif
