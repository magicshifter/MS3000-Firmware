class MagicBeatMode : public MagicShifterBaseMode {

private:

	float avgZ = 0;
	int colorIndex = 1;

	MS3000GlobalPBuffer_Modes_Beat _beat;

	const MS3000GlobalPBuffer_Modes_Beat_BeatMode beatModeSIDE = MS3000GlobalPBuffer_Modes_Beat_BeatMode_SIDE;
	const MS3000GlobalPBuffer_Modes_Beat_BeatMode beatModeCENTER = MS3000GlobalPBuffer_Modes_Beat_BeatMode_CENTER;

public:
	MagicBeatMode() {
		modeName = "Beat";
	}

	virtual void start() {

		_beat.sensitivity = 3;
		_beat.beatMode = beatModeCENTER;
	}

	virtual void stop(void) {
	}

	const int axis = 2;
	const float avgF = 1./32.0;

	void slog_sensitivity()
	{
		msSystem.slog("sensitivity:");
		msSystem.slogln(String(_beat.sensitivity));
	}

	virtual bool step(void) {
		float xPos;

		_beat = msGlobals.pbuf.modes.beat;

		avgZ = avgZ * (1 - avgF) + avgF * msGlobals.ggAccel[axis];

		xPos = (msGlobals.ggAccel[axis] - avgZ);

		xPos *= 1024 >> _beat.sensitivity;

		printf("magicBeat: beatMode: %d" \
				" sensitivity: %d" \ 
				" R: %d " \
				" G: %d " \
				" B: %d \n",
					_beat.beatMode,
					_beat.sensitivity,
					_beat.color.R,
					_beat.color.G,
					_beat.color.B);


		if (_beat.beatMode == beatModeSIDE) {
			if (xPos < 0)
				xPos = -xPos;

			xPos -= 5;
			if (xPos > MAX_LEDS)
				xPos = MAX_LEDS;
		}
		else if (_beat.beatMode == beatModeCENTER) {
			xPos += 7.5;
		}

		msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);

		if (_beat.beatMode == beatModeSIDE) {
			for (int i = 0; i < xPos; i++)
				msSystem.msLEDs.setLED((MAX_LEDS-1) - i, 
					_beat.color.R, 
					_beat.color.G, 
					_beat.color.B, 
					msGlobals.ggBrightness);
		}
		else if (_beat.beatMode == beatModeCENTER) {
			int xPosInt = (int)(xPos);
			float xPosRem = 1 - (xPos - xPosInt);			
			
			if (xPosInt >= 0 && xPosInt < MAX_LEDS) {
				msSystem.msLEDs.setLED(xPosInt, 
					(int)(_beat.color.R * xPosRem), 
					(int)(_beat.color.G * xPosRem), 
					(int)(_beat.color.B * xPosRem), 
					msGlobals.ggBrightness);
			}

			xPosInt++;
			xPosRem = 1 - xPosRem;
			if (xPosInt >= 0 && xPosInt < MAX_LEDS) {
				msSystem.msLEDs.setLED(xPosInt, 
					(int)(_beat.color.R * xPosRem), 
					(int)(_beat.color.G * xPosRem), 
					(int)(_beat.color.B * xPosRem), 
					msGlobals.ggBrightness);
			}
		}


		msSystem.msLEDs.updateLEDs();
		
		if ((msSystem.msButtons.msBtnALongHit) || (msSystem.msButtons.msBtnBLongHit)) {
			if (_beat.beatMode == beatModeCENTER) {
				_beat.beatMode = beatModeSIDE;
			}
			else
				if (_beat.beatMode == beatModeSIDE) {
					_beat.beatMode = beatModeCENTER;
				}

				msSystem.msButtons.msBtnALongHit = false;
				msSystem.msButtons.msBtnBLongHit = false;
			}


			if (msSystem.msButtons.msBtnAHit)
			{
				_beat.sensitivity = (_beat.sensitivity + 1) % 6;
				msSystem.msButtons.msBtnAHit = false;
				slog_sensitivity();
			}

			if (msSystem.msButtons.msBtnBHit)
			{
				_beat.sensitivity = (_beat.sensitivity + 5) % 6;
				msSystem.msButtons.msBtnBHit = false;
				slog_sensitivity();
			}

			if (msSystem.msButtons.msBtnPwrHit) {
				colorIndex++;
				if (colorIndex > 7) colorIndex = 1;
				_beat.color.R = (colorIndex & 1) ? 255 : 0;
				_beat.color.G = (colorIndex & 2) ? 255 : 0;
				_beat.color.B = (colorIndex & 4) ? 255 : 0;
				msSystem.msButtons.msBtnPwrHit = false;
			}



		} 
	};
