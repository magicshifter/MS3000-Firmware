//
// OTA Updater Mode
//


class MagicUpdateMode : public MagicShifterBaseMode {

public:

	MagicUpdateMode() {
		modeName = "Update";
	}

	virtual void start() {
		static int xx = 0;

		// set a basic background to start with ..
		for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
			msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
			msGlobals.ggRGBLEDBuf[i+1] = i * 2;
			msGlobals.ggRGBLEDBuf[i+2] = 255 - i * 2;
			msGlobals.ggRGBLEDBuf[i+3] = 128;
		}

		msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
		msSystem.msLEDs.updateLEDs();


		ArduinoOTA.onStart([]() { 
			// set LED's to start-state:
			msSystem.slog(" update: start ");
			msSystem.msLEDs.fillLEDs(0, 64, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
		});

		ArduinoOTA.onProgress([](int total, int size) {

			int percent = total / size;

			msSystem.msLEDs.fillLEDs(0, 64, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
		});

		ArduinoOTA.onEnd([]() { 
			// set LED's to end-state:
			msSystem.slog(" update: end");
			msSystem.msLEDs.fillLEDs(128, 32, 64, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
		});

		ArduinoOTA.onError([](ota_error_t error) { 
			msSystem.slog(" update: error");
			msSystem.msLEDs.fillLEDs(255, 0, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
		});


		ArduinoOTA.begin();

		msSystem.slog(" update: start (mode) ");

	}

	virtual void stop(void) {
		msSystem.slog(" update: stop (mode)");

		// set a basic background to start with ..
		for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
			msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
			msGlobals.ggRGBLEDBuf[i+1] = i * 2;
			msGlobals.ggRGBLEDBuf[i+2] = 255 - i * 2;
			msGlobals.ggRGBLEDBuf[i+3] = 128;
		}
	}

	virtual bool step(void) {
		// msSystem.slog(" update: step ");

		ArduinoOTA.handle();

		msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
		msSystem.msLEDs.updateLEDs();
		
		delay(10);

		return true;
	}

};