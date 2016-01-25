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
#include "Buttons.h"

// Power management
#define PIN_PWR_MGT 16

void USBPoll();

// TODO: all init and all sensors and leds in here :)
// (accelerometer wuld also be a class but the MAgicShifter object has one ;)
class MagicShifterSystem
{
private:

public:

  // todo: protect
  MagicShifterAccelerometer msAccel;
  MagicShifterLEDs msLEDs;
  MagicShifterEEPROMString  msEEPROMs;
  MagicShifterButtons msButtons;
  MDNSResponder msDNS;
  ESP8266WebServer msESPServer;
  WiFiUDP msUDP;

  int msFrame = 0;
  bool msAccelOK = false;
  long msPowerCountDown = 0;

public:
  void log(String msg) { 
  // todo:switch log from OFF, to BANNED (MIDI), to UDP .. etc.
    Serial.print(msg); 
    msUDP.beginPacket("192.168.1.112", 514); // wks port for syslog
    msUDP.print(msg);
    msUDP.endPacket();
  };

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
    uint8_t testVals[] = {1, 23, 3, 7};
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
    logln("System config:");
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

    log("uploadfile: "); logln(msGlobals.ggUploadFileName);


    // log("Reset info: ");
    // logln(String(ESP.getResetInfo()));
    //log("FS mount: ");
    //logln(String(FS.mount() ? "OK" : "ERROR!"));
    
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


  // reset the power controller
  void powerStabilize()
  {
    digitalWrite(PIN_PWR_MGT, HIGH);
    pinMode(PIN_PWR_MGT, OUTPUT);
    digitalWrite(PIN_PWR_MGT, HIGH);
  }

  // tell power controller to power down
  void powerDown()
  {
    // works even with pulldown but output seems to make more sense
    //pinMode(PIN_PWR_MGT, INPUT_PULLDOWN);
    pinMode(PIN_PWR_MGT, OUTPUT);
    digitalWrite(PIN_PWR_MGT, LOW);
    // now sleep forever...
    delay(1000);
  }

  // for fail-modes ..
  void infinite_swipe()
  { 
    while (1)
    {
      // swipe colors
      for (byte idx = 0; idx < MAX_LEDS; idx++)
      {
        msLEDs.setPixels(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, msGlobals.ggFactoryIntensity);
        msLEDs.updatePixels();
        delay(30);
      }
      for (byte idx = 0; idx < MAX_LEDS; idx++)
      {
        msLEDs.setPixels(idx, 0, 0, 0, 1);
        msLEDs.updatePixels();
        delay(30);
      }
    }
  }

  // gets the basic stuff set up
  void setup()
  {
     // led controllers and buffer
    msLEDs.initLEDHardware();
    msLEDs.initLEDBuffer();
    msLEDs.bootSwipe();

// !J! todo: enable MIDI, add arp mode
// #ifdef CONFIG_ENABLE_MIDI
//     Serial.begin(31250);
// #else
    Serial.begin(115200);
// #endif

    // !J! todo: get this from factory config
    delay(700); // this enables serial consoles to sync

    // #endif
    EEPROM.begin(512);

    logln(String("\r\nMagicShifter 3000 OS V0.30"));

    // ggUploadFile is prepared for display as necessary ..
    msEEPROMs.loadString(msGlobals.ggUploadFileName, MAX_FILENAME_LENGTH);

    // wake up filesystem
    log("SPIFFS:");

    if (SPIFFS.begin()) 
      log("done:");
    else
      log("noSPIFFS:");
    // !J! todo: infinite_loop()? 
    //TEST_SPIFFS_bug();

    // all engines turn on
    pinMode(PIN_PWR_MGT, INPUT);
    pinMode(PIN_LED_ENABLE, INPUT);

    // reset power controller to stay on
    // had some power down troubles so this needs to be further investigated
    powerStabilize();
    // !J! todo: power-management module 

    msButtons.setup();

#ifdef CONFIG_ENABLE_ACCEL
    // accelerometer 
    msAccel.initAccelerometer();
    msAccelOK = msAccel.resetAccelerometer(); //Test and intialize the MMA8452
#endif

    // led controllers and buffer
    msLEDs.initLEDHardware();
    msLEDs.initLEDBuffer();
    //msLEDs.bootSwipe();

    // boot that we are alive

    // // I2C test:
    // if (!Wire.available() ) {
    //   infinite_swipe(); // todo: explain to user: please reset device
    // }

    logSysInfo();

  }

  void restart()
  {
    ESP.restart();
  }

  void loop()
  {

    msButtons.loop();
    // internal button usage
    if (msButtons.msBtnPwrLongHit)
    {
      powerDown();
    }

    if (msButtons.msBtnActive) {
      msPowerCountDown = msGlobals.ggCurrentMicros;
    }

    if (msPowerCountDown < msGlobals.ggCurrentMicros - POWER_TIMEOUT) {
      powerDown();
    }

    if (msButtons.msBtnBHit)
    {
      msGlobals.ggCurrentMode = (msGlobals.ggCurrentMode+1)%NUM_MS_MODES;
      msButtons.msBtnBHit = false;
      log("Changed +Mode:"); logln(String(msGlobals.ggCurrentMode));
    }
    else
    if (msButtons.msBtnAHit)
    {
      msGlobals.ggCurrentMode = (msGlobals.ggCurrentMode-1)%NUM_MS_MODES;
      msButtons.msBtnBHit = false;
      log("Changed -Mode:"); logln(String(msGlobals.ggCurrentMode));
    }
    
    USBPoll();
  }


  void enableLongClicks(bool enable)
  {
    msButtons.msLongClickOK = enable;
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

  void hexDump(int len, byte *buf, const char*label) {
    log(label); 
    log(String(len)); 
    log("/");

    for (int x=0;x<len;x++) {
      if (x % 4 == 0) logln("");
      log(":"); Serial.print(buf[x], HEX);; 
    }
    logln("<<EOF");
  }


};

#include "USBCommandInterface.h"

#else
extern MagicShifterSystem msSystem;
#endif
