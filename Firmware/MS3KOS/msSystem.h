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

extern "C" {
#include "osapi.h"
#include "ets_sys.h"
#include "mem.h"
#include "user_interface.h"
}

#include <Base64.h>
#include <EEPROM.h>
#include <SPI.h>

#include "Hardware/EEPROMString.h"
#include "Hardware/LEDHardware.h"
#include "Hardware/Accelerometer.h"

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

  int msFrame = 0;

  bool msAccelOK = false;

// TODO: private state
// state for button timing
  int buttonAPressedTime = 0;
  int msBtnPwrPressTime = 0;
  int msBtnBPressTime = 0;

// state for double click timing
  int msBtnATTL = 0;
  int msBtnPwrTTL = 0;
  int msBtnBTTL = 0;

  bool msLongClickOK = true;

// todo public properties? Logic for consuming buttons?
// events for consumers true/false;
  bool msBtnAHit = false;
  bool msBtnPwrHit = false;
  bool msBtnBHit = false;

  bool msBtnALongHit = false;
  bool msBtnPwrLongHit = false;
  bool msBtnBLongHit = false;
  
  bool msBtnADoubleHit = false;
  bool msBtnPwrDoubleHit = false;
  bool msBtnBDoubleHit = false;

  MDNSResponder msDNS;
  ESP8266WebServer msESPWebServer;

public:

  // wrap a logging class ..
  void log(String msg)
  {
    Serial.print(String(msg));
    // #define DEBUGV(...) ets_printf(__VA_ARGS__)

Serial.print("\n------\n");
    ets_printf("This was: %s\n", "ets_printf");

//#endif
  };

  void log(int8_t &msg, int base) { log(String(msg)); } 
  void log(uint16_t &msg, int base) { log(String(msg));  }
  void log(unsigned int msg, int base) { log(String(msg)); } 
  void log(bool b) { log(String(b));}

  void logln(String msg) { log(msg); log("\r\n");  };
  
  void logln(int8_t &msg, int base) { logln(String(msg)); }
  void logln(uint16_t &msg, int base) { logln(String(msg));  }
  void logln(unsigned int msg, int base) { logln(String(msg));  }
  void logln(bool &msg, int base) { logln(String(msg));  }


  void TEST_SPIFFS_bug()
  {

    const char* debugPath = "XXXXX";
    uint8_t testVals[] = {1,23, 3, 7};
    uint8_t readBuffer[] = {0,0,0,0};
    //File file = SPIFFS.open((char *)debugPath.c_str(), "w");
    
    logln("openin for w: ");
    logln(String(debugPath));
    
    File file = SPIFFS.open(debugPath, "w");

    logln("opended for w: ");
    logln(String((bool)file));

    logln("writin: ");
    logln(String(testVals[1]));

    file.write((uint8_t *)testVals, sizeof testVals);
    file.close();

    logln("openin for r: ");
    logln(String(debugPath));
    
    File fileR = SPIFFS.open(debugPath, "r");

    logln("opended for r: ");
    logln(String((bool)fileR));

    logln("readin: ");

    fileR.read((uint8_t *)readBuffer, sizeof readBuffer);
    fileR.close();

    logln("readback: ");
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

/*
      TEST_SPIFFS_bug();
*/

    // all engines turn on
    pinMode(PIN_PWR_MGT, INPUT);
    pinMode(PIN_LED_ENABLE, INPUT);

    // init pin modes
    pinMode(PIN_BUTTON_A, INPUT);
    pinMode(PIN_BUTTON_B, INPUT);


#ifdef CONFIG_ENABLE_ACCEL
    // accelerometer 
    msAccel.initAccelerometer();
    msAccelOK = msAccel.resetAccelerometer(); //Test and intialize the MMA8452
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
    msBtnAHit = msBtnALongHit = false;
    if (!digitalRead(PIN_BUTTON_A))
    {
      Serial.println("A PRESSED");

      if (buttonAPressedTime)
        buttonAPressedTime += msGlobals.ggLastMicros;
      else
        buttonAPressedTime = 1;
    }
    else
    {
      if (msLongClickOK && buttonAPressedTime >= MIN_TIME_LONG_CLICK)
      {
        msBtnALongHit = true;
        Serial.println("msBtnALongHit");
      }
      else if (buttonAPressedTime >= MIN_TIME_CLICK)
      {
        msBtnAHit = true;
        Serial.println("msBtnAHit");
      }

      buttonAPressedTime = 0;
    }

    // reset public btton state
    msBtnBHit = msBtnBLongHit = false;
    if (!digitalRead(PIN_BUTTON_B))
    {
      Serial.println("B PRESSED");

      if (msBtnBPressTime)
        msBtnBPressTime += msGlobals.ggLastMicros;
      else
        msBtnBPressTime = 1;
    }
    else
    {
      if (msLongClickOK && msBtnBPressTime >= MIN_TIME_LONG_CLICK)
      {
        msBtnBLongHit = true;
        Serial.println("msBtnBLongHit");

      }
      else if (msBtnBPressTime >= MIN_TIME_CLICK)
      {
        msBtnBHit = true;
        Serial.println("msBtnBHit");
      }

      msBtnBPressTime = 0;
    }


    // reset public btton state
    msBtnPwrHit = msBtnPwrLongHit = false;

    //if (msFrame++ % 10 == 0)
    if (powerButtonPressed())
    {
      Serial.println("POWER PRESSED");

      if (msBtnPwrPressTime)
        msBtnPwrPressTime += msGlobals.ggLastMicros;
      else
        msBtnPwrPressTime = 1;
    }
    else
    {
      if (msBtnPwrPressTime >= MIN_TIME_LONG_CLICK)
      {
        msBtnPwrLongHit = true;
      }
      else if (msBtnPwrPressTime >= MIN_TIME_CLICK)
      {
        msBtnPwrHit = true;
      }

      msBtnPwrPressTime = 0;
    }
    //*/

    // internal button usage
    if (msBtnALongHit)
    {
      powerDown();
    }

    if (msBtnBHit)
    {
      msGlobals.ggGS+=2;
      if (msGlobals.ggGS > 31)
      {
        msGlobals.ggGS = 31;
      }

      Serial.println("cB");

      msGlobals.ggCurrentMode = (msGlobals.ggCurrentMode+1)%NUM_MS_MODES;
    }
    if (msBtnBLongHit)
    {
      msGlobals.ggGS-=6;
      if (msGlobals.ggGS < 1)
      {
        msGlobals.ggGS = 1;
      }

      Serial.println("looooooong clickedB");

      msGlobals.ggCurrentMode = (msGlobals.ggCurrentMode+1)%NUM_MS_MODES;
    }
  }

  void enableLongClicks(bool enable)
  {
    msLongClickOK = enable;
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
    if (msGlobals.ggModeAP)
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
