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

#include "Hardware/EEPROMString.h"
#include "Hardware/LEDHardware.h"
#include "Hardware/Sensor.h"
#include "Hardware/Buttons.h"

#include "msImage.h"


// forward-declared here because it is a client of msSystem ..
void CommandInterfacePoll();

class MagicShifterSystem;

// TODO: all init and all sensors and leds in here :)
// (accelerometer wuld also be a class but the MAgicShifter object has one ;)
class MagicShifterSystem
{
private:

public:
  // todo: protect
  MagicShifterAccelerometer msSensor;
  MagicShifterLEDs msLEDs;
  MagicShifterEEPROMString  msEEPROMs;
  MagicShifterButtons msButtons;
  MDNSResponder msDNS;
  ESP8266WebServer msESPServer;
  WiFiUDP msUDP;

  int msFrame = 0;
  bool msSensorOK = false;
  long msPowerCountDown = 0;

public:
  void log(String msg) { 
  // todo:switch log from OFF, to BANNED (MIDI), to UDP .. etc.
    Serial.print(msg); 
    // Serial.print("UDP:");    
    // Serial.print(String(msUDP.beginPacket("192.168.1.112", 514))); // wks port for syslog
    // msUDP.print(msg);
    // msUDP.endPacket();
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

  void dumpActiveHeader(const MSBitmapHeader& header)
  {
    logln("Header dump:");
    log("fileSize:"); logln(String(header.fileSize));
    log("pixelFormat:"); logln(String(header.pixelFormat));
    log("maxFrame:"); logln(String(header.maxFrame));
    log("frameWidth:"); logln(String(header.frameWidth));
    log("frameHeight:"); logln(String(header.frameHeight));
    log("subType:"); logln(String(header.subType));
    log("firstChar:"); logln(String(header.firstChar));
    log("animationDelay:"); logln(String(header.animationDelay));
  }

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
    showBatteryStatus(false);

    // works even with pulldown but output seems to make more sense
    //pinMode(PIN_PWR_MGT, INPUT_PULLDOWN);
    pinMode(PIN_PWR_MGT, OUTPUT);
    digitalWrite(PIN_PWR_MGT, LOW);
    // now sleep forever...
    delay(1000);
  }

  void modeAnimation()
  {
    for (int i=0;i<=3;i++)
    {
      msLEDs.fillLEDs(255, 255, 255, msGlobals.ggBrightness);
      msLEDs.updateLEDs();
      delay(35);
      msLEDs.fastClear();
      delay(35);
    }

    msLEDs.setLED(msGlobals.ggCurrentMode, 128, 128, 128, msGlobals.ggBrightness / 2);
    delay(35);
    msLEDs.fastClear();

  }

  // for fail-modes ..
  void infinite_swipe()
  { 
    while (1)
    {
      // swipe colors
      for (byte idx = 0; idx < MAX_LEDS; idx++)
      {
        msLEDs.setLED(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, msGlobals.ggBrightness);
        msLEDs.updateLEDs();
        delay(30);
      }
      for (byte idx = 0; idx < MAX_LEDS; idx++)
      {
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


  void displayButtons()
  {
    if (msButtons.msBtnPwrDoubleHit) {
      msLEDs.fillLEDs(0, 200, 0, msGlobals.ggBrightness);
      msLEDs.updateLEDs();
      delay(BUTTON_DISPLAY_PERIOD);
    }

    if (msButtons.msBtnPwrLongHit)
    {
      msLEDs.setLED(BUTTON_LED_PWR, 0, 0, 20, 20);
      msLEDs.setLED(BUTTON_LED_PWR + 1, 0, 0, 20, 20);
      msLEDs.updateLEDs();
      delay(BUTTON_DISPLAY_PERIOD);
    }
    if (msButtons.msBtnPwrHit)
    {
      msLEDs.setLED(BUTTON_LED_PWR, 20, 20, 0, 15);
      msLEDs.setLED(BUTTON_LED_PWR - 1, 20, 20, 0, 15);
      msLEDs.updateLEDs();
      delay(BUTTON_DISPLAY_PERIOD);
    }
    if (msButtons.msBtnALongHit)
    {
      msLEDs.setLED(BUTTON_LED_A, 20, 0, 20, 20);
      msLEDs.updateLEDs();
      delay(BUTTON_DISPLAY_PERIOD);
    }
    if (msButtons.msBtnAHit)
    {
      msLEDs.setLED(BUTTON_LED_A, 20, 20, 0, 20);
      msLEDs.updateLEDs();
      delay(BUTTON_DISPLAY_PERIOD);
    }
    if (msButtons.msBtnBLongHit)
    {
      msLEDs.setLED(BUTTON_LED_B, 20, 0, 20, 20);
      msLEDs.updateLEDs();
      delay(BUTTON_DISPLAY_PERIOD);
    }
    if (msButtons.msBtnBHit)
    {
      msLEDs.setLED(BUTTON_LED_B, 20, 20, 0, 20);
      msLEDs.updateLEDs();
      delay(BUTTON_DISPLAY_PERIOD);
    }
  }

#define BRIGHTNESS_CONTROL_TIME 600

uint8_t BrightnessLevels[16] = { 1,   2,   3, 4,
                 5,  6,  7, 8,
                  10,  12, 14, 16,
                18, 22, 26, 31};

#define BRIGHTNESS_UI_LEVEL 128

// -- brightness handling:
void brightnessControl()
{
  int newIdx = msGlobals.ggBrightness;
  float avgV = 0;
  int newV = 0, lastV = -1;
  uint16_t blink = 0;
  int skip = 100;

  // state: on
  if ((msButtons.powerButtonPressed()) && 
      (msButtons.msBtnPwrPressTime > BRIGHTNESS_CONTROL_TIME) )
  {

logln("brightnesscontrol EVENT");

    msGlobals.ggBrightness = 255;
    while (skip)
    {
      delay(1);
      
      msButtons.step();
      msSensor.step();

      if (msButtons.powerButtonPressed())
        skip = 100;
      else
      {
        if (skip >0) skip--;
      }

      // AccelPoll();

      // calculate average curve
      float fFactor = 0.96;
      avgV = (fFactor * avgV) + msGlobals.ggAccel[XAXIS] * (1 - fFactor);
      float lLEDRange=((MAX_LEDS - 1.0)/2.0);
      // calculate LED index
      newIdx = (int)((lLEDRange * 1.4) +  (lLEDRange * avgV) * 1.8);

      if (msGlobals.ggFault > 0)
        newIdx = -2;

      if (newIdx < -1) {
        newV = 0;
      }
      else
      {
        blink = 0;
        if (newIdx < 0) newIdx = 0;
        if (newIdx >= 15) newIdx = 15;
        newV = BrightnessLevels[newIdx];
      }

      if (newV == 0)
      {
        msLEDs.fillLEDs(0, 0, 0, 0);
        blink++;
        uint16_t bb = blink&0x1FF;
        if (bb > 255) bb =511-bb;
        //bb = (v*bb)/255;

        msLEDs.setLED(4, bb/8, bb/8, bb/8);
        msLEDs.setLED(5, bb/4, bb/4, bb/4);
        msLEDs.setLED(6, bb/2, bb/2, bb/2);
        msLEDs.setLED(7, bb, bb, bb);
        msLEDs.setLED(8, bb, bb, bb);
        msLEDs.setLED(9, bb/2, bb/2, bb/2);
        msLEDs.setLED(10, bb/4, bb/4, bb/4);
        msLEDs.setLED(11, bb/8, bb/8, bb/8);

        msLEDs.updateLEDs();
        delayMicroseconds(200);
      }
      else if (lastV != newV)
      {
        for (uint8_t i = 0; i <= 15; i++) {

          uint8_t lBr = BrightnessLevels[i];

          uint8_t dB = lBr;

          //if (dB <= 16) dB = 16;
          if (newV >= lBr)
            msLEDs.setLED(15 - i, BRIGHTNESS_UI_LEVEL, BRIGHTNESS_UI_LEVEL, BRIGHTNESS_UI_LEVEL, dB);
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

void WaitClearButtons()
{
  while (msButtons.powerButtonPressed()) {
    delay(1);
  }
}

void showBatteryStatus(bool shouldFadeIn)
{
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
    float iV = LIPO_DISPLAY_LOWER_LIMIT_V + (LIPO_DISPLAY_UPPER_LIMIT_V - LIPO_DISPLAY_LOWER_LIMIT_V) * (i / 16.0);

    if (batLevel > iV) {
      int red, green;
      if (iV > LIPO_DISPLAY_RED_LIMIT_V) {
        green = 255 * (iV - LIPO_DISPLAY_RED_LIMIT_V) / (LIPO_DISPLAY_UPPER_LIMIT_V - LIPO_DISPLAY_RED_LIMIT_V);
      } else
        green = 0;

      if (iV < LIPO_DISPLAY_ORANGE_LIMIT_V) {
        red = 255 * (LIPO_DISPLAY_ORANGE_LIMIT_V - iV) / (LIPO_DISPLAY_ORANGE_LIMIT_V - LIPO_DISPLAY_LOWER_LIMIT_V);
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
    // TEST_SPIFFS_bug();

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
    msSensor.initI2C();
    msSensorOK = msSensor.setupSensor(); //Test and intialize the MMA8452
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

    // global font objects
    MagicShifterImage::LoadBitmapBuffer("font4x5.magicFont", &msGlobals.tBitmap4x5);
    MagicShifterImage::LoadBitmapBuffer("font6x8.magicFont", &msGlobals.tBitmap6x8);
    MagicShifterImage::LoadBitmapBuffer("font7x12.magicFont", &msGlobals.tBitmap7x12);
    MagicShifterImage::LoadBitmapBuffer("font10x16.magicFont", &msGlobals.tBitmap10x16);

    logSysInfo();

  }

  void restart()
  {
    ESP.restart();
  }

  void loop()
  {

    msButtons.step();
    msSensor.step();

    displayButtons();

    if (msButtons.msBtnActive) {
      msPowerCountDown = msGlobals.ggCurrentMicros;
    }

    if (msPowerCountDown < msGlobals.ggCurrentMicros - POWER_TIMEOUT) {
      powerDown();
    }

    if (msButtons.msBtnBLongHit)
    {
      msGlobals.ggCurrentMode++;
      if (msGlobals.ggCurrentMode > (NUM_MS_MODES - 1)) // !J! todo: mode-list 
        msGlobals.ggCurrentMode=0;
      msButtons.msBtnBLongHit = false;
      modeAnimation();
      log("Changed +Mode:"); logln(String(msGlobals.ggCurrentMode));
    }

    if (msButtons.msBtnALongHit)
    {
      msGlobals.ggCurrentMode--;
      if (msGlobals.ggCurrentMode < 0) 
        msGlobals.ggCurrentMode = (NUM_MS_MODES - 1);
      msButtons.msBtnALongHit = false;
      modeAnimation();
      log("Changed -Mode:"); logln(String(msGlobals.ggCurrentMode));
    }

    if (msButtons.msBtnPwrDoubleHit)
    {
      logln("Double Power HIT!");
      brightnessControl();
      msButtons.msBtnPwrDoubleHit = false;
    }

    // if (msGlobals.allowCmd) 
    CommandInterfacePoll();

    // brightnessControl();

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
    // !J! todo: magic numbers are bad voodoo
    float r1 = 220, r2 = 820, r3 = 0; // alpha

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

#include "CommandInterface.h"

#else
extern MagicShifterSystem msSystem;
#endif
