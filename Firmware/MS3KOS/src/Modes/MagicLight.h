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

	bool dir = false;
	int d = 10;
	uint8_t lookup[6][3] = { {0, 1, 2}, {1, 0, 2}, {2, 0, 1}, {0, 2, 1}, {1, 2, 0}, {2, 1, 0} };
	uint8_t lookupindex = 0;

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
		if (lMode >= 1) {
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

			if (lMode == 1)
			{
				for (int index = 0; index < 3; index++) {
					startToEndChannel(start, end, d, lookup[lookupindex][index], 255);
				}
			}
			else if (lMode == 2)
			{
				startToEndZigZag(start, end, 1, 255, 255, 255);
			}

			lookupindex = (lookupindex + 1) % 6;
			dir = (dir + 1) % 2;
		}

		delay(55);

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
				msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
				msSystem.msLEDs.setLED(i, r, g, b);
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
