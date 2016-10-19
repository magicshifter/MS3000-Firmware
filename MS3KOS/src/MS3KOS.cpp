/*
 * MagicShifter3000 OS, Copyright (c) wizards@Work 2016
 * Authors: wizard23(pt), seclorum(jv), jaeh(je)
 * Notes: 
 *          All code is conflated by headers (.h):w
 *          Code prefix- is ms*, as in msConfig, msSystem, etc.
 *
 * The purpose of this code is to provide the MS3000 user with 
 * a unique and engaging user interface, with the available LED's 
 * and button configurations, as well as additional Web-based UI
 * for configuration and interaction.
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
#include <vector>
#include <map> 

#include <FS.h>

// note: local configuration, globals, and system objects get created now.
#include "msConfig.h"

#include "msGlobals.h"
MagicShifterGlobals msGlobals;

#include "msSystem.h"
MagicShifterSystem msSystem;
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
	// msGlobals.ggCurrentMode = 6;
	msGlobals.ggModeList.push_back(&msMIDIArpeggiator);
#endif

	msGlobals.ggModeList[msGlobals.ggCurrentMode]->start();


msSystem.slog("Mode Size:"); msSystem.slogln(String(msGlobals.ggModeList.size()));

	msModeSelector.start();
}


//
// main OS loop - despatch steps for each of:
//	system
//  webserver
//  check for menu-mode
//  check for faults

void loop()
{
	// let WebServer process events.
	msWebServer.step();

	// let SystemUI process events.
	msSystem.step();

	// if SystemUI events trigger it (UI), display the mode-selector menu:
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
		// despatch to current mode
		msGlobals.ggModeList[msGlobals.ggCurrentMode]->step();
	}

	// fault-checks
	if (msGlobals.ggFault > 0) {
		Serial.print("FAULT:");
		Serial.println(String(msGlobals.ggFault));
		switch (msGlobals.ggFault) {
			case FAULT_VERY_LOW_POWER:
				msPowerEmergencyMode.start();
			break;
			case FAULT_NO_ACCELEROMETER:
				msSystem.msLEDs.errorSwipe();
			break;
			default: {};
		}
	}
}
