#ifndef _MAGICLIGHT_MODE_H
#define _MAGICLIGHT_MODE_H

//
// render some magic lights
// 

class MagicLightMode : public MagicShifterBaseMode {

  private:
	int frame = 0;
	int xx = 0;
	int pDelay = 230;

	bool dir = false;

	int d = 10;
	uint8_t lookup[6][3] = { {0, 1, 2}, {1, 0, 2}, {2, 0, 1}, {0, 2, 1}, {1, 2, 0}, {2, 1, 0} };
	uint8_t lookupindex = 0;
	bool firstRun = false;
	int centerMode = 0;

	MS3KG_App_Light &_light = msGlobals.pbuf.apps.light;

public:

	MagicLightMode() { 
		_light.colorIndex = 6;
		_light.triggerSpeed = 0; // milliseconds

	 	modeName = "Light";
	}

	// depending on button-presses, let the user select
	// sub-mode options
    bool lightSubModeSelector() {

		int old_light_mode = _light.mode;
		int new_light_mode = old_light_mode;
		int trigger_time = _light.triggerSpeed;

		if (firstRun)
			firstRun = false;
		else
			msSystem.step();

		if ((msSystem.msButtons.msBtnPwrHit)  ) {
			centerMode++;
			msSystem.msButtons.msBtnPwrHit = false;
		} else {
			trigger_time++;
		}

		if (msSystem.msButtons.msBtnAHit) {
			
			new_light_mode--;

			msSystem.msButtons.msBtnAHit = false;
		}
		if (msSystem.msButtons.msBtnBHit) {
			
			new_light_mode++;

			msSystem.msButtons.msBtnBHit = false;
		}


		if (new_light_mode < _MS3KG_App_Light_Mode_MIN)
			new_light_mode = _MS3KG_App_Light_Mode_MAX;

		if (new_light_mode > _MS3KG_App_Light_Mode_MAX)
			new_light_mode = _MS3KG_App_Light_Mode_MIN;


		if (new_light_mode != old_light_mode) {

			_light.mode = (MS3KG_App_Light_Mode)new_light_mode;

			msSystem.slog("_light.mode: ");
			msSystem.slogln(String(_light.mode));

			msSystem.slog("_light.triggerSpeed: ");
			msSystem.slogln(String(_light.triggerSpeed));

			msSystem.slog("_light.trigger_time: ");
			msSystem.slogln(String(trigger_time));

			msSystem.msLEDs.fillLEDs(255, 255, 255, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
			delay(10);
			msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
		}

		// .. propagate the menu mode, in case its activited..
		return msSystem.modeMenuActivated;
	}

	void start() {
	} 

	void stop(void) {
	}

	bool step() {
		frame++;
		firstRun = true;
		int start, end;

		lightSubModeSelector();

		// rainbow
		if (_light.mode == MS3KG_App_Light_Mode_RAINBOW) {
			if (frame % pDelay == 0) {
				msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
				msSystem.msLEDs.setLED((xx + 0 * 3) & 0xf, 255, 0, 0, msGlobals.ggBrightness);

				msSystem.msLEDs.setLED((xx + 1 * 3) & 0xf, 255, 255, 0, msGlobals.ggBrightness);
				msSystem.msLEDs.setLED((xx + 2 * 3) & 0xf, 0, 255, 0, msGlobals.ggBrightness);

				msSystem.msLEDs.setLED((xx + 3 * 3) & 0xf, 0, 255, 255, msGlobals.ggBrightness);
				msSystem.msLEDs.setLED((xx + 4 * 3) & 0xf, 0, 0, 255, msGlobals.ggBrightness);

				xx++;
				msSystem.msLEDs.updateLEDs();
			}

			if (centerMode > 0) {
				centerMode--;
				pDelay += 1 + pDelay / 2;
				/*if (pDelay > 250 && pDelay < 500) {
					pDelay = 1000;
				} else */
				if (pDelay >= 5000) {
					pDelay = 1;
				}
			}
		}

		// normal
		if (_light.mode == MS3KG_App_Light_Mode_NORMAL) {
			int r=0,g=0,b=0;

			int ii = _light.colorIndex+1;
			
			if (ii & 1) r = 255;
			if (ii & 2) g = 255;
			if (ii & 4) b = 255;

			msSystem.msLEDs.fillLEDs(r, g, b, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();

			if (centerMode > 0) {
				centerMode--;
				_light.colorIndex = (_light.colorIndex + 1) % 7;
			}
		}

		if (_light.mode == MS3KG_App_Light_Mode_SCANNER_BW) {

			if (centerMode) {
				centerMode = 0;
			} else {
				centerMode = 1;
			}

			msSystem.slog("centerMode: ");
			msSystem.slogln(String(centerMode));

			if (dir)
			{
				start = 0;
				end = MAX_LEDS - 1;
			}
			else
			{
				start = MAX_LEDS - 1;
				end = 0;
			}
			startToEndZigZag(start, end, 1, 255, 255, 255);
			dir = (dir + 1) % 2;

		}

		// scanner 
		if (_light.mode == MS3KG_App_Light_Mode_SCANNER_RGB) {
			if (dir)
			{
				start = 0;
				end = MAX_LEDS - 1;
			}
			else
			{
				start = MAX_LEDS - 1;
				end = 0;
			}

			if (centerMode > 0) {
				centerMode--;
	
				if (_light.mode == MS3KG_App_Light_Mode_SCANNER_RGB)
				{
					for (int index = 0; index < 3; index++) {
						startToEndChannel(start, end, d, lookup[lookupindex][index], 255);
					}
				}
				else if (_light.mode == MS3KG_App_Light_Mode_SCANNER_BW)
				{
					startToEndZigZag(start, end, 1, 255, 255, 255);
				}

				lookupindex = (lookupindex + 1) % 6;
				dir = (dir + 1) % 2;
			}
			else { // MS3KG_App_Light_Mode_SCANNER_RGB_BW
				msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
				msSystem.msLEDs.setLED(start, 255, 255, 255, msGlobals.ggBrightness);
				msSystem.msLEDs.updateLEDs();
			}

			if (centerMode) {
				centerMode = 0;
			} else {
				centerMode = 1;
			}

		}

		return true;
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
			if (lightSubModeSelector()) {
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
				if (lightSubModeSelector()) {
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
