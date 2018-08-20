class MagicBeatMode : public MagicShifterBaseMode {

private:

	int frame = 0;
	float avgZ = 0;
	int colIdx = 1;
	int32 *sensitivity;
	// int renderMode = 1;

	// Color of our beat
	// uint8_t r, g, b;

public:
	MagicBeatMode() {
		modeName = "Beat";
	}

	virtual void start() {
		// msGlobals.protocolBuffer.modes.beat.beatMode = MS3KG_Modes_Beat_BeatMode_SIDE;
		msGlobals.protocolBuffer.modes.beat.sensitivity = 3;
		sensitivity = &msGlobals.protocolBuffer.modes.beat.sensitivity;
		msGlobals.protocolBuffer.modes.beat.color.R = 255;
	}

	virtual void stop(void) {
	}

	const int axis = 2;
	const float avgF = 1./32.0;

	void slog_sensitivity()
	{
		msSystem.slog("sensitivity:");
		msSystem.slogln(String(*sensitivity));
	}

	virtual bool step(void) {
		float xPos;

		avgZ = avgZ * (1 - avgF) + avgF * msGlobals.ggAccel[axis];

		frame++;

		// if (msGlobals.protocolBuffer.modes.beat.beatMode == 0) {
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

		//} else { // msGlobals.protocolBuffer.modes.beat.beatMode = 1
		xPos = (msGlobals.ggAccel[axis] - avgZ);

		xPos *= 1024 >> msGlobals.protocolBuffer.modes.beat.sensitivity;

		printf("magicBeat: beatMode is %d!\n", msGlobals.protocolBuffer.modes.beat.beatMode);
		printf("magicBeat: sensitivity is %d!\n", msGlobals.protocolBuffer.modes.beat.sensitivity);


			if (msGlobals.protocolBuffer.modes.beat.beatMode == MS3KG_Modes_Beat_BeatMode_SIDE) {
				if (xPos < 0)
					xPos = -xPos;

				xPos -= 5;
				if (xPos > MAX_LEDS)
					xPos = MAX_LEDS;
			}
			else if (msGlobals.protocolBuffer.modes.beat.beatMode == MS3KG_Modes_Beat_BeatMode_CENTER) {
				xPos += 7.5;
			}

			msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);

			if (msGlobals.protocolBuffer.modes.beat.beatMode == MS3KG_Modes_Beat_BeatMode_SIDE) {
				for (int i = 0; i < xPos; i++)
				msSystem.msLEDs.setLED((MAX_LEDS-1) - i, 
					msGlobals.protocolBuffer.modes.beat.color.R, 
					msGlobals.protocolBuffer.modes.beat.color.G, 
					msGlobals.protocolBuffer.modes.beat.color.B, 
					msGlobals.ggBrightness);
			}
			else if (msGlobals.protocolBuffer.modes.beat.beatMode == MS3KG_Modes_Beat_BeatMode_CENTER) {
				int xPosInt = (int)(xPos);
				float xPosRem = 1 - (xPos - xPosInt);			
			
				// first go one way 
				if (xPosInt >= 0 && xPosInt < MAX_LEDS) {
					msSystem.msLEDs.setLED(xPosInt, 
						(int)(msGlobals.protocolBuffer.modes.beat.color.R * xPosRem), 
						(int)(msGlobals.protocolBuffer.modes.beat.color.G * xPosRem),
						(int)(msGlobals.protocolBuffer.modes.beat.color.B * xPosRem),
						msGlobals.ggBrightness);
				}

				// render the other direction
				xPosInt++;
				xPosRem = 1 - xPosRem;
				if (xPosInt >= 0 && xPosInt < MAX_LEDS) {
					msSystem.msLEDs.setLED(xPosInt, 
							(int)(msGlobals.protocolBuffer.modes.beat.color.R * xPosRem), 
							(int)(msGlobals.protocolBuffer.modes.beat.color.G * xPosRem),
							(int)(msGlobals.protocolBuffer.modes.beat.color.B * xPosRem),
							msGlobals.ggBrightness);
				}
			}
			
			msSystem.msLEDs.updateLEDs();
		
		if ((msSystem.msButtons.msBtnALongHit) || (msSystem.msButtons.msBtnBLongHit)) {
 			if (msGlobals.protocolBuffer.modes.beat.beatMode == MS3KG_Modes_Beat_BeatMode_CENTER) {
				msGlobals.protocolBuffer.modes.beat.beatMode = MS3KG_Modes_Beat_BeatMode_SIDE;
			}
			else
			if (msGlobals.protocolBuffer.modes.beat.beatMode == MS3KG_Modes_Beat_BeatMode_SIDE) {
				msGlobals.protocolBuffer.modes.beat.beatMode = MS3KG_Modes_Beat_BeatMode_CENTER;
			}

			msSystem.msButtons.msBtnALongHit = false;
			msSystem.msButtons.msBtnBLongHit = false;
		}


		if (msSystem.msButtons.msBtnAHit)
		{
			*sensitivity = (*sensitivity + 1) % 6;
			msSystem.msButtons.msBtnAHit = false;
			slog_sensitivity();
		}

		if (msSystem.msButtons.msBtnBHit)
		{
			*sensitivity = (*sensitivity + 5) % 6;
			msSystem.msButtons.msBtnBHit = false;
			slog_sensitivity();
		}

		if (msSystem.msButtons.msBtnPwrHit) {
			colIdx++;
			if (colIdx > 7) colIdx = 1;
			msGlobals.protocolBuffer.modes.beat.color.R = (colIdx & 1) ? 255 : 0;
			msGlobals.protocolBuffer.modes.beat.color.G = (colIdx & 2) ? 255 : 0;
			msGlobals.protocolBuffer.modes.beat.color.B = (colIdx & 4) ? 255 : 0;
			msSystem.msButtons.msBtnPwrHit = false;
		}



	} 
};
