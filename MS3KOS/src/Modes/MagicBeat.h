class MagicBeatMode : public MagicShifterBaseMode {

private:

	float avgZ;
	int colorIndex;

	MS3KG_App_Beat &_beat = msGlobals.pbuf.applications.beat;


public:
	MagicBeatMode() {
		modeName = "Beat";

		colorIndex = 1;

		msGlobals.pbuf.has_applications = 1;
        msGlobals.pbuf.applications.has_current = 1;   
		msGlobals.pbuf.applications.has_beat = 1;

		// _beat = &msGlobals.pbuf.applications.beat;
	
		_beat.color.B = 255;
		_beat.sensitivity = 1;
		_beat.mode = MS3KG_App_Beat_Mode_CENTER;

		_beat.has_color = 1;
        _beat.has_mode = 1;
        _beat.has_sensitivity = 1;
	}

	virtual void start() {


	}

	virtual void stop(void) {
	}

	const int axis = 2;
	const float avgF = 1./32.0;

	virtual bool step(void) {
		float xPos;

		avgZ = avgZ * (1 - avgF) + avgF * msGlobals.ggAccel[axis];

		xPos = (msGlobals.ggAccel[axis] - avgZ);

		xPos *= 1024 >> _beat.sensitivity;

#if 0 
		printf( " subMode: %d" \
				" sensitivity: %d" \
				" colorIndex: %d" \
				" R: %d " \
				" G: %d " \
				" B: %d \n",
					_beat.mode,
					_beat.sensitivity,
					colorIndex,
					_beat.color.R,
					_beat.color.G,
					_beat.color.B);
#endif

		if (_beat.mode == MS3KG_App_Beat_Mode_SIDE) {
			if (xPos < 0)
				xPos = -xPos;

			xPos -= 5;
			if (xPos > MAX_LEDS)
				xPos = MAX_LEDS;
		}
		else if (_beat.mode == MS3KG_App_Beat_Mode_CENTER) {
			xPos += 7.5; //(MAX_LEDS - 1) / 2.0f;
		}

		msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);

		if (_beat.mode == MS3KG_App_Beat_Mode_SIDE) {
			for (int i = 0; i < xPos; i++)
				msSystem.msLEDs.setLED((MAX_LEDS-1) - i, 
					_beat.color.R, 
					_beat.color.G, 
					_beat.color.B, 
					msGlobals.ggBrightness);
		}
		else if (_beat.mode == MS3KG_App_Beat_Mode_CENTER) {
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
		
		if ((msSystem.msButtons.msBtnALongHit) || 
			(msSystem.msButtons.msBtnBLongHit)) {

			msSystem.msButtons.msBtnALongHit = false;
			msSystem.msButtons.msBtnBLongHit = false;

			if (_beat.mode == MS3KG_App_Beat_Mode_CENTER) {
				_beat.mode = MS3KG_App_Beat_Mode_SIDE;
			}
			else {
				_beat.mode = MS3KG_App_Beat_Mode_CENTER;
			}

		}

		if (msSystem.msButtons.msBtnAHit) {
			_beat.sensitivity = (_beat.sensitivity + 1) % 6;
			msSystem.msButtons.msBtnAHit = false;
		}

		if (msSystem.msButtons.msBtnBHit) {
			_beat.sensitivity = (_beat.sensitivity + 5) % 6;
			msSystem.msButtons.msBtnBHit = false;
		}

		if (msSystem.msButtons.msBtnPwrHit) {
			colorIndex++;
			if (colorIndex > 7) colorIndex = 1;
			_beat.color.R = (colorIndex & 1) ? 255 : 0;
			_beat.color.G = (colorIndex & 2) ? 255 : 0;
			_beat.color.B = (colorIndex & 4) ? 255 : 0;
			msSystem.msButtons.msBtnPwrHit = false;
		}

		return true;
	}


};
