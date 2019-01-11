// SystemText

#define NUM_POWER_MODES 6

	MSColor aRED = 		{ 0xff, 0x00, 0x00 };
	MSColor aGREEN = 	{ 0x00, 0xff, 0x00 };
	MSColor aBLUE = 	{ 0x00, 0x00, 0xff };
	MSColor aWHITE = 	{ 0xff, 0xff, 0xff };

class SystemTextMode:public MagicShifterBaseMode {

  private:

	MS3KG_App_System &_system = msGlobals.pbuf.apps.system;

  	MagicPOVMode lPOVMode;
	MagicShifterImageText msMagicShakeText;

	bool correctBrightness = false;
	bool shouldBlackenBackground = true;
	int ptLoMode = 0;	// index
	int ptHiMode = 0;
	bool needsTextUpdate = false;

	struct powerMode_t
	{
		String label;
		unsigned long powertime;
	} ;


	powerMode_t predefinedPowerModes[NUM_POWER_MODES] = {
		{"Infi",0},
		{"5m", 5 * 60 * 1000} ,
		{"10m",10 * 60 * 1000},
		{"30m",30 * 60 * 1000},
		{"1hr",60 * 60 * 1000},
		{"2hr",2 * 60 * 60 * 1000},
	};

  public:
  	SystemTextMode() { 
		 	modeName = "Values";
	}

	void setText(char *label, char *value, MSColor color, char *value2, MSColor color2) {
	
		Coordinate_s tPos;

		msMagicShakeText.resetTexts();

		tPos.x = 0;
		tPos.y = 0;
		msGlobals.ggtBitmap4x5.color = aBLUE;
		msMagicShakeText.plotTextString(label, msGlobals.ggtBitmap4x5, tPos);

		tPos.y += msGlobals.ggtBitmap4x5.header.frameHeight;
		msGlobals.ggtBitmap4x5.color = color;

		msMagicShakeText.plotTextString(value, msGlobals.ggtBitmap4x5, tPos);

		if (value2 != NULL) {
			tPos.y += msGlobals.ggtBitmap4x5.header.frameHeight;
			msGlobals.ggtBitmap4x5.color = color2;

			msMagicShakeText.plotTextString(value2, msGlobals.ggtBitmap4x5, tPos);
		}

		lPOVMode.setImage(&msMagicShakeText);


	}

	void setLargeText(char *label, char *value, MSColor color) {

		Coordinate_s tPos;

		msMagicShakeText.resetTexts();

		tPos.x = 0;
		tPos.y = 0;
		msGlobals.ggtBitmap4x5.color = aBLUE;
		msMagicShakeText.plotTextString(label, msGlobals.ggtBitmap4x5, tPos);

		tPos.y += msGlobals.ggtBitmap4x5.header.frameHeight;
		msGlobals.ggtBitmap6x8.color = color;

		msMagicShakeText.plotTextString(value, msGlobals.ggtBitmap6x8, tPos);

		lPOVMode.setImage(&msMagicShakeText);

	}

	void setText(char *label, char *value, MSColor color) {

		setText(label, value, color, NULL, aWHITE);
	}

	void start() {
		setText((char *) "SYSTEM", (char *) "VALUES", aBLUE);

		for (int i=0;i<NUM_POWER_MODES;i++){
			if (predefinedPowerModes[i].powertime <= msGlobals.ui.timeoutLowPower)
				ptLoMode = i;
			if (predefinedPowerModes[i].powertime <= msGlobals.ui.timeoutHighPower)
				ptHiMode = i;
		}

		lPOVMode.setFrameMultiplier(2);

	}

// stop the mode
	void stop(void) {
		lPOVMode.setImage(NULL);
	}


#define NUM_SYS_TEXTS 9

// step through a frame of the mode 
	bool step() {
		
		bool isConnectedToAP = (WiFi.status() == WL_CONNECTED);
		bool isBatteryLow = msSystem.lowBatteryMillis != 0;
		int new_mode = _system.mode;

		// cycle through the texts ..
		if (msSystem.msButtons.msBtnALongHit) {
			msSystem.msButtons.msBtnALongHit = false;
			msSystem.showBatteryStatus(true);
		}

		if (msSystem.msButtons.msBtnBLongHit) {
			msSystem.msButtons.msBtnBLongHit = false;
			if (new_mode == MS3KG_App_System_Mode_CALIBRATION)
				msSystem.powerCalibrate();
		}

		if (msSystem.msButtons.msBtnAHit) {
			msSystem.msButtons.msBtnAHit = false;	// !J! todo: button callbacks
			
			new_mode++;

			if (new_mode >= _MS3KG_App_System_Mode_MAX)
				new_mode = _MS3KG_App_System_Mode_MIN;


			needsTextUpdate = true;
		}

		if (msSystem.msButtons.msBtnBHit) {
			msSystem.msButtons.msBtnBHit = false;	// !J! todo: button callbacks
			
			new_mode--;
			
			if (new_mode < _MS3KG_App_System_Mode_MIN)
				new_mode = _MS3KG_App_System_Mode_MAX;

			needsTextUpdate = true;
		}

		if (msSystem.msButtons.msBtnPwrHit) {
			msSystem.slogln("btpwr");
			msSystem.msButtons.msBtnPwrHit = false;
			if (new_mode == MS3KG_App_System_Mode_WIFI) { // WIFI
				msSystem.slogln("wifi cursor!");
				if (msGlobals.ggEnableWIFI)  {
					WiFi.disconnect(true);
					msGlobals.ggEnableWIFI = false;
					msSystem.slogln("disconnected");
				}
				else {
					msSystem.slogln("autoconnect");
					msGlobals.ggEnableWIFI = true;
					AutoConnect();
				}
			}
			if (new_mode == MS3KG_App_System_Mode_POWER_LO) {
				// power-timeout modes
				ptLoMode++;
				if(ptLoMode >= NUM_POWER_MODES)
					ptLoMode=0;

				msGlobals.ui.timeoutLowPower = predefinedPowerModes[ptLoMode].powertime;
				msSystem.Settings.setUIConfig(&msGlobals.ui);
			}
			if (new_mode == MS3KG_App_System_Mode_POWER_HI) {
				// power-timeout modes
				ptHiMode++;
				if(ptHiMode >= NUM_POWER_MODES)
					ptHiMode=0;

				msGlobals.ui.timeoutHighPower = predefinedPowerModes[ptHiMode].powertime;
				msSystem.Settings.setUIConfig(&msGlobals.ui);
			}


			needsTextUpdate = true;
		}

		if (needsTextUpdate) {

			if (new_mode == MS3KG_App_System_Mode_VERSION) {
				setText((char *)String("VERSION").c_str(),
						(char *)MS3KOS_VERSION, aWHITE);
			}
			if (new_mode == MS3KG_App_System_Mode_IP) {
				setText((char *) String("IP").c_str(),
						(char *) WiFi.localIP().toString().c_str(), aWHITE);
			} else if (new_mode == MS3KG_App_System_Mode_SOFTIP) {
				setText((char *) String("SOFTIP").c_str(),
						(char *) WiFi.softAPIP().toString().c_str(), aWHITE);
			} else if (new_mode == MS3KG_App_System_Mode_SSID) {
				if (isConnectedToAP) {
					setText((char *) String("SSID").c_str(),
							(char *) WiFi.SSID().c_str(), aWHITE);
				}
				else {
					setText((char *) String("SSID").c_str(),
							(char *)msGlobals.ggSoftAP.auth.ssid , aWHITE);
				}
			} else if (new_mode == MS3KG_App_System_Mode_POWER_LO) {
				setText((char *) String("P-LO").c_str(),
						(char *)predefinedPowerModes[ptLoMode].label.c_str(), aWHITE);
			} else if (new_mode == MS3KG_App_System_Mode_POWER_HI) {
				setText((char *) String("P-HI").c_str(),
						(char *)predefinedPowerModes[ptHiMode].label.c_str(), aWHITE);
			} else if (new_mode == MS3KG_App_System_Mode_POWER_VALUE) {
				setText((char *) String("VOLT").c_str(),
						(char *)String(msSystem.batteryVoltage).c_str(), isBatteryLow ? aRED : aGREEN);
			} else if (new_mode == MS3KG_App_System_Mode_CALIBRATION) {
				setText((char *) String("CALIB").c_str(),
						(char *)String(msGlobals.batVoltCalibration).c_str(), isBatteryLow ? aRED : aGREEN);
			} else if (new_mode == MS3KG_App_System_Mode_WIFI) {
				if  (!msGlobals.ggEnableWIFI)
					setText((char *) String("WIFI").c_str(), (char *) String("OFF").c_str(), aRED);
				else
					setText((char *) String("WIFI").c_str(), (char *) String("ON").c_str(), aGREEN);

				Coordinate_s tPos;
				tPos.x = 15;
				tPos.y = 0;
				tPos.y += msGlobals.ggtBitmap4x5.header.frameHeight;

				if (!isConnectedToAP)
				{
					msGlobals.ggtBitmap4x5.color = aRED;
					msMagicShakeText.plotTextString((char *)"N", msGlobals.ggtBitmap4x5, tPos);
				}
				else
				{	
					msGlobals.ggtBitmap4x5.color = aGREEN;
					msMagicShakeText.plotTextString((char *)"Y", msGlobals.ggtBitmap4x5, tPos);
				}
				lPOVMode.setImage(&msMagicShakeText);
			}
		}


		if (_system.mode != new_mode)
			_system.mode = (MS3KG_App_System_Mode)new_mode;

		if (lPOVMode.step()) {
			return true;
		} else {	
			// !J! : note - debug code to indicate values
			// msSystem.slog("bV:"); msSystem.slogln(String(msSystem.batteryVoltage));
			msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);

			if (isBatteryLow){ 
				msSystem.msLEDs.setLED(0, 255, 0, 0, msGlobals.ggBrightness);
			}
			else {
				msSystem.msLEDs.setLED(0, 0, 255, 0, msGlobals.ggBrightness);
			}

			if  (msGlobals.ggEnableWIFI) {
				msSystem.msLEDs.setLED(1, 0, 255, 0, msGlobals.ggBrightness);
			}
			else {
				msSystem.msLEDs.setLED(1, 255, 0, 0, msGlobals.ggBrightness);
			}

			if  (isConnectedToAP) {
				msSystem.msLEDs.setLED(2, 0, 255, 0, msGlobals.ggBrightness);
			}
			else {
				msSystem.msLEDs.setLED(2, 255, 0, 0, msGlobals.ggBrightness);
			}

			msSystem.msLEDs.updateLEDs();

			needsTextUpdate = true;

			delay(30);

			return false;
		}

		return false;
	}
};
