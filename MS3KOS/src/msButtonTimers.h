//
// Due to hardware design, the MS3000 power button is best read
// on a timer, outside the scope of the rest of the system/OS/etc.
//
// Thus, we have a few things related to the read of the power
// button in this module.
//

// The timer is polled frequently:
os_timer_t aPowerButtonTimer;
// .. ~8 times/second
#define POWER_BUTTON_TIMER_PERIOD 125

// !J! TODO: Adjust this according to feel:
#define MIN_POWER_LEVEL_THRESHOLD (3.0f)

//
// The ESP8266 Timer-handler is plain 'ol C .. (in lieu of a class)
// 

//
// Calculate the current voltage of the power system.
// 
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

//
// Handle the regular polling of the power button hardware and
// make the results available to the rest of the system.
//
void PowerButtonTimerCallback(void *pArg) {
	os_intr_lock();
	// tickOccured = true;
	msGlobals.ggLastADValue = analogRead(A0);

	// !J! Not needed
	// if (calculateVoltage(msGlobals.ggLastADValue) <= MIN_POWER_LEVEL_THRESHOLD)
	// 	msGlobals.ggFault = FAULT_VERY_LOW_POWER;

	os_intr_unlock();
} // End of timerCallback

//
// Initialize the timers for use, and fire them off ..
//
void initMS3000SystemTimers()
{
	os_timer_setfn(&aPowerButtonTimer, PowerButtonTimerCallback, NULL);
	os_timer_arm(&aPowerButtonTimer, POWER_BUTTON_TIMER_PERIOD, true);
}

