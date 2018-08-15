class MagicBeatMode : public MagicShifterBaseMode {

private:

	int frame = 0;
	float avgZ = 0;
	int colIdx = 1;
	int sensitivity = 3;
	int renderMode = 1;

	// Color of our beat
	uint8_t r, g, b;

public:
	MagicBeatMode() {
		modeName = "Beat";
	}

	virtual void start() {
		r = g = 0;
		b = 255;
	}

	virtual void stop(void) {
	}

	const int axis = 2;
	const float avgF = 1./32.0;

	void slog_sensitivity()
	{
		msSystem.slog("sensitivity:");
		msSystem.slogln(String(sensitivity));
	}

	virtual bool step(void) {
		float xPos;

		avgZ = avgZ * (1 - avgF) + avgF * msGlobals.ggAccel[axis];

		frame++;

		// if (renderMode == 0) {
		// 	xPos = 7 * (1 << FIXED_SHIFT) + (1 << (FIXED_SHIFT - 1)) + (((msGlobals.ggAccelCount[2] - avgZ) * 40) >> sensitivity);
		// 	if (xPos < 0)
		// 		xPos = 0;
		// 	if (xPos > 15 * (1 << FIXED_SHIFT))
		// 		xPos = 15 * (1 << FIXED_SHIFT);

		// 	int8_t xPosInt = xPos >> FIXED_SHIFT;			
		// 	uint8_t xPosRemainder = (xPos & (1 << FIXED_SHIFT) - 1);

		// msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
			
		// 	msSystem.msLEDs.setLED(xPosInt + 1, (r * xPosRemainder) >> FIXED_SHIFT, (g * xPosRemainder) >> FIXED_SHIFT, (b * xPosRemainder) >> FIXED_SHIFT,
		// 		msGlobals.ggBrightness);

		// 	xPosRemainder = ((1 << FIXED_SHIFT) - 1) - xPosRemainder;			

		// 	msSystem.msLEDs.setLED(xPosInt, (r * xPosRemainder) >> FIXED_SHIFT, (g * xPosRemainder) >> FIXED_SHIFT, (b * xPosRemainder) >> FIXED_SHIFT,
		// 		msGlobals.ggBrightness);

		// 	msSystem.msLEDs.updateLEDs();

		//} else { // renderMode = 1
			xPos = (msGlobals.ggAccel[axis] - avgZ);

			xPos *= 1024 >> sensitivity;

		renderMode = msGlobals.protocolBuffer.modes.beat.beatMode;
		sensitivity = msGlobals.protocolBuffer.modes.beat.sensitivity;

		r = msGlobals.protocolBuffer.modes.beat.color.R;
        g = msGlobals.protocolBuffer.modes.beat.color.G;
        b = msGlobals.protocolBuffer.modes.beat.color.B;


		printf("magicBeat: beatMode is %d!\n", msGlobals.protocolBuffer.modes.beat.beatMode);
		printf("magicBeat: sensitivity is %d!\n", msGlobals.protocolBuffer.modes.beat.sensitivity);


			if (renderMode == 0) {
				if (xPos < 0)
					xPos = -xPos;

				xPos -= 5;
				if (xPos > MAX_LEDS)
					xPos = MAX_LEDS;
			}
			else {
				xPos += 7.5;
			}

			msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);

			if (renderMode == 0) {
				for (int i = 0; i < xPos; i++)
				msSystem.msLEDs.setLED((MAX_LEDS-1) - i, r, g, b, msGlobals.ggBrightness);
			}
			else {
				int xPosInt = (int)(xPos);
				float xPosRem = 1 - (xPos - xPosInt);			
			
				if (xPosInt >= 0 && xPosInt < MAX_LEDS) {
					msSystem.msLEDs.setLED(xPosInt, 
						(int)(r * xPosRem), 
						(int)(g * xPosRem),
						(int)(b * xPosRem),
						msGlobals.ggBrightness);
				}

				xPosInt++;
				xPosRem = 1 - xPosRem;
				if (xPosInt >= 0 && xPosInt < MAX_LEDS) {
					msSystem.msLEDs.setLED(xPosInt, 
							(int)(r * xPosRem), 
							(int)(g * xPosRem),
							(int)(b * xPosRem),
							msGlobals.ggBrightness);
				}
			}
			
			msSystem.msLEDs.updateLEDs();
		//}

		// 
		if (msSystem.msButtons.msBtnALongHit) {
			renderMode++;
			if (renderMode > 1) renderMode = 0;
			msSystem.msButtons.msBtnALongHit = false;

		}
		if (msSystem.msButtons.msBtnBLongHit) {
			renderMode--;
			if (renderMode < 0) renderMode = 1;
			msSystem.msButtons.msBtnBLongHit = false;
		}

		if (msSystem.msButtons.msBtnAHit)
		{
			sensitivity = (sensitivity + 1) % 6;
			msSystem.msButtons.msBtnAHit = false;
			slog_sensitivity();
		}

		if (msSystem.msButtons.msBtnBHit)
		{
			sensitivity = (sensitivity + 5) % 6;
			msSystem.msButtons.msBtnBHit = false;
			slog_sensitivity();
		}

		if (msSystem.msButtons.msBtnPwrHit) {
			colIdx++;
			if (colIdx > 7) colIdx = 1;
			r = (colIdx & 1) ? 255 : 0;
			g = (colIdx & 2) ? 255 : 0;
			b = (colIdx & 4) ? 255 : 0;
			msSystem.msButtons.msBtnPwrHit = false;
		}



	} 
};
