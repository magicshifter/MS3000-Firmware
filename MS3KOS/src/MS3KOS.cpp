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
	if (msSystem.Settings.getSyslogConfig(&sysLogInfo))
		msSystem.msSysLog.setup(sysLogInfo.hostname);

	msSystem.msSysLog.
		sendSysLogMsg(String("LOG WAS ENABLED.................."));

	msSystem.showBatteryStatus(true);

	// configure all modes available in the main menu
	msGlobals.ggModeList.push_back(&msMagicShake);
	msGlobals.ggModeList.push_back(&msMagicLight);
	msGlobals.ggModeList.push_back(&msMagicMagnet);
	msGlobals.ggModeList.push_back(&msSysText);
	msGlobals.ggModeList.push_back(&msMagicRemote);
	msGlobals.ggModeList.push_back(&msMagicBeat);

#ifdef CONFIG_ENABLE_MIDI
	msGlobals.ggModeList.push_back(&msMIDIShifter);
#endif

	msGlobals.ggModeList[msGlobals.ggCurrentMode]->start();

	msModeSelector.start();
}

void loop()
{
	// inside time-frame
	msWebServer.step();

	msSystem.step();

	msGlobals.ggLFrameTime =
		msGlobals.ggCurrentMicros - msGlobals.ggLastFrameMicros;
	msGlobals.ggCurrentFrame++;
	msGlobals.ggLastFrameMicros = msGlobals.ggCurrentMicros;

	if (msSystem.modeMenuActivated) {
		int newMode = msModeSelector.select();
		if (newMode >= 0) {
			// stop all modes..
			msGlobals.ggModeList[msGlobals.ggCurrentMode]->stop();
			msSystem.modeMenuActivated = false;
			msGlobals.ggCurrentMode = newMode;
			msGlobals.ggModeList[msGlobals.ggCurrentMode]->start();
		}
	} else {
		if (msGlobals.ggCurrentMode < msGlobals.ggModeList.size()) {
			// despatch to mode

			msSystem.setLocalYieldState(
				!msGlobals.ggModeList[msGlobals.ggCurrentMode]->step()
				);
		}
	}

	// fault-checks
	if (msGlobals.ggFault > 0) {
		Serial.print("FAULT:");
		Serial.println(String(msGlobals.ggFault));
		msSystem.msLEDs.errorSwipe();
	}
}
