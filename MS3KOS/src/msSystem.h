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


#include <Base64.h>
#include <EEPROM.h>
#include <SPI.h>
// #include <WiFi.h>
// #include <WiFiUdp.h>

#include "Hardware/EEPROMString.h"
#include "Hardware/LEDHardware.h"
#include "Hardware/Sensor.h"
#include "Hardware/Buttons.h"
#include "msImage.h"
#include "msSysLog.h"

#include "button_timers.h"

// MIDI features can be configured in or out of
// the project according to Serial needs, and can be
// enabled for WiFi.
// e.g. debugging
//
#ifdef CONFIG_ENABLE_MIDI
#include "AppleMidi.h"
// RTPMIDI is usable over WiFi
#define CONFIG_MIDI_RTP_MIDI
APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h
bool isRTPConnected = false;

// RTP MIDI event handlers:
// -----------------------------------------------------------------------------
void OnRTPMIDI_Connect(uint32_t ssrc, char* name) {
  isRTPConnected = true;
  Serial.print("Connected to session ");
  Serial.println(name);
}

void OnRTPMIDI_Disconnect(uint32_t ssrc) {
  isRTPConnected = false;
  Serial.println("Disconnected");
}

void OnRTPMIDI_NoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOn from channel:");
  Serial.print(String(channel));
  Serial.print(" note:");
  Serial.print(String(note));
  Serial.print(" velocity:");
  Serial.println(String(velocity));
}

void OnRTPMIDI_NoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOff from channel:");
  Serial.print(String(channel));
  Serial.print(" note:");
  Serial.print(String(note));
  Serial.print(" velocity:");
  Serial.println(String(velocity));
}
#endif // CONFIG_MIDI_RTP_MIDI

// OSC = Open Sound Control
#ifdef CONFIG_ENABLE_OSC
#include "OSCMessage.h"
#endif // CONFIG_ENABLE_OSC


// forward-declared here because it is a client of msSystem ..
void CommandInterfacePoll();

#include "msSettingsManager.h"

// TODO: all init and all sensors and leds in here :)
// (accelerometer wuld also be a class but the MAgicShifter object has one ;)
class MagicShifterSystem {

#define WL_MAC_ADDR_LENGTH 6

  private:

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

	int lowBatteryMillis;
	float  batteryVoltage = 0.0;

	bool _shouldLocalYield = true;

  public:
	// todo:switch slog from OFF, to BANNED (MIDI), to UDP .. etc.

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
		slog("fileSize:");
		slogln(String(header.fileSize));
		slog("pixelFormat:");
		slogln(String(header.pixelFormat));
		slog("maxFrame:");
		slogln(String(header.maxFrame));
		slog("frameWidth:");
		slogln(String(header.frameWidth));
		slog("frameHeight:");
		slogln(String(header.frameHeight));
		slog("subType:");
		slogln(String(header.subType));
		slog("firstChar:");
		slogln(String(header.firstChar));
		slog("animationDelay:");
		slogln(String(header.animationDelay));
	}

	void slogSysInfo() {
		slogln("System config:");
		slog("Vcc: ");
		slogln(String(ESP.getVcc()));
		slog("Free heap: ");
		slogln(String(ESP.getFreeHeap()));
		slog("Chip ID: ");
		slogln(String(ESP.getChipId()));
		slog("SDK version: ");
		slogln(String(ESP.getSdkVersion()));
		slog("Boot version: ");
		slogln(String(ESP.getBootVersion()));
		slog("Boot mode: ");
		slogln(String(ESP.getBootMode()));
		slog("CPU freq.: ");
		slogln(String(ESP.getCpuFreqMHz()));
		slog("Flash chip ID: ");
		slogln(String(ESP.getFlashChipId(), HEX));
		// // gets the actual chip size based on the flash id
		slog("Flash real size: ");
		slogln(String(ESP.getFlashChipRealSize()));
		slog("Flash real size (method b): ");
		slogln(String(ESP.getFlashChipSizeByChipId()));
		// // gets the size of the flash as set by the compiler
		slog("flash configured size: ");
		slogln(String(ESP.getFlashChipSize()));
		if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize())
		{
		  slogln(String("WARNING: configured flash size does not match real flash size!"));
		}
		slog("flash speed: ");
		slogln(String(ESP.getFlashChipSpeed()));
		slog("flash mode: ");
		slogln(String(ESP.getFlashChipMode()));
		slog("Sketch size: ");
		slogln(String(ESP.getSketchSize()));
		slog("Free sketch space: ");
		slogln(String(ESP.getFreeSketchSpace()));

		slog("uploadfile: ");
		slogln(msGlobals.ggUploadFileName);

		FSInfo linfo;
		SPIFFS.info(linfo);

		slog("linfo.blockSize =");
		slogln(String(linfo.blockSize));
		slog("linfo.pageSize =");
		slogln(String(linfo.pageSize));
		slog("linfo.maxOpenFiles =");
		slogln(String(linfo.maxOpenFiles));
		slog("linfo.maxPathLength =");
		slogln(String(linfo.maxPathLength));
		slog("linfo.totalBytes =");
		slogln(String(linfo.totalBytes));
		slog("linfo.usedBytes = ");
		slogln(String(linfo.usedBytes));

		slog("Reset info: ");
		slogln(String(ESP.getResetInfo()));

		slog("timeoutHighPower: ");
		slogln(String(msGlobals.ggUIConfig.timeoutHighPower));
		slog("timeoutLowPower: ");
		slogln(String(msGlobals.ggUIConfig.timeoutLowPower));
		slog("defaultBrightness: ");
		slogln(String(msGlobals.ggUIConfig.defaultBrightness));
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
	void setMode(int newMode)
	{
		if (newMode < msGlobals.ggModeList.size()) {
			
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
	void brightnessControl() {
		int newIdx = msGlobals.ggBrightness;
		float avgV = 0;
		int newV = 0, lastV = -1;
		uint16_t blink = 0;
		int skip = 100;

		// state: on
		if ((msButtons.powerButtonPressed()) &&
			(msButtons.msBtnPwrPressTime > BRIGHTNESS_CONTROL_TIME)) {

			slogln("brightnesscontrol EVENT");

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

// #ifdef CONFIG_ENABLE_MIDI
// #warning "MIDI has been enabled - Serial I/O at 31250 - serial logging disabled (use wlan)"
		// Serial.begin(31250);
// #else
		Serial.begin(921600);
// #endif

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

		initPowerButtonTimer();

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


	void setLocalYieldState(bool state)
	{
		_shouldLocalYield = state;
	}


	void step() {

		// this is needed, as we have timers and stuff.
		yield();

		msGlobals.ggLastMicros = msGlobals.ggCurrentMicros;
		msGlobals.ggCurrentMicros = micros();
		msGlobals.ggCurrentMillis = millis();
		
		displayButtons();

		// getADValue(); // !J! we use timer

		brightnessControl();

		msButtons.step();
		msSensor.step();

		batteryVoltage = calculateVoltage(msGlobals.ggLastADValue, msGlobals.batVoltCalibration);

		if (batteryVoltage < 4.0) {
			if (lowBatteryMillis == 0)
				lowBatteryMillis = msGlobals.ggCurrentMillis;
		}
		else
		{
			lowBatteryMillis = 0;
		}

		if (msButtons.msBtnActive) {
			msPowerCountDown = msGlobals.ggCurrentMillis;
		}

		if (msGlobals.ggUIConfig.timeoutHighPower != 0) {
			// note! unsigned values!
			if (msPowerCountDown + msGlobals.ggUIConfig.timeoutHighPower < msGlobals.ggCurrentMillis) {
				powerDown();
			}
		}

		if ((lowBatteryMillis != 0) && 
			(msGlobals.ggUIConfig.timeoutLowPower != 0) && 
			(lowBatteryMillis + (10 * 1000) < msGlobals.ggCurrentMillis)) { // 10 seconds 
			// note! unsigned values!
			if (msPowerCountDown + msGlobals.ggUIConfig.timeoutLowPower < msGlobals.ggCurrentMillis) {
				powerDown();
			}
		}

		if (msButtons.checkMenueEnterCondition()) {
			if (!modeMenuActivated) {
				modeMenuActivated = true;
				feedbackAnimation(msGlobals.feedbackType::MODE_MENU);
			}
		}

		CommandInterfacePoll();

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
			if (x % 4 == 0)
				slogln("");
			// slog(":");
			Serial.print(buf[x], HEX);;
		}
		slogln("<<EOF");
	}

};

#include "CommandInterface.h"


#else
extern MagicShifterSystem msSystem;
#endif
