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

// globals are available for all system methods, modules, etc.
#include "msGlobals.h"
MagicShifterGlobals msGlobals;

// the system UI module
#include "msSystem.h"
MagicShifterSystem msSystem;

// the system Web Server module
#include "WebServer/WebServer.h"
MagicShifterWebServer msWebServer;

// Onboard GUI modes, per configuration
#include "Modes/Modes.h"


// Detect system-faults for factory testing and otherwise
// A low-level system fault can either be:
//  a) a low-power emergency
//  b) something faulty with an onboard sensor device
//
void systemFault(int aFault)
{
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


// Begin MagicShifter3000 operation
//  initializes the system, web server, configuration, syslog,
//  and populates the global mode list depending on configuration
void setup()
{
	// record our bootup time from the beginning
	msGlobals.ggBootTime = millis();

	// start the system UI and event handlers
	msSystem.setup();

	// get the web interface started
	msWebServer.start();

	// set up syslog .. we can use UDP syslog if necessary, so that the Serial
	// ports can be available for other things, e.g. MIDI
	ServerConfig sysLogInfo;
	if (msSystem.Settings.getSyslogConfig(&sysLogInfo)) {
		msSystem.msSysLog.setup(sysLogInfo.hostname);
		msSystem.msSysLog.sendSysLogMsg(String("sysLog functionality is enabled."));
	}

	// configure all modes available in the main menu
	msGlobals.ggModeList.push_back(&msMagicShake);
	msGlobals.ggModeList.push_back(&msMagicLight);
	msGlobals.ggModeList.push_back(&msMagicMagnet);
	msGlobals.ggModeList.push_back(&msSysText);
	msGlobals.ggModeList.push_back(&msMagicRemote);
	msGlobals.ggModeList.push_back(&msMagicBeat);
	msGlobals.ggModeList.push_back(&msMagicCountdown);

	WiFi.hostname(msSystem.Settings.getAPNameOrUnique().c_str());
	ArduinoOTA.setHostname(msSystem.Settings.getAPNameOrUnique().c_str());

	msSystem.slogln("wifi: hostname is:" + WiFi.hostname());
	msSystem.slogln("wifi: OTA hostname is:" + ArduinoOTA.getHostname());

	// if MIDI has been configured, enable the additional MIDI mode(s)
#ifdef CONFIG_ENABLE_MIDI
	
	msGlobals.ggModeList.push_back(&msMIDIArpeggiator);
	msGlobals.ggModeList.push_back(&msMIDISequencer);

#ifdef CONFIG_ENABLE_MIDI_SERIAL
	// The MIDI byte parser, provided by the miby module ..

	SERIAL_MIDI_init();
	
	msGlobals.ggCurrentMode = 7;

#endif

#ifdef CONFIG_ENABLE_MIDI_RTP
	setupRTPDebugHandlers();
	AppleMIDI.begin(msSystem.Settings.getAPNameOrUnique().c_str());
	msSystem.slog("MIDI(rtp) session started, identity: " + String(AppleMIDI.getSessionName()) );
#endif

#endif // CONFIG_ENABLE_MIDI

	// OTA updater
	msGlobals.ggModeList.push_back(&msMagicUpdate);

	// Show the battery power level on startup
	msSystem.showBatteryStatus(true);

	// mode selector is a special submode, so we start it now also
	msModeSelector.start();

	// now prepare the first module for starting
	msGlobals.ggModeList[msGlobals.ggCurrentMode]->start();
}


// main OS loop - despatch steps for each of:
//	a) system user interface events (buttons/sensors/power)
//  b) webserver events
//  c) if configured, MIDI and rtpMIDI too..
//
// then check for menu-mode flags, switch immediately to menu-mode if necessary
//   .. otherwise, let the currently selected mode execute a step()
// last, check for general faults
void loop()
{
	// let WebServer process events.
	msWebServer.step();

	// let SystemUI process events.
	msSystem.step();

	// if rtpMIDI is configured, run it..
#ifdef CONFIG_ENABLE_MIDI_RTP
		AppleMIDI.run();
#endif

#ifdef CONFIG_ENABLE_MIDI_SERIAL
		SERIAL_MIDI_loop();
#endif

	// if SystemUI events trigger it (UI), display the mode-selector menu:
	if (msSystem.modeMenuActivated) {
		int newMode = msModeSelector.select();
		if (newMode >= 0) {
			msSystem.modeMenuActivated = false;
			msSystem.setMode(newMode);
		}
	} else {
		// despatch to current mode
		msGlobals.ggModeList[msGlobals.ggCurrentMode]->step();
	}

	// fault-checks
	if (msGlobals.ggFault > 0) {
		systemFault(msGlobals.ggFault);
	}
}
