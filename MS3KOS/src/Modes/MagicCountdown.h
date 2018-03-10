
#define COUNTDOWN_UPDATE_PERIOD 125

class MagicCountdownMode : public MagicShifterBaseMode {
private:
	static int countdownTicks;

public:
	MagicCountdownMode() {
		modeName = "countR";
	}

	os_timer_t aCountdownTimer;
	static void countdownTimerCallback(void *pArg) {
		os_intr_lock();
		countdownTicks+=COUNTDOWN_UPDATE_PERIOD;
		os_intr_unlock();
	} // En

	void start() {
		os_timer_setfn(&aCountdownTimer, countdownTimerCallback, &countdownTicks);
		os_timer_arm(&aCountdownTimer, COUNTDOWN_UPDATE_PERIOD, true);

		for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
			msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
			msGlobals.ggRGBLEDBuf[i+1] = i * 4;
			msGlobals.ggRGBLEDBuf[i+2] = 255 - i * 16;
			msGlobals.ggRGBLEDBuf[i+3] = 0;
		}
	}

	void stop(void) {
		os_timer_disarm(&aCountdownTimer);
	}

	bool step(void) {
		int lp = countdownTicks % COUNTDOWN_UPDATE_PERIOD;
		msGlobals.ggRGBLEDBuf[lp] = 0;
		Serial.print(" lp:");
		Serial.print(lp);
		msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
		msSystem.msLEDs.updateLEDs();
		delay(10);
	}
};

