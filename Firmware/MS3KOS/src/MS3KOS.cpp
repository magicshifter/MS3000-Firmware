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

	delay(350);					// debug !J!

	// start the system
	msSystem.setup();

	// get the web interface started
	msWebServer.StartWebServer();

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

		msGlobals.ggLastMicros = msGlobals.ggCurrentMicros;
		msGlobals.ggCurrentMicros = micros();

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
			if (msGlobals.ggCurrentMode == 0) {
				if (msGlobals.ggFault == FAULT_NEW_FILEUPLOAD) {
					msGlobals.ggFault = 0;
					msMagicShake.reset();
				}

				msMagicShake.step();
			} else if (msGlobals.ggCurrentMode == 1) {

				msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
				msSystem.msLEDs.updateLEDs();
				delay(10);

				// W: hijacked mode 1 for video
				//   static int xx = 0;
				//   int gs = msGlobals.ggBrightness;

				// msSystem.msLEDs.fillLEDs(0, 0, 0);
				// msSystem.msLEDs.setLED((xx + 0 * 3) & 0xF, 255, 0, 0, gs);

				// msSystem.msLEDs.setLED((xx + 1 * 3) & 0xF, 255, 255, 0, gs);
				// msSystem.msLEDs.setLED((xx + 2 * 3) & 0xF, 0, 255, 0, gs);

				// msSystem.msLEDs.setLED((xx + 3 * 3) & 0xF, 0, 255, 255, gs);
				// msSystem.msLEDs.setLED((xx + 4 * 3) & 0xF, 0, 0, 255, gs);

				// msSystem.msLEDs.updateLEDs();

				// xx++;
				//   delay(350);
			} else if (msGlobals.ggCurrentMode == 2) {
				// swipe colors
				for (byte idx = 0; idx < MAX_LEDS; idx++) {
					if (idx % 8 == 0) {
						msSystem.msLEDs.setLED(idx, 255,
											   (idx & 2) ? 255 : 0,
											   (idx & 4) ? 255 : 0, 1);
					} else
						msSystem.msLEDs.setLED(idx, (idx & 1) ? 255 : 0,
											   (idx & 2) ? 255 : 0,
											   (idx & 4) ? 255 : 0, 1);
					// super-bright
					// msSystem.msLEDs.setLED(idx, 255,255,255,20);

					msSystem.msLEDs.updateLEDs();
					delay(10);
				}
			} else if (msGlobals.ggCurrentMode == 3) {
				msSysText.step();
			} else if (msGlobals.ggCurrentMode == 4) {
				msMagicMagnet.step();
			} else if (msGlobals.ggCurrentMode == 5) {
				msMagicLight.step();
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
