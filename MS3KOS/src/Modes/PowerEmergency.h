//
// PowerEmergencyMode: 	what do, when power low?
//
// Story: A Power Emergency has occured, because Threshold X has been reached
// 		  We will: a) Live for 60 seconds only, and then power off
// 				   b) Check for any button press: if true, poweroff
//				   c) Blink a single LED on and off at 125 intervals
//                 d) if the cable is plugged in, abort the 60-second countdown and
//					  continue to normal operation
//

#define POWERDOWN_LIVE_TIME (60 * 1000)
#define BLINK_PERIOD (125)

class PowerEmergencyMode : public MagicShifterBaseMode {

public:
	String modeName = "pwr!";

private:

	unsigned int modeStartTime = 0;
	unsigned int blinkedAt = 0;

public:
	PowerEmergencyMode() {
		modeName = "pwr!";
	};

	virtual void start() {
		modeStartTime = msGlobals.ggCurrentMillis;
		blinkedAt = modeStartTime;
	};

	// step through a frame of the mode 
	virtual bool step() {
		// a) live for 60 seconds, then power off
		static int LEDidx = 0;

		if (modeStartTime == 0) {
			start();
		}
		// todo: d)
		// if calculateVoltage(int adValue)
			// blahblah();

		// power down if we have been up for too long (a), or if the user presses a button (b) ..
		if ( msGlobals.ggCurrentMillis >= (modeStartTime + POWERDOWN_LIVE_TIME) ||
 			 (msSystem.msButtons.msBtnAHit) || (msSystem.msButtons.msBtnBHit) || (msSystem.msButtons.msBtnPwrHit) ) {
			msSystem.powerDown();
		}

		// blink the LED (c)
		if (msGlobals.ggCurrentMillis < (blinkedAt + BLINK_PERIOD)) {
			msSystem.msLEDs.setLED(LEDidx, (LEDidx & 1) ? 255 : 0, 0, 0, msGlobals.ggBrightness);
		}
		else {
			msSystem.msLEDs.setLED(LEDidx, 0, 0, 0);
			LEDidx++;
			LEDidx %= MAX_LEDS;
			blinkedAt = msGlobals.ggCurrentMillis;
			msSystem.slog(String(modeStartTime)); msSystem.slog(":"); msSystem.slogln(String(blinkedAt));
		}

		msSystem.msLEDs.updateLEDs();


	};

};
