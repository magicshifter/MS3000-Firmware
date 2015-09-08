/* NOTES

 * This is the MIDIShifter branch, which is set up to add full MIDI
   capabilities to the MagicShifter.  We use MagicShifter3000 modules,
   but will proceed rapidly to tighten/tidy things up a bit..

 * At the moment, we depend on a hack to FS.h/FS.cpp, which is not 
    upstream .. hacked by wizard23

    The hack is:
  bool FS::exists(const String& path) {
      return exists(path.c_str());
  } 

  bool FS::exists(const char* path) {
     File f = open(path, "r");
     if (f)
     {
      return true;
     }
     else
     {
      return false;
     }
  }

 * To prevent restarts: Make sure GPIO 2 is not connected, i.e. float.
      from https://github.com/esp8266/Arduino/issues/373


 * TODO:
      !J! Construct msGlobals &etc.
      !J! Make execution environment changes (i.e. main_loop() -> app_loop())
      !J! Add single MagicShifter:: API: LEDs, MIDI, Web-UI, Files, etc.
      !J! Port existing MagicShifter UI apps to new app_loop() and API
      */

//#include <Ticker.h>
#include <math.h>
#include <Wire.h> // Used for I2C
#include <Arduino.h>
#include <FS.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Base64.h>
#include <EEPROM.h>

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
#include <math.h>	/* for HUGE_VAL */

//compiler.c.elf.libs=-lm -lgcc -lhal -lphy -lnet80211 -llwip -lwpa -lmain -lpp -lsmartconfig -lc -ljson
extern "C" {
  #include "json/json.h"
  #include <json/jsonparse.h>
  #include <json/jsontree.h>
}

#include "tools.h"
#include "Config.h"
#include "APA102.h"
#include "MMA8542.h"
#include "WebServer.h"

#include "MagicShifterGlobals.h"
#include "MagicShifterSystem.h"

MagicShifterGlobals msGlobals;
MagicShifterSystem msSystem;

#include "MagicMode.h"

#ifdef MIDISHIFTER
#include "MidiShifter/MidiShifter.h"
#endif

#include "CircleBall.h"

MagicMode magicMode;
DebugMode debugMode;
//POVShakeSync shakeSync;
POVShakeSyncDummy shakeSync;
//CircleBall aBouncingBall(600);
BouncingBall aBouncingBall(600);


void log(String msg, int level = ERROR)
{
  if (msGlobals.DEBUG_LEVEL <= level)
  {
    #ifdef DEBUG_SERIAL
    Serial.print(msg)
    #endif

// !J!
#define DEBUG_SYSLOG 0
#ifdef DEBUG_SYSLOG
WiFiUDP udp;
#define __SYSLOG_PORT 514
    // udp.beginPacket("192.168.43.151", __SYSLOG_PORT); //NTP requests are to port 123
    udp.beginPacket("192.168.0.24", __SYSLOG_PORT); //NTP requests are to port 123
    udp.print(msg);
    udp.endPacket();
    #endif
  }
}

void logln(String msg, int level = ERROR)
{
  if (msGlobals.DEBUG_LEVEL <= level)
  Serial.println(msg);
}


void TEST_SPIFFS_bug()
{

  const char* debugPath = "XXXXX";
  uint8_t testVals[] = {1,23, 3, 7};
  uint8_t readBuffer[] = {0,0,0,0};
  //File file = SPIFFS.open((char *)debugPath.c_str(), "w");
  
  Serial.print("openin for w: ");
  Serial.println(debugPath);
  
  File file = SPIFFS.open(debugPath, "w");

  Serial.print("opended for w: ");
  Serial.println((bool)file);

  Serial.print("writin: ");
  Serial.println(testVals[1]);

  file.write((uint8_t *)testVals, sizeof testVals);
  file.close();

  Serial.print("openin for r: ");
  Serial.println(debugPath);
  
  File fileR = SPIFFS.open(debugPath, "r");

  Serial.print("opended for r: ");
  Serial.println((bool)fileR);

  Serial.print("readin: ");

  fileR.read((uint8_t *)readBuffer, sizeof readBuffer);
  fileR.close();

  Serial.print("readback: ");
  Serial.println(readBuffer[1]);

}

void setup()
{
  msSystem.setup();
  


  msGlobals.bootTime = millis();


  // DUMP sysinfo
  Serial.print("Vcc: ");
  Serial.println(ESP.getVcc());
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId());
  Serial.print("SDK version: ");
  Serial.println(ESP.getSdkVersion());
  Serial.print("Boot version: ");
  Serial.println(ESP.getBootVersion());
  Serial.print("Boot mode: ");
  Serial.println(ESP.getBootMode());
  Serial.print("CPU freq.: ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.print("Flash chip ID: ");
  Serial.println(ESP.getFlashChipId(), HEX);
  //gets the actual chip size based on the flash id
  Serial.print("Flash real size: ");
  Serial.println(ESP.getFlashChipRealSize());
  Serial.print("Flash real size (method b): ");
  Serial.println(ESP.getFlashChipSizeByChipId());
  //gets the size of the flash as set by the compiler
  Serial.print("flash configured size: ");
  Serial.println(ESP.getFlashChipSize());
  if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize())
  {
    Serial.println("WARNING: configured flash size does not match real flash size!");
  }
  Serial.print("flash speed: ");
  Serial.println(ESP.getFlashChipSpeed());
  Serial.print("flash mode: ");
  Serial.println(ESP.getFlashChipMode());
  Serial.print("Sketch size: ");
  Serial.println(ESP.getSketchSize());
  Serial.print("Free sketch space: ");
  Serial.println(ESP.getFreeSketchSpace());
  Serial.print("Reset info: ");
  Serial.println(ESP.getResetInfo());

  if (SPIFFS.begin()) 
  Serial.println("SPIFFS begin!");
  else
  Serial.println("SPIFFS not begin .. :(");

// TEST_SPIFFS_bug();



  //Serial.print("FS mount: ");
  //Serial.println(FS.mount() ? "OK" : "ERROR!");
  
  // chercking crashes the ESP so its disabled atm
  //Serial.print("FS check: ");
  //Serial.println(FS.check() ? "OK" : "ERROR!");


  #ifndef DISABLE_ACCEL
  InitMMA8452(); //Test and intialize the MMA8452
  #endif

  StartWebServer();

  loadString(msGlobals.uploadFileName, FILENAME_LENGTH);
  //if (!FS.exists(msGlobals.uploadFileName))
  {
    Serial.print("could not find: ");
    Serial.println(msGlobals.uploadFileName);
    strcpy(msGlobals.uploadFileName, "big_smile_gif.magicBitmap");
  }
  Serial.print("using POV file: ");
  Serial.println(msGlobals.uploadFileName);
  magicMode.start(&msSystem);
  magicMode.setActiveFile(msGlobals.uploadFileName);

///*
for (byte idx = 0; idx < LEDS; idx++)
{
  setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
}
updatePixels();
  //saveBuffer(msGlobals.web_rgb_buffer);
//*/

while (0)
{
  float voltage = msSystem.getBatteryVoltage();

  Serial.print(voltage);
  Serial.println("V");

  for (int i = 0; i < 10; i ++)
  {
    msSystem.getBatteryVoltage();
    delay(1); 
  }

  int bbb = 255;

  for (byte idx = 0; idx < LEDS; idx++)
  {
    setPixel(idx, ((idx % 3)  == 0) ? bbb : 0, ((idx  % 3) == 1 ) ? bbb : 0, ((idx %  3) == 2) ? bbb : 0, 0);

    setPixel((LEDS + idx - 16)%LEDS, 0, 0, 0, 0);
    updatePixels();
    delay(100);
    msSystem.getBatteryVoltage();
  }

  delay(1);

/*
     // swipe colors
    for (byte idx = 0; idx < LEDS; idx++)
    {
      setPixel(idx, (idx & 1) ? bbb : 0, (idx & 2) ? bbb : 0, (idx & 4) ? bbb : 0, 0);
      updatePixels();
      delay(20);
      msGlobals.msSystem.getBatteryVoltage();
    }
    */  

  }
  /*



 // delay(1000)

// while (1)
// {
//   // swipe colors
//     for (byte idx = 0; idx < LEDS; idx++)
//     {
//       setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, msGlobals.GLOBAL_GS);
//       updatePixels();
//       delay(30);
//     }
//     for (byte idx = 0; idx < LEDS; idx++)
//     {
//       setPixel(idx, 0, 0, 0, 1);
//       updatePixels();
//       delay(30);
//     }
// }
  

  while (0)
  {
    MSImage activeImage = MSImage(msGlobals.uploadFileName);
    Serial.print("loaded: ");
    Serial.println(msGlobals.uploadFileName);

    Serial.print("width: ");
    Serial.println(activeImage.getWidth());

    for (int i = 0; i < activeImage.getWidth(); i++)
    {
      byte povData[RGB_BUFFER_SIZE];
      activeImage.readFrame(i, povData, RGB_BUFFER_SIZE);
      loadBuffer(povData);
      updatePixels();
      delay(1);
    }

    activeImage.close();
    
  } 
  */
}

void loop()
{
  pinMode(PIN_BUTTON_B, INPUT);
  msSystem.loop();

  HandleWebServer();

  msGlobals.lastMicros = msGlobals.currentMicros;
  msGlobals.currentMicros = micros();
  msGlobals.loops++;

  shakeSync.setFrames(32);

  if (msGlobals.loops % 1000 == 0)
  {
    Serial.print("_");
  }

  if (msGlobals.lastFrameMicros + msGlobals.speedMicros < msGlobals.currentMicros)
  {
    msGlobals.lastFrameMicros = msGlobals.currentMicros;
    msGlobals.currentFrame++;

    // pov aBouncingBall mode
    if (msGlobals.shifterMode == 0)
    {
      {
        for (byte idx = 0; idx < LEDS; idx++)
        {
          float scale = aBouncingBall.getLedBright(idx, LEDS);

          scale *= 0.5;

          /*if (aBouncingBall.allowFlash && aBouncingBall.smoothLanding)
          {


          }
          else
          {
            scale *= 0.25;
          }
          */

         // int msGlobals.bright = 1;
          //scale *= 10;
          //setPixel(idx, (msGlobals.currentFrame & 1) ? msGlobals.bright*scale : 0, (msGlobals.currentFrame & 2) ? msGlobals.bright*scale : 0, (msGlobals.currentFrame & 4) ? msGlobals.bright*scale : 0, msGlobals.gs);
          
          if (aBouncingBall.allowFlash)
          {
            if (aBouncingBall.smoothLanding)
            {
              setPixel(idx, 0, msGlobals.bright * scale, 0, msGlobals.GLOBAL_GS);
            }
            else
            {
              setPixel(idx, msGlobals.bright * scale, msGlobals.bright * scale, msGlobals.bright * scale, msGlobals.GLOBAL_GS);
            }
          }
          else
          {  
            setPixel(idx, msGlobals.bright * scale, 0, 0.5 * msGlobals.bright * scale, msGlobals.GLOBAL_GS);
          }
        }
      }
      updatePixels();
    }
    else if (msGlobals.shifterMode == 1)
    {
      loadBuffer(msGlobals.web_rgb_buffer);
      updatePixels();
    }
    else if (msGlobals.shifterMode == 2)
    {
      magicMode.loop();
    }
  }

  #ifndef DISABLE_ACCEL
  readAccelData(msGlobals.accelCount);

  for (int i = 0 ; i < 3 ; i++)
  {
    msGlobals.accelG[i] = (float) msGlobals.accelCount[i] / ((1 << 12) / (2 * GSCALE)); // get actual g value, this depends on scale being set
  }
  #endif

  float fX = msGlobals.accelG[0];
  float fY = msGlobals.accelG[1];

  //aBouncingBall.applyForce((msGlobals.currentMicros - msGlobals.lastMicros) / 1000.0, fX, fY);
  aBouncingBall.applyForce((msGlobals.currentMicros - msGlobals.lastMicros) / 1000.0, fX);
}
