/*
 * MagicShifter3000 OS, Copyright (c) wizards@Work
 * Authors: wizard23(pt), seclorum(jv)
 * Notes: 
 *          All code is conflated by headers (.h):w
 *          Code prefix- is ms*, as in msConfig, msSystem, etc.
 */

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
#include <math.h> /* for HUGE_VAL */

//compiler.c.elf.libs=-lm -lgcc -lhal -lphy -lnet80211 -llwip -lwpa -lmain -lpp -lsmartconfig -lc -ljson
 extern "C" {
  #include <json/json.h>
  #include <json/jsonparse.h>
  #include <json/jsontree.h>
 }

#include <FS.h>

// note: local configuration, globals, and system objects get created now.
#include "msConfig.h"

#include "msGlobals.h"
 MagicShifterGlobals msGlobals;
// note: beyond this point, please consider the above globals.

#include "msSystem.h"
 MagicShifterSystem msSystem;
// note: WebServer and msSystem are in love
#include "WebServer.h" 
 MagicShifterWebServer msWebServer;

// MIDI can be configured on or off 
#ifdef CONFIG_ENABLE_MIDI
#include "MidiShifter.h"
#endif

// GUI modes, well actually .. modes are more of an 'app' ..
#include "Modes.h"

 MagicShakeMode msMagicShake;
 POVShakeSyncDummyMode msModePOVShake;
 POVMode msHackDemo;

// Begin MagicShifter3000 operation
 void setup()
 {
  // record our bootup time
  msGlobals.ggBootTime = millis();

delay(350); // debug !J!

  // start the system
  msSystem.setup();
  
  // get the web interface started
  msWebServer.StartWebServer();

  msSystem.msLEDs.bootSwipe();

  // initialize the modules ..
  msMagicShake.start();
  msHackDemo.start();

  // !J! set autoload
  // msMagicShake.loadShakeFile("blueghost_png.magicBitmap");
  // todo: move to module start sequence.
  msModePOVShake.setFrames(32);

#if 0
  // // debug output so we know we're alive in case a mode dies ..
  // for (byte idx = 0; idx < MAX_LEDS; idx++)
  // {
  //   msSystem.msLEDs.setPixels(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
  // }
#endif 

}


void testAccelerometer()
{
  Serial.println("trying accel!");

  int d = 500;
  int b = 20;
  while (1)
  {
    if (msSystem.msAccelOK) 
    {
      msSystem.msLEDs.fillPixels(0, b, 0, 0xff);
    }
    else
    {
     msSystem.msLEDs.fillPixels(b, 0, 0, 0xff);
   }
   msSystem.msLEDs.updatePixels();
   delay(d);

   msSystem.msLEDs.fillPixels(b, b, b, 0xff);
   msSystem.msLEDs.updatePixels();
   delay(d);
 }
}


void testButtonForBOM_X()
{
  if (msGlobals.ggCurrentFrame % 1000 == 0)
  {
    msSystem.logln("_");
  }

  if (msSystem.msButtons.msBtnPwrLongHit)
  {
    msSystem.msLEDs.setPixels(1, 0, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnPwrHit)
  {
    msSystem.msLEDs.setPixels(1, 20, 20, 0, 15);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnALongHit)
  {
    msSystem.msLEDs.setPixels(0, 20, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnAHit)
  {
    msSystem.msLEDs.setPixels(0, 20, 20, 0, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnBLongHit)
  {
    msSystem.msLEDs.setPixels(2, 20, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnBHit)
  {
    msSystem.msLEDs.setPixels(2, 20, 20, 0, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
}


void testSimpleButtons()
{

  double avgVal = 800;
  double avgF = 0.01;

  int cnt = 0;

  int minV = 1000;
  int maxV = 0;

  cnt++;

  int adVal = msSystem.getADValue();

  avgVal = avgF * adVal + (1-avgF) * avgVal; 

  if (minV > adVal) minV = adVal;
  if (maxV < adVal) maxV = adVal;

  if (adVal > 950 )
  {
    msSystem.msLEDs.setPixels(7, 0, 10, 0, msGlobals.ggFactoryIntensity);
    msSystem.msLEDs.setPixels(8, 0, 10, 0, msGlobals.ggFactoryIntensity);
  }
  else
  {
    msSystem.msLEDs.setPixels(7, 10, 0, 0, msGlobals.ggFactoryIntensity);
    msSystem.msLEDs.setPixels(8, 10, 0, 0, msGlobals.ggFactoryIntensity);
  }

  if (!digitalRead(PIN_BUTTON_A))
  {
    msSystem.msLEDs.setPixels(9, 0, 10, 0, msGlobals.ggFactoryIntensity);
  }
  else
  {
    msSystem.msLEDs.setPixels(9, 10, 0, 0, msGlobals.ggFactoryIntensity);
  }

  if (!digitalRead(PIN_BUTTON_B))
  {
    msSystem.msLEDs.setPixels(6, 0, 10, 0, msGlobals.ggFactoryIntensity);
  }
  else
  {
    msSystem.msLEDs.setPixels(6, 10, 0, 0, msGlobals.ggFactoryIntensity);
  }
  if (cnt % 300 == 550)
  {
    minV++;
    maxV--;
  }

  if (cnt%40 == 0)
  {
    Serial.print(minV);
    Serial.print(":");
    Serial.print(maxV);
    Serial.print("|");
    Serial.print(adVal);
    Serial.print("/");
    Serial.println(avgVal);
  }
  msSystem.msLEDs.updatePixels();
  delay(10);
  //return;
}

void logButtons()
{
  msSystem.log("msBtnA:"); msSystem.logln(String(msSystem.msButtons.msBtnAPressTime));
  msSystem.log("msBtnPwr:"); msSystem.logln(String(msSystem.msButtons.msBtnPwrPressTime));
  msSystem.log("msBtnB:"); msSystem.logln(String(msSystem.msButtons.msBtnBPressTime));
}

void loop()
{

  msGlobals.ggLastMicros = msGlobals.ggCurrentMicros;
  msGlobals.ggCurrentMicros = micros();

  msSystem.loop();

  msWebServer.loop();

  // do some tests
  // testAccelerometer();
  // testButtonForBOM_X();
  // testSimpleButtons();
  // logButtons();

  // outside time-frame
#ifdef CONFIG_ENABLE_ACCEL
    msSystem.msAccel.readAccelData(msGlobals.ggAccelCounts);

    for (int i = 0 ; i < 3 ; i++) {
      msGlobals.ggAccel[i] = (float) msGlobals.ggAccelCounts[i] / ((1 << 12) / (2 * GSCALE)); // get actual g value, this depends on scale being set
    }
#endif

  // inside time-frame
  if (msGlobals.ggLastFrameMicros + msGlobals.ggSpeedMicros < msGlobals.ggCurrentMicros)
  {
    msGlobals.ggLFrameTime = msGlobals.ggCurrentMicros - msGlobals.ggLastFrameMicros;
    msGlobals.ggCurrentFrame++;
    msGlobals.ggLastFrameMicros = msGlobals.ggCurrentMicros;

msGlobals.ggCurrentMode = 2;

    // dispatch to the mode handler .. 
    if (msGlobals.ggCurrentMode == 0)
    {
      msMagicShake.step();
    }
    else 
    if (msGlobals.ggCurrentMode == 1)
    {
      msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
      msSystem.msLEDs.updatePixels();
      delay(10);
    }
    else
    if (msGlobals.ggCurrentMode == 2) {
      // swipe colors
      for (byte idx = 0; idx < MAX_LEDS; idx++)
      {
      //   if (idx % 8 == 0) {
      //     msSystem.msLEDs.setPixels(idx, 255, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
      //   }
      // else
      //   msSystem.msLEDs.setPixels(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);


        msSystem.msLEDs.setPixels(idx, 255,255,255,20);


        msSystem.msLEDs.updatePixels();
        delay(10);
      }
      // for (byte idx = 0; idx < MAX_LEDS; idx++)
      // {
      //   msSystem.msLEDs.setPixels(idx, 0, 0, 0, 1 / 2);
      //   msSystem.msLEDs.updatePixels();
      //   delay(10);
      // }
    }
    else 
    if (msGlobals.ggCurrentMode = 3)
    {
      msHackDemo.step();
    }
    else { // mode out of bounds failure
    }

  }

}

