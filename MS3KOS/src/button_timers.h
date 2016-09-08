
os_timer_t aPowerButtonTimer;
#define POWER_BUTTON_TIMER_PERIOD 125

// !J! TODO: Adjust this:
#define MIN_POWER_LEVEL_THRESHOLD (3.0f)

float calculateVoltage(int adValue, int batVoltCalibration)
{
	float voltage;
	
	if (batVoltCalibration == 0) {
		int ad1V = 1024;
		float r1 = 220, r2 = 820, r3 = 0;	// alpha
		voltage = ((float) (r1 + r2 + r3) * adValue) / (r1 * ad1V);
	} else {
		voltage = (4.2 * adValue) / batVoltCalibration; 
	}

	return voltage;
}

void PowerButtonTimerCallback(void *pArg) {
	os_intr_lock();
	// tickOccured = true;
	msGlobals.ggLastADValue = analogRead(A0);

	// !J! Not needed
	// if (calculateVoltage(msGlobals.ggLastADValue) <= MIN_POWER_LEVEL_THRESHOLD)
	// 	msGlobals.ggFault = FAULT_VERY_LOW_POWER;

	os_intr_unlock();
} // End of timerCallback

void initPowerButtonTimer()
{
	os_timer_setfn(&aPowerButtonTimer, PowerButtonTimerCallback, NULL);
	os_timer_arm(&aPowerButtonTimer, POWER_BUTTON_TIMER_PERIOD, true);
}

