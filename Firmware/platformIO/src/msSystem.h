#ifndef _MS_SYSTEM_H
#define _MS_SYSTEM_H

#include <math.h>
#include <Wire.h> // Used for I2C
#include <Arduino.h>
#include <Esp.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Base64.h>
#include <EEPROM.h>
#include <SPI.h>

#include "EEPROMString.h"
#include "LEDHardware.h"
#include "Accelerometer.h"

// Power management
#define PIN_PWR_MGT 16

// double features as bootloader button
#define PIN_BUTTON_A  0
#define PIN_BUTTON_B 12

// TODO: all init and all sensors and leds in here :)
// (accelerometer wuld also be a class but the MAgicShifter object has one ;)
class MagicShifterSystem
{
private:

public:

  // todo: protect
  MagicShifterAccelerometer msAccel;
  MagicShifterLEDs msLEDs;
  MagicShiftermsEEPROMsing  msEEPROMs;

  int bFrame = 0;

  bool accelerometerWorking = false;

// TODO: private state
// state for button timing
  int buttonAPressedTime = 0;
  int buttonPowerPressedTime = 0;
  int buttonBPressedTime = 0;

// state for double click timing
  int timeToLastClickedButtonA = 0;
  int timeToLastClickedButtonPower = 0;
  int timeToLastClickedButtonB = 0;

  bool m_enableLongClicks = true;

// todo public properties? Logic for consuming buttons?
// events for consumers true/false;
  bool clickedButtonA = false;
  bool clickedButtonPower = false;
  bool clickedButtonB = false;

  bool longClickedButtonA = false;
  bool longClickedButtonPower = false;
  bool longClickedButtonB = false;
  
  bool doubleClickedButtonA = false;
  bool doubleClickedButtonPower = false;
  bool doubleClickedButtonB = false;

  MDNSResponder msDNS;
  ESP8266WebServer msESPServer;

public:
  // wrap a logging class ..
  void log(String msg) { Serial.print(msg); };
  void logln(String msg) { Serial.println(msg); }; 

  void log(int8_t &msg, int base) { log(String(msg)); } 
  void log(uint16_t &msg, int base) { log(String(msg));  }
  void log(unsigned int msg, int base) { log(String(msg)); } 
//  void log(bool b) { if (b) log(String("true")); else log (String("false")); }
  
  void logln(int8_t &msg, int base) { logln(String(msg)); }
  void logln(uint16_t &msg, int base) { logln(String(msg));  }
  void logln(unsigned int msg, int base) { logln(String(msg));  }
//  void logln(bool b) { if (b) logln(String("true")); else logln(String("false")); }

  void TEST_SPIFFS_bug()
  {


    const char* debugPath = "XXXXX";
    uint8_t testVals[] = {1,23, 3, 7};
    uint8_t readBuffer[] = {0,0,0,0};
    //File file = SPIFFS.open((char *)debugPath.c_str(), "w");
    
    log("openin for w: ");
    logln(String(debugPath));
    
    File file = SPIFFS.open(debugPath, "w");

    log("opended for w: ");
    logln(String((bool)file));

    log("writin: ");
    logln(String(testVals[1]));

    file.write((uint8_t *)testVals, sizeof testVals);
    file.close();

    log("openin for r: ");
    logln(String(debugPath));
    
    File fileR = SPIFFS.open(debugPath, "r");

    log("opended for r: ");
    logln(String((bool)fileR));

    logln("readin: ");

    fileR.read((uint8_t *)readBuffer, sizeof readBuffer);
    fileR.close();

    log("readback: ");
    logln(String(readBuffer[1]));
  };

  void logSysInfo()
  {
    // // DUMP sysinfo
    log("Vcc: ");
    logln(String(ESP.getVcc()));
    log("Free heap: ");
    logln(String(ESP.getFreeHeap()));
    log("Chip ID: ");
    logln(String(ESP.getChipId()));
    log("SDK version: ");
    logln(String(ESP.getSdkVersion()));
    log("Boot version: ");
    logln(String(ESP.getBootVersion()));
    log("Boot mode: ");
    logln(String(ESP.getBootMode()));
    log("CPU freq.: ");
    logln(String(ESP.getCpuFreqMHz()));
    log("Flash chip ID: ");
    logln(String(ESP.getFlashChipId(), HEX));
    // // gets the actual chip size based on the flash id
    log("Flash real size: ");
    logln(String(ESP.getFlashChipRealSize()));
    log("Flash real size (method b): ");
    logln(String(ESP.getFlashChipSizeByChipId()));
    // // gets the size of the flash as set by the compiler
    log("flash configured size: ");
    logln(String(ESP.getFlashChipSize()));
    // if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize())
    // {
    //   logln(String("WARNING: configured flash size does not match real flash size!"));
    // }
    log("flash speed: ");
    logln(String(ESP.getFlashChipSpeed()));
    log("flash mode: ");
    logln(String(ESP.getFlashChipMode()));
    log("Sketch size: ");
    logln(String(ESP.getSketchSize()));
    log("Free sketch space: ");
    logln(String(ESP.getFreeSketchSpace()));
    log("Reset info: ");
    logln(String(ESP.getResetInfo()));
    //log("FS mount: ");
    //logln(String(FS.mount() ? "OK" : "ERROR!"));
    
    logln(String());
    
#if 0
    log(F("Heap: ")); 
    logln(String(system_get_free_heap_size()));
    log(F("Boot Vers: ")); 
    logln(String(system_get_boot_version()));
    log(F("CPU: ")); 
    logln(String(system_get_cpu_freq()));
    log(F("SDK: ")); 
    logln(String(system_get_sdk_version()));
    log(F("Chip ID: ")); 
    logln(String(system_get_chip_id()));
    log(F("Flash ID: ")); 
    logln(String(spi_flash_get_id()));
    log(F("Vcc: ")); 
    logln(String(readvdd33()));
#endif

    // chercking crashes the ESP so its disabled atm
    //log("FS check: ");
    //logln(String(FS.check() ? "OK" : "ERROR!"));

  };

  void setup()
  {


// #ifdef CONFIG_ENABLE_MIDI
//     Serial.begin(31250);
// #else
    Serial.begin(115200);
// #endif

    EEPROM.begin(512);

    logSysInfo();

    // wake up filesystem
    if (SPIFFS.begin()) 
    {
      logln("SPIFFS begin!");
    }
    else
    {
      logln("SPIFFS not begin .. :|");
    }

      TEST_SPIFFS_bug();

    // all engines turn on
    pinMode(PIN_PWR_MGT, INPUT);
    pinMode(PIN_LED_ENABLE, INPUT);

    // init pin modes
    pinMode(PIN_BUTTON_A, INPUT);
    pinMode(PIN_BUTTON_B, INPUT);


#ifdef CONFIG_ENABLE_ACCEL
    // accelerometer 
    msAccel.initAccelerometer();
    accelerometerWorking = msAccel.resetAccelerometer(); //Test and intialize the MMA8452
#endif

    // led controllers and buffer
    msLEDs.initLEDHardware();
    msLEDs.initLEDBuffer();
  // boot that we are alive
    msLEDs.bootSwipe();

    logln(String("\r\nMagicShifter 3000 OS V0.24"));
  }

  void powerDown()
  {
    // works even with pulldown but output seems to make more sense
    //pinMode(PIN_PWR_MGT, INPUT_PULLDOWN);
    pinMode(PIN_PWR_MGT, OUTPUT);
    digitalWrite(PIN_PWR_MGT, LOW);
    // now sleep forever...
    delay(1000);
  }

  void restart()
  {
    ESP.restart();
  }

  void loop()
  {

    // logln("(LOOP)");
    // handle Buttons:
  pinMode(PIN_BUTTON_A, INPUT);
  pinMode(PIN_BUTTON_B, INPUT);

    // reset public button state
    clickedButtonA = longClickedButtonA = false;
    if (!digitalRead(PIN_BUTTON_A))
    {
      Serial.println("A PRESSED");

      if (buttonAPressedTime)
        buttonAPressedTime += msGlobals.lastMicros;
      else
        buttonAPressedTime = 1;
    }
    else
    {
      if (m_enableLongClicks && buttonAPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonA = true;
        Serial.println("longClickedButtonA");
      }
      else if (buttonAPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonA = true;
        Serial.println("clickedButtonA");
      }

      buttonAPressedTime = 0;
    }

    // reset public btton state
    clickedButtonB = longClickedButtonB = false;
    if (!digitalRead(PIN_BUTTON_B))
    {
      Serial.println("B PRESSED");

      if (buttonBPressedTime)
        buttonBPressedTime += msGlobals.lastMicros;
      else
        buttonBPressedTime = 1;
    }
    else
    {
      if (m_enableLongClicks && buttonBPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonB = true;
        Serial.println("longClickedButtonB");

      }
      else if (buttonBPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonB = true;
        Serial.println("clickedButtonB");
      }

      buttonBPressedTime = 0;
    }


    // reset public btton state
    clickedButtonPower = longClickedButtonPower = false;

    //if (bFrame++ % 10 == 0)
    if (powerButtonPressed())
    {
      Serial.println("POWER PRESSED");

      if (buttonPowerPressedTime)
        buttonPowerPressedTime += msGlobals.lastMicros;
      else
        buttonPowerPressedTime = 1;
    }
    else
    {
      if (buttonPowerPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonPower = true;
      }
      else if (buttonPowerPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonPower = true;
      }

      buttonPowerPressedTime = 0;
    }
    //*/

    // internal button usage
    if (longClickedButtonA)
    {
      powerDown();
    }

    if (clickedButtonB)
    {
      msGlobals.GLOBAL_GS+=2;
      if (msGlobals.GLOBAL_GS > 31)
      {
        msGlobals.GLOBAL_GS = 31;
      }

      Serial.println("cB");

      msGlobals.shifterMode = (msGlobals.shifterMode+1)%NUM_MS_MODES;
    }
    if (longClickedButtonB)
    {
      msGlobals.GLOBAL_GS-=6;
      if (msGlobals.GLOBAL_GS < 1)
      {
        msGlobals.GLOBAL_GS = 1;
      }

      Serial.println("looooooong clickedB");

      msGlobals.shifterMode = (msGlobals.shifterMode+1)%NUM_MS_MODES;
    }
  }

  void enableLongClicks(bool enable)
  {
    m_enableLongClicks = enable;
  }

  int getADValue(void)
  {
    return analogRead(A0);
  }


  float getBatteryVoltage(void)
  {
    int adValue = getADValue();
    int ad1V = 1023;
    float avg = 3.2;

    //float r1 = 180, r2 = 390, r3 = 330; // gamma??? or (not beta)
    float r1 = 270, r2 = 1000, r3 = 0; // alpha

    float voltage = ((float)(r1 + r2 + r3) * adValue) / (r1 * ad1V);

    static float p = 0.01;
    avg = p*0.02 + voltage * (1-p);

    return avg;
  }



  bool powerButtonPressed(void)
  {
    return getADValue() > 950;
  }

  IPAddress getIP()
  {
    if (msGlobals.apMode)
    {
      return WiFi.softAPIP();
    }
    else
    {
      return WiFi.localIP();
    }
  }
};

#else
extern MagicShifterSystem msSystem;
#endif
