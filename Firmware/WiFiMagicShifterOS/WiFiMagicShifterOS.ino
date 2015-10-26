/*
 * MagicShifter3000 OS
 * Copyright (c) wizards@Work
 */

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
#include <SPI.h>

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

#include "Util/StringURL.h"

#include "msConfig.h"

#include "WebServer/WebServer.h"

#include "msGlobals.h"
#include "msSystem.h"

MidiShifterGlobals msGlobals;
MidiShifterSystem msSystem;

// mode modules 
#ifdef MIDISHIFTER
#include "MidiShifter/MidiShifterModes.h"
#endif
#include "Modes/BouncingBall.h"
BouncingBallMode msBouncingBallMode(600);

#include "Modes/MagicShake.h"
MagicShakeMode msShakeMode;

//POVShakeSync msPOVShakeSyncMode;
POVShakeSyncDummyMode msPOVShakeSyncMode;


void setup()
{
  msSystem.setup();
  msGlobals.bootTime = millis();

  msSystem.logSysInfo();

  if (SPIFFS.begin()) {
    msSystem.logln("SPIFFS begin!");
  }
  else
  {
    msSystem.TEST_SPIFFS_bug();
    msSystem.logln("SPIFFS not begin .. :(");
  }

#ifndef DISABLE_ACCEL
  resetAccelerometer(); //Test and intialize the MMA8452
#endif

  StartWebServer();

  // start Modes as necessary ..
  loadString(msGlobals.uploadFileName, FILENAME_LENGTH);
  if (!SPIFFS.exists(msGlobals.uploadFileName))
  {
    msSystem.log("could not find: ");
    msSystem.logln(msGlobals.uploadFileName);
    // strcpy(msGlobals.uploadFileName, "big_smile_gif.magicBitmap");
  }
  else {
  msSystem.log("using POV file: ");
  msSystem.logln(msGlobals.uploadFileName);
    msShakeMode.start();
  }

  // debug output so we know we're alive in case a mode dies ..
  for (byte idx = 0; idx < LEDS; idx++)
  {
    setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
  }
  updatePixels();

#if 0 
//saveBuffer(msGlobals.web_rgb_buffer);
// TEST CODE
while (0)
{
  float voltage = msSystem.getBatteryVoltage();

  msSystem.log(voltage);
  msSystem.logln("V");

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

   // swipe colors
  for (byte idx = 0; idx < LEDS; idx++)
  {
    setPixel(idx, (idx & 1) ? bbb : 0, (idx & 2) ? bbb : 0, (idx & 4) ? bbb : 0, 0);
    updatePixels();
    delay(20);
    msGlobals.msSystem.getBatteryVoltage();
  }
}
 // delay(1000)

while (1)
{
  // swipe colors
    for (byte idx = 0; idx < LEDS; idx++)
    {
      setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, msGlobals.GLOBAL_GS);
      updatePixels();
      delay(30);
    }
    for (byte idx = 0; idx < LEDS; idx++)
    {
      setPixel(idx, 0, 0, 0, 1);
      updatePixels();
      delay(30);
    }
}
  
while (0)
{
  MSImage activeImage = MSImage(msGlobals.uploadFileName);
  msSystem.log("loaded: ");
  msSystem.logln(msGlobals.uploadFileName);

  msSystem.log("width: ");
  msSystem.logln(activeImage.getWidth());

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

#endif
}

void loop()
{
  msGlobals.lastMicros = msGlobals.currentMicros;
  msGlobals.currentMicros = micros();
  msGlobals.loops++;

  pinMode(PIN_BUTTON_B, INPUT);

  msSystem.loop();

  HandleWebServer();

  msPOVShakeSyncMode.setFrames(32);

  if (msGlobals.loops % 1000 == 0)
  {
    msSystem.log("_");
  }

  if (msGlobals.lastFrameMicros + msGlobals.speedMicros < msGlobals.currentMicros)
  {
    msGlobals.lastFrameMicros = msGlobals.currentMicros;
    msGlobals.currentFrame++;

    // pov msBouncingBallMode mode
    if (msGlobals.shifterMode == 0)
    {
      { 
        for (byte idx = 0; idx < LEDS; idx++)
        {
          float scale = msBouncingBallMode.getLedBright(idx, LEDS);

          scale *= 0.5;

          /*if (msBouncingBallMode.allowFlash && msBouncingBallMode.smoothLanding)
          {
          }
          else
          {
            scale *= 0.25;
          }
          */

         //  msGlobals.bright = 1;
          //scale *= 10;
          //setPixel(idx, (msGlobals.currentFrame & 1) ? msGlobals.bright*scale : 0, (msGlobals.currentFrame & 2) ? msGlobals.bright*scale : 0, (msGlobals.currentFrame & 4) ? msGlobals.bright*scale : 0, msGlobals.gs);
          
          if (msBouncingBallMode.allowFlash)
          {
            if (msBouncingBallMode.smoothLanding)
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
      msShakeMode.step();
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

  //msBouncingBallMode.applyForce((msGlobals.currentMicros - msGlobals.lastMicros) / 1000.0, fX, fY);
  msBouncingBallMode.applyForce((msGlobals.currentMicros - msGlobals.lastMicros) / 1000.0, fX);
}
