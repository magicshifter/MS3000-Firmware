class MagicBeatMode : public MagicShifterBaseMode {

private:

	float avgZ = 0;
	int colorIndex = 1;

	
	int32 *sensitivity = &msGlobals.pbuf.modes.beat.sensitivity;
	
	int *R = &msGlobals.pbuf.modes.beat.color.R;
	int *G = &msGlobals.pbuf.modes.beat.color.G;
	int *B = &msGlobals.pbuf.modes.beat.color.B;

	MS3000GlobalPBuffer_Modes_Beat_BeatMode *beatMode = &msGlobals.pbuf.modes.beat.beatMode;

	const MS3000GlobalPBuffer_Modes_Beat_BeatMode beatModeSIDE = MS3000GlobalPBuffer_Modes_Beat_BeatMode_SIDE;
	const MS3000GlobalPBuffer_Modes_Beat_BeatMode beatModeCENTER = MS3000GlobalPBuffer_Modes_Beat_BeatMode_CENTER;


public:
	MagicBeatMode() {
		modeName = "Beat";
	}

	virtual void start() {
		*sensitivity = 3;
		*beatMode = beatModeCENTER;
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

		xPos = (msGlobals.ggAccel[axis] - avgZ);

		xPos *= 1024 >> *sensitivity;

		printf("magicBeat: beatMode is %d!\n", *beatMode);
		printf("magicBeat: sensitivity is %d!\n", *sensitivity);

		if (*beatMode == beatModeSIDE) {
			if (xPos < 0)
				xPos = -xPos;

			xPos -= 5;
			if (xPos > MAX_LEDS)
				xPos = MAX_LEDS;
		}
		else if (*beatMode == beatModeCENTER) {
			xPos += 7.5;
		}

		msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);


		if (*beatMode == beatModeSIDE) {
			for (int i = 0; i < xPos; i++)
				msSystem.msLEDs.setLED((MAX_LEDS-1) - i, *R, *G, *B, msGlobals.ggBrightness);
		}
		else if (*beatMode == beatModeCENTER) {
			int xPosInt = (int)(xPos);
			float xPosRem = 1 - (xPos - xPosInt);			
			
				// first go one way 
			if (xPosInt >= 0 && xPosInt < MAX_LEDS) {
				msSystem.msLEDs.setLED(xPosInt, (int)(*R * xPosRem), (int)(*G * xPosRem), (int)(*B * xPosRem), msGlobals.ggBrightness);
			}

				// render the other direction
			xPosInt++;
			xPosRem = 1 - xPosRem;
			if (xPosInt >= 0 && xPosInt < MAX_LEDS) {
				msSystem.msLEDs.setLED(xPosInt, 
					(int)(*R * xPosRem), (int)(*G * xPosRem), (int)(*B * xPosRem), msGlobals.ggBrightness);
			}
		}


		msSystem.msLEDs.updateLEDs();
		
		if ((msSystem.msButtons.msBtnALongHit) || (msSystem.msButtons.msBtnBLongHit)) {
			if (*beatMode == beatModeCENTER) {
				*beatMode = beatModeSIDE;
			}
			else
				if (*beatMode == beatModeSIDE) {
					*beatMode = beatModeCENTER;
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
				colorIndex++;
				if (colorIndex > 7) colorIndex = 1;
				msGlobals.pbuf.modes.beat.color.R = (colorIndex & 1) ? 255 : 0;
				msGlobals.pbuf.modes.beat.color.G = (colorIndex & 2) ? 255 : 0;
				msGlobals.pbuf.modes.beat.color.B = (colorIndex & 4) ? 255 : 0;
				msSystem.msButtons.msBtnPwrHit = false;
			}



		} 
	};
