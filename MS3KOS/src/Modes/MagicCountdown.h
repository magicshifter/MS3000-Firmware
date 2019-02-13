
#define COUNTDOWN_UPDATE_PERIOD 125

class MagicCountdownMode : public MagicShifterBaseMode {
private:
	MS3KG_App_Countdown &_countdown = msGlobals.pbuf.apps.countdown;


public:
	static int _ticks;

	MagicCountdownMode() {
		modeName = "countR";
	}


	os_timer_t osTimer;

	static void tickCallback(void *pArg) {
		os_intr_lock();
		_ticks+=COUNTDOWN_UPDATE_PERIOD;
		os_intr_unlock();
	} // En


// !J! TODO: Finish Countdown Timer
	void start() {

		os_timer_setfn(&osTimer, tickCallback, &_ticks);
		os_timer_arm(&osTimer, COUNTDOWN_UPDATE_PERIOD, true);

		for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
			msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
			msGlobals.ggRGBLEDBuf[i+1] = i * 4;
			msGlobals.ggRGBLEDBuf[i+2] = 255 - i * 16;
			msGlobals.ggRGBLEDBuf[i+3] = 0;
		}
	}

	void stop(void) {
		os_timer_disarm(&osTimer);
	}

	bool step(void) {
		int lp = _ticks % COUNTDOWN_UPDATE_PERIOD;
		msGlobals.ggRGBLEDBuf[lp] = 0;
		Serial.print(" lp:" + String(lp));
		Serial.println(" _ticks:" + String(_ticks));
		msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
		msSystem.msLEDs.updateLEDs();
		delay(10);

		return true;
	}
};

