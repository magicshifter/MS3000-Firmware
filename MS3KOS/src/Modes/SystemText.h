	MSColor aRED = 		{ 0xff, 0x00, 0x00 };
	MSColor aGREEN = 	{ 0x00, 0xff, 0x00 };
	MSColor aBLUE = 	{ 0x00, 0x00, 0xff };
	MSColor aWHITE = 	{ 0xff, 0xff, 0xff };


class SystemTextMode:public MagicShifterBaseMode {

  private:
  	MagicPOVMode lPOVMode;
	MagicShifterImageText msMagicShakeText;

	bool correctBrightness = false;
	int sysCursor = 0;
	bool shouldBlackenBackground = true;

  public:
	const char *modeName = "SystemText";


	void setText(char *label, char *value, MSColor color) {
		

		Coordinate_s tPos;

		msMagicShakeText.resetTexts();

		tPos.x = 0;
		tPos.y = 0;
		msGlobals.tBitmap4x5.color = aBLUE;
		msMagicShakeText.plotTextString(label, msGlobals.tBitmap4x5, tPos);

		tPos.y += msGlobals.tBitmap4x5.header.frameHeight;
		msGlobals.tBitmap4x5.color = color;

		msMagicShakeText.plotTextString(value, msGlobals.tBitmap4x5, tPos);
		lPOVMode.setImage(&msMagicShakeText);
	}

	void start() {
		setText((char *) "SYSTEM", (char *) "VALUES", aBLUE);
	}

// stop the mode
	void stop(void) {
		lPOVMode.setImage(NULL);
	}

	enum {
		STR_IP = 0,
		STR_SOFTIP = 1,
		STR_SSID = 2,
		STR_WIFI = 3,
		STR_VERSION = 4
	};

// step through a frame of the mode 
	bool step() {
		bool needsTextUpdate = false;

		// cycle through the texts ..
		if (msSystem.msButtons.msBtnAHit) {
			msSystem.msButtons.msBtnAHit = false;	// !J! todo: button callbacks
			sysCursor++;
			if (sysCursor > 4)
				sysCursor = 0;
			msSystem.slog("cursor:");
			msSystem.slogln(String(sysCursor));
			needsTextUpdate = true;
		}
		if (msSystem.msButtons.msBtnBHit) {
			msSystem.msButtons.msBtnBHit = false;	// !J! todo: button callbacks
			sysCursor--;
			if (sysCursor < 0)
				sysCursor = 4;
			msSystem.slog("cursor:");
			msSystem.slogln(String(sysCursor));
			needsTextUpdate = true;
		}
		if (msSystem.msButtons.msBtnPwrHit) {
			msSystem.slogln("btpwr");
			if (sysCursor == STR_WIFI) { // WIFI
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
				msSystem.msButtons.msBtnPwrHit = false;
			}
			needsTextUpdate = true;
		}

		if (needsTextUpdate) {
			if (sysCursor == STR_VERSION) {
				setText((char *)String("VERSION").c_str(),
						(char *)MS3KOS_VERSION, aWHITE);
			}
			if (sysCursor == STR_IP) {
				setText((char *) String("IP").c_str(),
						(char *) WiFi.localIP().toString().c_str(), aWHITE);
			} else if (sysCursor == STR_SOFTIP) {
				setText((char *) String("SOFTIP").c_str(),
						(char *) WiFi.softAPIP().toString().c_str(), aWHITE);
			} else if (sysCursor == STR_SSID) {
				setText((char *) String("SSID").c_str(),
						(char *) WiFi.SSID().c_str(), aWHITE);
			} else if (sysCursor == STR_WIFI) {
				if  (!msGlobals.ggEnableWIFI)
					setText((char *) String("WIFI").c_str(), (char *) String("OFF").c_str(), aRED);
				else
					setText((char *) String("WIFI").c_str(), (char *) String("ON").c_str(), aGREEN);

				Coordinate_s tPos;
				tPos.x = 15;
				tPos.y = 0;
				tPos.y += msGlobals.tBitmap4x5.header.frameHeight;
				if  (WiFi.status() != WL_CONNECTED)
				{
					msGlobals.tBitmap4x5.color = aRED;
					msMagicShakeText.plotTextString((char *)"N", msGlobals.tBitmap4x5, tPos);
				}
				else
				{	
					msGlobals.tBitmap4x5.color = aGREEN;
					msMagicShakeText.plotTextString((char *)"Y", msGlobals.tBitmap4x5, tPos);
				}
				lPOVMode.setImage(&msMagicShakeText);
			}
		}

		if (lPOVMode.step()) {
			return true;
		} else {	
			delay(30);
			return false;
		}

		return false;
	}
};
