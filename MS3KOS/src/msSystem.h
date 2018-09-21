#ifndef _MS_SYSTEM_H
#define _MS_SYSTEM_H

#include <math.h>
#include <Wire.h>				// Used for I2C
#include <Arduino.h>
#include <Esp.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include <EEPROM.h>
#include <SPI.h>
// #include <WiFi.h>
// #include <WiFiUdp.h>

#include "Hardware/EEPROMString.h"
#include "Hardware/LEDHardware.h"
#include "Hardware/Sensor.h"
#include "Hardware/Buttons.h"

// Custom image handler for MS3000 POV images
#include "msImage.h"
#include "msSysLog.h"

// Note: here we use a timer to sample the
// middle button (power-down/brightness control)
// it is done in a timer because the AD op is slow
// and therefore better done outside the context
// of the main runloop
#include "msButtonTimers.h"

// Current View per MIDI input
typedef struct {
	uint8_t midi_channel;		// MIDI channel of View
	uint16_t time_base;			// Base Time for sequencer-Put
	void *v_arg;				// user data
} MIDIViewT;

MIDIViewT curr_midiview;

// forward-declared here because it is a client of msSystem ..
void CommandInterfacePoll();

class MagicShifterSystem;
extern MagicShifterSystem msSystem;

// TODO: all init and all sensors and leds in here :)
// (accelerometer wuld also be a class but the MAgicShifter object has one ;)
class MagicShifterSystem {

	class SettingsManager {

	private:
			// used in resetAPList & getNextAP
		int apListIndex = -1;
		File smAPListFile;
		APAuth apInfo;

		bool loadData(String path, void *config, int len) {
			if (SPIFFS.exists((char *) path.c_str())) {
				File file = SPIFFS.open((char *) path.c_str(), "r");
				file.read((uint8_t *) config, len);
				file.close();

				return true;
			} else {
				msSystem.slog("webserver: loadData: can not open config file ");
				msSystem.slogln((char *) path.c_str());
			}

			return false;
		}

		bool saveData(String path, void *config, int len) {
			File file = SPIFFS.open((char *) path.c_str(), "w");
			if (file) {
				file.write((uint8_t *) config, len);
				file.close();

				return true;
			} else {
				msSystem.slog("webserver: can not open config file ");
				msSystem.slogln((char *) path.c_str());

				return false;
			}
		}



	public:

		const String apConfigPath = "settings/ap.bin";
		const String apServerConfigPath = "settings/server1.bin";
		const String apListConfigPath = "settings/aplist1.bin";
		const String apSysLogConfigPath = "settings/syslog.bin";
		const String preferredAPConfigPath = "settings/preferredap.bin";
		const String uiSettingsConfigPath = "settings/ui.bin";

		String getUniqueSystemName() 
		{
			uint8_t mac[WL_MAC_ADDR_LENGTH];
			WiFi.softAPmacAddress(mac);
			String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);

			macID.toUpperCase();
			String UniqueSystemName = String(AP_NAME_OVERRIDE) + String("-") + macID;

			return UniqueSystemName;
		}

		bool getUIConfig(struct UIConfig *config) {
				//msSystem.slog("config: sizeof ");
				//msSystem.slogln(String(sizeof(*config)));
			bool result = loadData(uiSettingsConfigPath, config, sizeof(*config));
			if (!result) {
				config->timeoutHighPower = 0;
					config->timeoutLowPower =  10 * 60 * 1000; // 10 minutes
					config->defaultBrightness = 2;
				}
			return result;
		}

		bool setUIConfig(struct UIConfig *config) {
			return saveData(uiSettingsConfigPath, config, sizeof(*config));
		}

		String getAPNameOrUnique() {
			String retName;
			bool gotAPConfig = msSystem.Settings.getAPConfig(&apInfo);

			if (gotAPConfig) {
				retName = String(apInfo.ssid);
			}
			else {
				retName = msSystem.Settings.getUniqueSystemName();
			}

			return retName;

		}

		bool getServerConfig(struct ServerConfig *config) {

			String path = apServerConfigPath;
			if (SPIFFS.exists((char *) path.c_str())) {
				File file = SPIFFS.open((char *) path.c_str(), "r");
				file.read((uint8_t *) config, sizeof(*config));
				file.close();
				return true;
			} else {
				msSystem.slog("webserver: no server config file? ");
				msSystem.slogln((char *) path.c_str());
			}

			return false;
		}

		void setServerConfig(struct ServerConfig *config) {

			String path = apServerConfigPath;
			File file = SPIFFS.open((char *) path.c_str(), "w");
			file.write((uint8_t *) config, sizeof(*config));

			file.close();

		}

		bool getSyslogConfig(struct ServerConfig *config) {

			String path = apSysLogConfigPath;
			if (SPIFFS.exists((char *) path.c_str())) {
				File file = SPIFFS.open((char *) path.c_str(), "r");
				file.read((uint8_t *) config, sizeof(*config));
				file.close();
				return true;
			} else {
				msSystem.slog("webserver: no syslog config file? ");
				msSystem.slogln((char *) path.c_str());
			}

			return false;
		}

		void setSyslogConfig(struct ServerConfig *config) {

			String path = apSysLogConfigPath;
			File file = SPIFFS.open((char *) path.c_str(), "w");
			file.write((uint8_t *) config, sizeof(*config));

			file.close();

		}

		bool getAPConfig(struct APAuth *config) {

			String path = apConfigPath;

			if (SPIFFS.exists((char *) path.c_str())) {
				File file = SPIFFS.open((char *) path.c_str(), "r");
				file.read((uint8_t *) config, sizeof(*config));
				file.close();

				msSystem.slogln("webserver: AP name is: " + String(config->ssid));

				return true;

			} else {
				msSystem.slogln("webserver: AP config missing: " + path);
			}

			// don't have a config file, so we return a default
			l_safeStrncpy(config->ssid, getUniqueSystemName().c_str(),
				sizeof(config->ssid));
			l_safeStrncpy(config->password, "", sizeof(config->password));

msSystem.slogln("webserver: not configured, using unique name: " + String(config->ssid));

			return false;
		}

		void setAPConfig(struct APAuth *config) {

			String path = apConfigPath;
			File file = SPIFFS.open((char *) path.c_str(), "w");
			file.write((uint8_t *) config, sizeof(*config));
			file.close();

			msSystem.slogln("webserver: saved:");
			msSystem.slogln(config->ssid);

		}

		bool getPreferredAP(struct APAuth *config) {

			String path = preferredAPConfigPath;
			if (SPIFFS.exists((char *) path.c_str())) {
				File file = SPIFFS.open((char *) path.c_str(), "r");
				file.read((uint8_t *) config, sizeof(*config));
				file.close();
				return true;
			}
			l_safeStrncpy(config->ssid, "", sizeof(config->ssid));
			l_safeStrncpy(config->password, "", sizeof(config->password));
			return false;

		}

		void setPreferredAP(struct APAuth *config) {

			String path = preferredAPConfigPath;
			File file = SPIFFS.open((char *) path.c_str(), "w");
			file.write((uint8_t *) config, sizeof(*config));
			file.close();

		}

		//
		// delete an AP Auth structure from the list
		// if its not our ssid, add it to the list,
		// otherwise ignore it 
		// and save the list again
		//
		void deleteAP(char *ssid) {
			String path = apListConfigPath;

			typedef std::map<String, String> AuthItems;
			typedef std::map<String, String>::iterator AuthItems_it;
			AuthItems authItems;

			File inFile;
			APAuth inAPAuth;

			// open the existing AP settings file if we can, and construct the map
			if (SPIFFS.exists((char *)path.c_str()))  {
				inFile = SPIFFS.open((char *) path.c_str(), "r");
			}
			if (inFile) {
				msSystem.slog("webserver: opened AP config file:");
				msSystem.slogln(path);

				// create a list of AP entries already in the file
				while (inFile.read((uint8_t *) &inAPAuth, sizeof (APAuth)) == sizeof(APAuth)) {
					// exclude the one we want to delete
					if (strncmp(inAPAuth.ssid, ssid, MAX_AP_LEN) == 0)
						continue;
					else
						authItems[inAPAuth.ssid] = inAPAuth.password;
				}

				inFile.close();

			} else {
				msSystem.slog("webserver: couldn't open AP inFile:");
				msSystem.slogln(path);
			}

			// dump the map back to the file
			File outFile = SPIFFS.open((char *) path.c_str(), "w+");
			if (outFile) {
				AuthItems_it it;
				for(it = authItems.begin(); it != authItems.end(); it++)
				{
					APAuth t_Auth;
					l_safeStrncpy(t_Auth.ssid, it->first.c_str(), MAX_AP_LEN);
					l_safeStrncpy(t_Auth.password, it->second.c_str(), MAX_AP_LEN);
					outFile.write((uint8_t *)&t_Auth, sizeof(APAuth));
				}

				outFile.close();
				msSystem.slog("webserver: saved AP configuration");
				msSystem.slogln(path);
			} else {
				msSystem.slog("webserver: couldn't save AP outFile:");
				msSystem.slogln(path);
			}

		}

		//
		// add an AP Auth structure to the list
		// if its there already, update the password
		// if its not there, add it to the list
		//
		void addAP(struct APAuth *apInfo) {
			String path = apListConfigPath;

			typedef std::map<String, String> AuthItems;
			typedef std::map<String, String>::iterator AuthItems_it;
			AuthItems authItems;

			File inFile;
			APAuth inAPAuth;

			// open the existing AP settings file if we can, and construct the map
			if (SPIFFS.exists((char *)path.c_str()))  {
				inFile = SPIFFS.open((char *) path.c_str(), "r");
			}
			if (inFile) {
				msSystem.slog("webserver: opened AP config file:");
				msSystem.slogln(path);

				// create a list of AP entries already in the file
				while (inFile.read((uint8_t *) &inAPAuth, sizeof (APAuth)) == sizeof(APAuth)) {
					authItems[inAPAuth.ssid] = inAPAuth.password;
				}

				inFile.close();

			} else {
				msSystem.slog("webserver: couldn't open AP inFile:");
				msSystem.slogln(path);
			}

			// check if there is an entry in the map for the incoming apInfo
			// (map could also be empty)
			auto existingAP = authItems.find(apInfo->ssid);

			if (existingAP != authItems.end()) {
		    	// we found it, so set the new password
				existingAP->second = apInfo->password;
		    } else { 	// .. otherwise, add one and set the password
		    	authItems[apInfo->ssid] = apInfo->password;
		    }

			// dump the map back to the file
		    File outFile = SPIFFS.open((char *) path.c_str(), "w+");
		    if (outFile) {
		    	AuthItems_it it;
		    	for(it = authItems.begin(); it != authItems.end(); it++)
		    	{
		    		APAuth t_Auth;
		    		l_safeStrncpy(t_Auth.ssid, it->first.c_str(), MAX_AP_LEN);
		    		l_safeStrncpy(t_Auth.password, it->second.c_str(), MAX_AP_LEN);
		    		outFile.write((uint8_t *)&t_Auth, sizeof(APAuth));
		    	}

		    	outFile.close();
		    	msSystem.slog("webserver: saved AP configuration");
		    	msSystem.slogln(path);
		    } else {
		    	msSystem.slog("webserver: couldn't save AP outFile:");
		    	msSystem.slogln(path);
		    }

		}

		void resetAPList() {
			apListIndex = -1;
			smAPListFile.close();

		}

		bool getNextAP(struct APAuth *apInfo) {
			if (apListIndex < 0) {
				String path = apListConfigPath;
				if (SPIFFS.exists((char *) path.c_str())) {
					smAPListFile = SPIFFS.open((char *) path.c_str(), "r");
					apListIndex = 0;
				}
			}

			if (apListIndex >= 0) {
				const int requiredBytes = sizeof(*apInfo);
				do {
					if (smAPListFile.read((uint8_t *) apInfo, requiredBytes) ==
						requiredBytes) {
						apListIndex++;
					if (!msSystem.msEEPROMs.
						memcmpByte((byte *) apInfo, 0, requiredBytes))
						return true;
					} else {
						return false;
					}
				} while (true);

			} else {
				return false;
			}
				///hack
			return false;
		}
		
	};

#define WL_MAC_ADDR_LENGTH 6

public:
	SettingsManager Settings;
	MagicShifterSysLog msSysLog;
	// todo: protect
	MagicShifterAccelerometer msSensor;
	MagicShifterLEDs msLEDs;
	MagicShifterEEPROMString msEEPROMs;
	MagicShifterButtons msButtons;
	MDNSResponder msDNS;
	ESP8266WebServer msESPServer;
	WiFiManager msWifiManager;

	bool msMIDIActive = true;
	int msFrame = 0;
	bool msSensorOK = false;
	unsigned long msPowerCountDown = 0;

	int modeMenuActivated = false;

	uint32_t lowBatteryMillis;
	float  batteryVoltage = 0.0;

	void slog(String msg) {
		msSysLog.sendSysLogMsg(msg);
	};
	void slogln(String msg) {
		msSysLog.sendSysLogMsg(msg);
	};

	void slog(int8_t & msg, int base) {
		slog(String(msg));
	}
	void slog(uint16_t & msg, int base) {
		slog(String(msg));
	}
	void slog(unsigned int msg, int base) {
		slog(String(msg));
	}
//  void slog(bool b) { if (b) slog(String("true")); else slog(String("false")); }

	void slogln(int8_t & msg, int base) {
		slogln(String(msg));
	}
	void slogln(uint16_t & msg, int base) {
		slogln(String(msg));
	}
	void slogln(unsigned int msg, int base) {
		slogln(String(msg));
	}
//  void slogln(bool b) { if (b) slogln(String("true")); else slogln(String("false")); }

	void dumpActiveHeader(const MSBitmapHeader & header) {
		slogln("Header dump:");
		slogln("fileSize:" + String(header.fileSize));
		slogln("pixelFormat:" + String(header.pixelFormat));
		slogln("maxFrame:" + String(header.maxFrame));
		slogln("frameWidth:" + String(header.frameWidth));
		slogln("frameHeight:" + String(header.frameHeight));
		slogln("subType:" + String(header.subType));
		slogln("firstChar:" + String(header.firstChar));
		slogln("animationDelay:" + String(header.animationDelay));
	}

	void slogSysInfo() {
		slogln("System config:");
		slogln("Vcc: " + String(ESP.getVcc()));
		slogln("Free heap: " + String(ESP.getFreeHeap()));
		slogln("Chip ID: " + String(ESP.getChipId()));
		slogln("SDK version: " + String(ESP.getSdkVersion()));
		slogln("Boot version: " + String(ESP.getBootVersion()));
		slogln("Boot mode: " + String(ESP.getBootMode()));
		slogln("CPU freq.: " + String(ESP.getCpuFreqMHz()));
		slogln("Flash chip ID: " + String(ESP.getFlashChipId(), HEX));
		// // gets the actual chip size based on the flash id
		slogln("Flash real size: " + String(ESP.getFlashChipRealSize()));
		slogln("Flash real size (method b): " + String(ESP.getFlashChipSizeByChipId()));
		// // gets the size of the flash as set by the compiler
		slogln("flash configured size: " + String(ESP.getFlashChipSize()));
		if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize())
		{
			slogln(String("WARNING: configured flash size does not match real flash size!"));
		}
		slogln("flash speed: " + String(ESP.getFlashChipSpeed()));
		slogln("flash mode: " + String(ESP.getFlashChipMode()));
		slogln("Sketch size: " + String(ESP.getSketchSize()));
		slogln("Free sketch space: " + String(ESP.getFreeSketchSpace()));

		slogln("uploadfile: " + String(msGlobals.ggUploadFileName));

		FSInfo linfo;
		SPIFFS.info(linfo);

		slogln("linfo.blockSize =" + String(linfo.blockSize));
		slogln("linfo.pageSize =" + String(linfo.pageSize));
		slogln("linfo.maxOpenFiles =" + String(linfo.maxOpenFiles));
		slogln("linfo.maxPathLength =" + String(linfo.maxPathLength));
		slogln("linfo.totalBytes =" + String(linfo.totalBytes));
		slogln("linfo.usedBytes = " + String(linfo.usedBytes));

		slogln("Reset info: " + String(ESP.getResetInfo()));

		slogln("timeoutHighPower: " + String(msGlobals.ggUIConfig.timeoutHighPower));
		slogln("timeoutLowPower: " + String(msGlobals.ggUIConfig.timeoutLowPower));
		slogln("defaultBrightness: " + String(msGlobals.ggUIConfig.defaultBrightness));
	};


	int  loadCalibration()
	{
		File calibFile;
		uint16_t calib = 0;

		calibFile = SPIFFS.open(CALIBRATION_FILENAME, "r");

		if (calibFile) {
			calibFile.read((unsigned byte *)&calib, sizeof (int));
			calibFile.close();
		}

		return calib;
	}

	void saveCalibration(int calib_value)
	{
		File calibFile;

		calibFile = SPIFFS.open(CALIBRATION_FILENAME, "w");

		if (calibFile) {
			calibFile.write((unsigned byte *)&calib_value, sizeof (int));
			calibFile.close();
		}
	}

	void powerCalibrate() {
		saveCalibration(msGlobals.ggLastADValue);
		msGlobals.batVoltCalibration = msGlobals.ggLastADValue;
	}


	// reset the power controller
	void powerStabilize() {

#ifdef CONFIG_PWR_FORCE
		// this forces pwr_enable to high in the circuit so it doesn't turn off
		digitalWrite(PIN_PWR_MGT, HIGH);
		pinMode(PIN_PWR_MGT, OUTPUT);
		digitalWrite(PIN_PWR_MGT, HIGH);
#endif

	}

	// tell power controller to power down
	void powerDown() {
		slogln("POWERDOWN: Now we sleep forever..");
		showBatteryStatus(false);
		// works even with pulldown but output seems to make more sense
		//pinMode(PIN_PWR_MGT, INPUT_PULLDOWN);
		pinMode(PIN_PWR_MGT, OUTPUT);
		digitalWrite(PIN_PWR_MGT, LOW);
		// now sleep forever...
		delay(1000);
	}

	// sets the current shifter Mode .. 
	void setMode(uint32_t newMode)
	{
		if (newMode < msGlobals.ggModeList.size() && 
			(newMode != msGlobals.ggCurrentMode)) {
			msGlobals.ggModeList[msGlobals.ggCurrentMode]->stop();
			msGlobals.ggCurrentMode = newMode;
			msGlobals.ggModeList[newMode]->start();
		}
	}

	void feedbackAnimation(int mode) {
		int r, g, b = 0x00;

		if (mode == msGlobals.feedbackType::OK) {
			r = 0x00;
			g = 0xff;
			b = 0x00;
		} else if (mode == msGlobals.feedbackType::NOT_OK) {
			r = 0xff;
			g = 0x00;
			b = 0x00;
		} else {
			r = 0xff;
			g = 0xff;
			b = 0xff;
		}

		for (int i = 0; i <= 3; i++) {
			msLEDs.fillLEDs(r, g, b, msGlobals.ggBrightness);
			msLEDs.updateLEDs();
			delay(35);
			msLEDs.fastClear();
			delay(35);
		}

		msLEDs.setLED(msGlobals.ggCurrentMode, 128, 128, 128, msGlobals.ggBrightness);
		delay(35);
		msLEDs.fastClear();

	}

		// for fail-modes ..
	void infinite_swipe() {
		while (1) {
				// swipe colors
			for (byte idx = 0; idx < MAX_LEDS; idx++) {
				msLEDs.setLED(idx, (idx & 1) ? 255 : 0,
					(idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0,
					msGlobals.ggBrightness);
				msLEDs.updateLEDs();
				delay(30);
			}
			for (byte idx = 0; idx < MAX_LEDS; idx++) {
				msLEDs.setLED(idx, 0, 0, 0, 1);
				msLEDs.updateLEDs();
				delay(30);
			}
		}
	}

#define BUTTON_LED_A (MAX_LEDS - 1)
#define BUTTON_LED_PWR (MAX_LEDS / 2)
#define BUTTON_LED_B 0
#define BUTTON_DISPLAY_PERIOD 1


void displayButtons() {
	if (msButtons.msBtnPwrDoubleHit) {
		msLEDs.fillLEDs(0, 200, 0, msGlobals.ggBrightness);
		msLEDs.updateLEDs();
		delay(BUTTON_DISPLAY_PERIOD);
	}

	if (msButtons.msBtnPwrLongHit) {
		msLEDs.setLED(BUTTON_LED_PWR, 0, 0, 20, 20);
		msLEDs.setLED(BUTTON_LED_PWR + 1, 0, 0, 20, 20);
		msLEDs.updateLEDs();
		delay(BUTTON_DISPLAY_PERIOD);
	}
	if (msButtons.msBtnPwrHit) {
		msLEDs.setLED(BUTTON_LED_PWR, 20, 20, 0, 15);
		msLEDs.setLED(BUTTON_LED_PWR - 1, 20, 20, 0, 15);
		msLEDs.updateLEDs();
		delay(BUTTON_DISPLAY_PERIOD);
	}
	if (msButtons.msBtnALongHit) {
		msLEDs.setLED(BUTTON_LED_A, 20, 0, 20, 20);
		msLEDs.updateLEDs();
		delay(BUTTON_DISPLAY_PERIOD);
	}
	if (msButtons.msBtnAHit) {
		msLEDs.setLED(BUTTON_LED_A, 20, 20, 0, 20);
		msLEDs.updateLEDs();
		delay(BUTTON_DISPLAY_PERIOD);
	}
	if (msButtons.msBtnBLongHit) {
		msLEDs.setLED(BUTTON_LED_B, 20, 0, 20, 20);
		msLEDs.updateLEDs();
		delay(BUTTON_DISPLAY_PERIOD);
	}
	if (msButtons.msBtnBHit) {
		msLEDs.setLED(BUTTON_LED_B, 20, 20, 0, 20);
		msLEDs.updateLEDs();
		delay(BUTTON_DISPLAY_PERIOD);
	}
}

#define BRIGHTNESS_CONTROL_TIME (850 * 1000)

uint8_t BrightnessLevels[16] = { 1, 2, 3, 4,
	5, 6, 7, 8,
	10, 12, 14, 16,
	18, 22, 26, 31
};

#define BRIGHTNESS_UI_LEVEL 0xFF

// -- brightness handling:
void brightnessControlStep() {
	int newIdx = msGlobals.ggBrightness;
	float avgV = 0;
	int newV = 0, lastV = -1;
	uint16_t blink = 0;
	int skip = 100;

		// state: on
	if ((msButtons.powerButtonPressed()) &&
		(msButtons.msBtnPwrPressTime > BRIGHTNESS_CONTROL_TIME)) {

		slogln("brightnessControlStep EVENT");

	while (skip) {
		delay(1);

		getADValue();
		msButtons.step();
		msSensor.step();

		if (msButtons.powerButtonPressed())
			skip = 100;
		else {
			if (skip > 0)
				skip--;
		}

				// AccelPoll();

				// calculate average curve
		float fFactor = 0.96;
		avgV =
		(fFactor * avgV) + msGlobals.ggAccel[XAXIS] * (1 -
			fFactor);
		float lLEDRange = ((MAX_LEDS - 1.0) / 2.0);
				// calculate LED index
		newIdx =
		(int) ((lLEDRange * 1.4) + (lLEDRange * avgV) * 1.8);

		if (msGlobals.ggFault > 0)
			newIdx = -2;

		if (newIdx < -1) {
			newV = 0;
		} else {
			blink = 0;
			if (newIdx < 0)
				newIdx = 0;
			if (newIdx >= 15)
				newIdx = 15;
			newV = BrightnessLevels[newIdx];
		}

		if (newV == 0) {
			msLEDs.fillLEDs(0, 0, 0, 0);
			blink++;
			uint16_t bb = blink & 0x1FF;
			if (bb > 255)
				bb = 511 - bb;
					//bb = (v*bb)/255;

			msLEDs.setLED(4, bb / 8, bb / 8, bb / 8, msGlobals.ggBrightness);
			msLEDs.setLED(5, bb / 4, bb / 4, bb / 4, msGlobals.ggBrightness);
			msLEDs.setLED(6, bb / 2, bb / 2, bb / 2, msGlobals.ggBrightness);
			msLEDs.setLED(7, bb, bb, bb, msGlobals.ggBrightness);
			msLEDs.setLED(8, bb, bb, bb, msGlobals.ggBrightness);
			msLEDs.setLED(9, bb / 2, bb / 2, bb / 2, msGlobals.ggBrightness);
			msLEDs.setLED(10, bb / 4, bb / 4, bb / 4, msGlobals.ggBrightness);
			msLEDs.setLED(11, bb / 8, bb / 8, bb / 8, msGlobals.ggBrightness);

			msLEDs.updateLEDs();
			delayMicroseconds(200);
		} else if (lastV != newV) {
			for (uint8_t i = 0; i <= 15; i++) {

				uint8_t lBr = BrightnessLevels[i];

				uint8_t dB = lBr;

						//if (dB <= 16) dB = 16;
				if (newV >= lBr)
					msLEDs.setLED(15 - i, BRIGHTNESS_UI_LEVEL,
						BRIGHTNESS_UI_LEVEL,
						BRIGHTNESS_UI_LEVEL, dB);
				else
					msLEDs.setLED(15 - i, 0, 0, 0);
			}
			msLEDs.updateLEDs();
			delayMicroseconds(200);
		}
		lastV = newV;
	}

	if (newV == 0)
		powerDown();

	msGlobals.ggBrightness = newV;

	WaitClearButtons();

	msLEDs.fillLEDs(0, 0, 0, 0);
	msLEDs.updateLEDs();
}
}


// -- battery status methods:
#define LIPO_DISPLAY_LOWER_LIMIT_V         3.1
#define LIPO_DISPLAY_RED_LIMIT_V           3.65
#define LIPO_DISPLAY_ORANGE_LIMIT_V        3.9
#define LIPO_DISPLAY_UPPER_LIMIT_V         4.5

void WaitClearButtons() {
	while (msButtons.powerButtonPressed()) {
		delay(1);
	}
}

void showBatteryStatus(bool shouldFadeIn) {
		//v = MAXMV;

	int d = 500;
	int gs = 10;
	float batLevel = 0.0f;

	if (!shouldFadeIn)
		d = d * -1;

	WaitClearButtons();
	delay(50);
	batLevel = getBatteryVoltage();

	msLEDs.fillLEDs(0, 0, 0, 0);

	for (int i = 0; i >= 0 && i <= 15; i++) {
		float iV =
		LIPO_DISPLAY_LOWER_LIMIT_V + (LIPO_DISPLAY_UPPER_LIMIT_V -
			LIPO_DISPLAY_LOWER_LIMIT_V) *
		(i / 16.0);

		if (batLevel > iV) {
			int red, green;
			if (iV > LIPO_DISPLAY_RED_LIMIT_V) {
				green =
				255 * (iV -
					LIPO_DISPLAY_RED_LIMIT_V) /
				(LIPO_DISPLAY_UPPER_LIMIT_V -
					LIPO_DISPLAY_RED_LIMIT_V);
			} else
			green = 0;

			if (iV < LIPO_DISPLAY_ORANGE_LIMIT_V) {
				red =
				255 * (LIPO_DISPLAY_ORANGE_LIMIT_V -
					iV) / (LIPO_DISPLAY_ORANGE_LIMIT_V -
					LIPO_DISPLAY_LOWER_LIMIT_V);
				} else
				red = 0;
				msLEDs.setLED(15 - i, red, green, 0, gs);

				//msLEDs.setLED(i, 0, iV > LIPO_DISPLAY_RED_LIMIT_V  ? 150 : 0, iV < LIPO_DISPLAY_ORANGE_LIMIT_V ? 150 : 0, gs);
			}

			if (d > 0) {
				msLEDs.updateLEDs();
				delay(12);
			}
		}
		msLEDs.updateLEDs();


		if (d < 0) {
			d = -d;
			for (int i = 0; i < d; i += 20) {
				delay(20);
			}

			for (int i = 0; i >= 0 && i <= 15; i++) {
				msLEDs.setLED(i, 0, 0, 0);
				msLEDs.updateLEDs();
				delay(12);
			}
		} else {
			delay(100);
			for (int i = 0; i < d; i += 20) {
				delay(20);
			}
		}

		msLEDs.fillLEDs(0, 0, 0, 0);;
		msLEDs.updateLEDs();

	}

// NOTE: this is being left in for future testing of SPIFFS .. 
	void TEST_SPIFFS_bug()
	{

		const char* debugPath = "XXXXX";
		uint8_t testVals[] = {1,23, 3, 7};
		uint8_t readBuffer[] = {0,0,0,0};
    //File file = SPIFFS.open((char *)debugPath.c_str(), "w");

		slogln("openin for w: ");
		slogln(String(debugPath));

		File file = SPIFFS.open(debugPath, "w");

		slogln("opended for w: ");
		slogln(String((bool)file));

		slogln("writin: ");
		slogln(String(testVals[1]));

		file.write((uint8_t *)testVals, sizeof testVals);
		file.close();

		slogln("openin for r: ");
		slogln(String(debugPath));

		File fileR = SPIFFS.open(debugPath, "r");

		slogln("opended for r: ");
		slogln(String((bool)fileR));

		slogln("readin: ");

		fileR.read((uint8_t *)readBuffer, sizeof readBuffer);
		fileR.close();

		slogln("readback: ");
		slogln(String(readBuffer[1]));
	};


	// gets the basic stuff set up
	void setup() {

		// led controllers and buffer
		msLEDs.initLEDHardware();
		msLEDs.initLEDBuffer();

		EEPROM.begin(512);

#ifdef CONFIG_ENABLE_MIDI_SERIAL
#warning "SERIAL MIDI has been enabled - Serial I/O at 31250 - serial logging disabled (use wlan)"
		Serial.begin(31250);
#else
#warning "SERIAL MIDI is disabled - syslog will use serial logging"
		Serial.begin(921600);
#endif

		slogln(String("\r\nMagicShifter 3000 OS V" + String(MS3KOS_VERSION)));

		// wake up filesystem
		slog("SPIFFS:");

		if (SPIFFS.begin())
			slog("done:");
		else
			slog("noSPIFFS:");

		// TEST_SPIFFS_bug();

		Settings.getUIConfig(&msGlobals.ggUIConfig);

		msGlobals.ggBrightness = msGlobals.ggUIConfig.defaultBrightness;

		msGlobals.batVoltCalibration = msSystem.loadCalibration();

		msLEDs.bootSwipe();

		// all engines turn on
		pinMode(PIN_PWR_MGT, INPUT);

		// pinMode(PIN_LED_ENABLE, INPUT);

		// reset power controller to stay on
		// had some power down troubles so this needs to be further investigated
		powerStabilize();
		// !J! todo: power-management module 

		msButtons.setup();

		initMS3000SystemTimers();

#ifdef CONFIG_ENABLE_ACCEL
		// accelerometer 
		msSensor.initI2C();
		msSensorOK = msSensor.setupSensor();	//Test and intialize the MMA8452
#endif

		// led controllers and buffer
		msLEDs.initLEDHardware();
		msLEDs.initLEDBuffer();

		// global font objects
		MagicShifterImage::LoadBitmapBuffer("font4x5.magicFont",
			&msGlobals.ggtBitmap4x5);
		MagicShifterImage::LoadBitmapBuffer("font6x8.magicFont",
			&msGlobals.ggtBitmap6x8);
		MagicShifterImage::LoadBitmapBuffer("font7x12.magicFont",
			&msGlobals.ggtBitmap7x12);
		MagicShifterImage::LoadBitmapBuffer("font10x16.magicFont",
			&msGlobals.ggtBitmap10x16);



		slogSysInfo();
		
	}

	// restart the system
	void restart() {
		
		ESP.restart();
		
		do { 
			delay(1);
		} while(1); // !J! wait until the restart completes
	}

	void calculateBatteryLevel() {
		batteryVoltage = calculateVoltage(msGlobals.ggLastADValue, msGlobals.batVoltCalibration);

		if (batteryVoltage < 4.0) {
			if (lowBatteryMillis == 0)
				lowBatteryMillis = msGlobals.ggCurrentMillis;
		}
		else
		{
			lowBatteryMillis = 0;
		}
	}

	void updateGlobalTiming(){
		msGlobals.ggLastMicros = msGlobals.ggCurrentMicros;
		msGlobals.ggCurrentMicros = micros();
		msGlobals.ggCurrentMillis = millis();
	}

	void powerDownStep()
	{
		calculateBatteryLevel();

		if (msButtons.msBtnActive) {
			msPowerCountDown = msGlobals.ggCurrentMillis;
		}

		if (msGlobals.ggUIConfig.timeoutHighPower != 0) {
			// note! unsigned values!
			if (msPowerCountDown + msGlobals.ggUIConfig.timeoutHighPower < msGlobals.ggCurrentMillis) {
				powerDown();
			}
		}

		// 
		if ((lowBatteryMillis != 0) && 
			(msGlobals.ggUIConfig.timeoutLowPower != 0) && 
			(lowBatteryMillis + (10 * 1000) < msGlobals.ggCurrentMillis)) { // 10 seconds 
			// note! unsigned values!
			if (msPowerCountDown + msGlobals.ggUIConfig.timeoutLowPower < msGlobals.ggCurrentMillis) {
				powerDown();
			}
		}
	}

	void updateGlobalFrameCounts()
	{
		// update timing post-system work
		msGlobals.ggLFrameTime = msGlobals.ggCurrentMicros - msGlobals.ggLastFrameMicros;
		msGlobals.ggCurrentFrame++;
		msGlobals.ggLastFrameMicros = msGlobals.ggCurrentMicros;
	}

	// internal system loop - handles the system UI, basic buttons, mode-switching, etc.
	void step() {

		// let timers spin for AD read 
		yield();

		// displayButtons(); // for debugging

		// time globals are subsequently used
		updateGlobalTiming();

		// getADValue(); // !J! timer does it instead

		// check for and display the brightness control if users holds middle button
		brightnessControlStep();

		// buttons and sensor handling
		msButtons.step();
		msSensor.step();

		// power down if user, or battery state, requires it
		powerDownStep();

		// check for the menu buttons being held down by the user
		if (msButtons.menuButtonsSelected()) {
			if (!modeMenuActivated) {
				modeMenuActivated = true;
				feedbackAnimation(msGlobals.feedbackType::MODE_MENU);
			}
		}

#ifndef CONFIG_ENABLE_MIDI_SERIAL
		// poll the serial interface for test/flash commands, etc.
		CommandInterfacePoll();
#endif

		updateGlobalFrameCounts();

	}

	void enableLongClicks(bool enable) {
		msButtons.msLongClickOK = enable;
	}

	// slow analog read
	// we store the value so that it is available per
	// module step()
	int getADValue(void) {
		msGlobals.ggLastADValue = analogRead(A0);
		return msGlobals.ggLastADValue;
	}


	float getBatteryVoltage(void) {
		return calculateVoltage(msGlobals.ggLastADValue, msGlobals.batVoltCalibration);
	}

	IPAddress getIP() {
		if (msGlobals.ggModeAP) {
			return WiFi.softAPIP();
		} else {
			return WiFi.localIP();
		}
	}

	void hexDump(int len, byte * buf, const char *label) {
		slog(label);
		slog(String(len));
		slog("/");

		for (int x = 0; x < len; x++) {
			if (x % 8 == 0)
				slogln("");
			// slog(":");
			Serial.print(buf[x], HEX); Serial.print (" ");
		}
		slogln("<<EOF");
	}

};

#include "CommandInterface.h"


#else
extern MagicShifterSystem msSystem;
#endif
