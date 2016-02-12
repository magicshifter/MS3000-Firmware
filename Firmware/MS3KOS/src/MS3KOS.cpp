/*
 * MagicShifter3000 OS, Copyright (c) wizards@Work
 * Authors: wizard23(pt), seclorum(jv)
 * Notes: 
 *          All code is conflated by headers (.h):w
 *          Code prefix- is ms*, as in msConfig, msSystem, etc.
 */

extern "C" {
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <math.h>				/* for HUGE_VAL */
#include <json/json.h>
#include <json/jsonparse.h>
#include <json/jsontree.h>
}
//compiler.c.elf.libs=-lm -lgcc -lhal -lphy -lnet80211 -llwip -lwpa -lmain -lpp -lsmartconfig -lc -ljson
#include <FS.h>
// note: local configuration, globals, and system objects get created now.
#include "msConfig.h"

#include "msGlobals.h"
MagicShifterGlobals msGlobals;
// note: beyond this point, please consider the above globals.

#include "msSystem.h"
MagicShifterSystem msSystem;
// note: WebServer and msSystem are in love
#include "WebServer/WebServer.h"
MagicShifterWebServer msWebServer;

// MIDI can be configured on or off 
#ifdef CONFIG_ENABLE_MIDI
#include "MIDI/MidiShifter.h"
#endif

// GUI modes, well actually .. modes are more of an 'app' ..
#include "Modes/Modes.h"

// Begin MagicShifter3000 operation
void setup()
{
	// record our bootup time
	msGlobals.ggBootTime = millis();

	// start the system
	msSystem.setup();

	// get the web interface started
	msWebServer.start();

	ServerConfig sysLogInfo;
	// set up syslog ..
	if (Settings.getSyslogConfig(&sysLogInfo))
		msSystem.msSysLog.setup(sysLogInfo.hostname);

	msSystem.msSysLog.
		sendSysLogMsg(String("LOG WAS ENABLED.................."));

	msSystem.showBatteryStatus(true);

	// initialize the modules ..
	msMagicShake.start();
	msSysText.start();
	msMagicMagnet.start();
	msModeSelector.start();

}

void loop()
{

	// inside time-frame
	if (msGlobals.ggLastFrameMicros + msGlobals.ggSpeedMicros < micros()) {

		msSystem.loop();

		msWebServer.loop();

		msGlobals.ggLFrameTime =
			msGlobals.ggCurrentMicros - msGlobals.ggLastFrameMicros;
		msGlobals.ggCurrentFrame++;
		msGlobals.ggLastFrameMicros = msGlobals.ggCurrentMicros;

		if (msSystem.modeMenuActivated) {
			int newMode = msModeSelector.step();
			if (newMode >= 0) {
				msSystem.modeMenuActivated = false;
				msGlobals.ggCurrentMode = newMode;
			}
		} else {
			// dispatch to the mode handler .. 
			if (msGlobals.ggCurrentMode == 4) {	// remote-light
				msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
				msSystem.msLEDs.updateLEDs();
				delay(10);
			} else if (msGlobals.ggCurrentMode == 3) {	// system values
				msSysText.step();
			} else if (msGlobals.ggCurrentMode == 2) {	// compass
				msMagicMagnet.step();
			} else if (msGlobals.ggCurrentMode == 1) {	// flashlight / disco!
				msMagicLight.step();
			} else if (msGlobals.ggCurrentMode == 0) { // magic shake
				msMagicShake.step();
			}
		}

		// fault-checks
		if (msGlobals.ggFault > 0) {
			Serial.print("FAULT:");
			Serial.println(String(msGlobals.ggFault));
			msSystem.msLEDs.errorSwipe();
		}
	}
}
