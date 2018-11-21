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



		ArduinoOTA.onStart([]() { 
			// set LED's to start-state:
			msSystem.slog(" update: start ");

			// set a basic background to start with ..
			for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
				msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
				msGlobals.ggRGBLEDBuf[i+1] = i * 2;
				msGlobals.ggRGBLEDBuf[i+2] = 32 - i * 2;
				msGlobals.ggRGBLEDBuf[i+3] = 32;
			}
	
			msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
			msSystem.msLEDs.updateLEDs();

		});

		ArduinoOTA.onProgress([](int total, int size) {
			msSystem.slog(" update: .. ");
			// !J! TODO: render progress
			msSystem.msLEDs.setLED((xx + 0 * 3) & 0xF, 255, 0, 255, msGlobals.ggBrightness);

			msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
			msSystem.msLEDs.updateLEDs();

		});

		ArduinoOTA.onEnd([]() { 
			// set LED's to end-state:
			msSystem.slog(" update: end");
			msSystem.msLEDs.setLED((xx + 0 * 3) & 0xF, 255, 255, 255, msGlobals.ggBrightness);
			msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
			msSystem.msLEDs.updateLEDs();

		});

		ArduinoOTA.onError([](ota_error_t error) { 
			msSystem.slog(" update: error");

			// set LED's to error-state:
			for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
				msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
				msGlobals.ggRGBLEDBuf[i+1] = 255;
				msGlobals.ggRGBLEDBuf[i+2] = 0;
				msGlobals.ggRGBLEDBuf[i+3] = 0;
			}
			msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
			msSystem.msLEDs.updateLEDs();

		});

		ArduinoOTA.begin();

		// set a basic background to start with ..
		for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
			msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
			msGlobals.ggRGBLEDBuf[i+1] = i * 2;
			msGlobals.ggRGBLEDBuf[i+2] = 255 - i * 2;
			msGlobals.ggRGBLEDBuf[i+3] = 128;
		}

		msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
		msSystem.msLEDs.updateLEDs();

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