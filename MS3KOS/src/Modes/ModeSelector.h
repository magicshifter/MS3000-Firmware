//
// the main mode selector for the ms3000 interface
//

class ModeSelectorMode:public MagicShifterBaseMode {

  private:
	// MagicShifterImageAbstr *msImage;

	// local POVMode for the text
	MagicPOVMode lPOVMode;
	MagicShifterImageText msMagicShakeText;
	
	uint32_t _currentMode = 0;

  public:

	void setText(const char *label) {
		// MSColor aRED = { 0xff, 0x00, 0x00 };
		MSColor aWhite = { 0xFF, 0xFF, 0xFF };
		Coordinate_s tPos;

		msMagicShakeText.resetTexts();

		tPos.x = 0;
		tPos.y = 0;
		msGlobals.ggtBitmap6x8.color = aWhite;
		msMagicShakeText.plotTextString((char *) label,
										msGlobals.ggtBitmap6x8, tPos);
		lPOVMode.setImage(&msMagicShakeText);
	}

	virtual void start() {
		setIndex(msGlobals.ggCurrentMode);

	}

// stop the mode
	virtual void stop(void) {
		lPOVMode.setImage(NULL);
	}

	void setIndex(uint32_t idx) {

		msSystem.slogln(" idx: " + String(idx));

		if (idx < 0) {
			idx = msGlobals.ggModeList.size() - 1;
		}
		if (idx >= msGlobals.ggModeList.size()) {
			idx = 0;
		}
		_currentMode = idx;

		setText(msGlobals.ggModeList[_currentMode]->modeName.c_str());
	}

// step through a frame of the mode 
	int select() {
		int _index = _currentMode;
		int _posIdx = _index;

		// button handling
		if (msSystem.msButtons.msBtnPwrHit) {
			// blink selected mode 4 times
			for (byte i = 0; i < 4; i++) {
				msSystem.msLEDs.setLED(_posIdx, 255, 255, 255,
									   msGlobals.ggBrightness);
				msSystem.msLEDs.updateLEDs();
				delay(50);
				msSystem.msLEDs.setLED(_posIdx, 0, 0, 0);
				msSystem.msLEDs.updateLEDs();
				delay(50);
			}
			return _index;
		}
		// cycle through the texts ..
		if (msSystem.msButtons.msBtnAHit) {
			msSystem.msButtons.msBtnAHit = false;	// !J! todo: button callbacks
			setIndex(_currentMode - 1);
		}
		// cycle through the texts ..
		if (msSystem.msButtons.msBtnBHit) {
			msSystem.msButtons.msBtnBHit = false;	// !J! todo: button callbacks
			setIndex(_currentMode + 1);
		}

		if (lPOVMode.step()) {
			return -1;
		} else {
			int selectedMode = 1 + (_index % 7);

			int r = (selectedMode >> 0) % 2 ? 255 : 0;
			int g = (selectedMode >> 1) % 2 ? 255 : 0;
			int b = (selectedMode >> 2) % 2 ? 255 : 0;

			for (byte j = 0; j < 16; j++) {
				if (j == _posIdx) {
					msSystem.msLEDs.setLED(j, r, g, b,
										   msGlobals.ggBrightness);
				} else {
					msSystem.msLEDs.setLED(j, 0, 0, 0, msGlobals.ggBrightness);
				}
			}

			msSystem.msLEDs.updateLEDs();
		}

		return -1;
	}
};
