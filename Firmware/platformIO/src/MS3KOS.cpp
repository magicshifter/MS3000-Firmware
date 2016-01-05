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

 BouncingBallMode msModeBouncingBall(600);
 MagicShakeMode msModeShake;
 POVShakeSyncDummyMode msModePOVShake;

// Begin MagicShifter3000 operation
 void setup()
 {
  // record our bootup time
  msGlobals.ggBootTime = millis();

  // start the system
  msSystem.setup();
  // start Modes as necessary ..
  msSystem.msEEPROMs.loadString(msGlobals.ggUploadFileName, MAX_FILENAME_LENGTH);
  msSystem.logln("upload file:");
  msSystem.logln(msGlobals.ggUploadFileName);

  if (SPIFFS.exists(msGlobals.ggUploadFileName)) {
    msModeShake.start();
  }

  // todo: move to module start sequence.
  msModePOVShake.setFrames(32);

  // get the web interface started
  msWebServer.StartWebServer();

#if 0
  // // debug output so we know we're alive in case a mode dies ..
  // for (byte idx = 0; idx < MAX_LEDS; idx++)
  // {
  //   msSystem.msLEDs.setPixels(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
  // }
#endif 

  msSystem.msLEDs.bootSwipe();

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

  if (msSystem.msBtnPwrLongHit)
  {
    msSystem.msLEDs.setPixels(1, 0, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msBtnPwrHit)
  {
    msSystem.msLEDs.setPixels(1, 20, 20, 0, 15);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msBtnALongHit)
  {
    msSystem.msLEDs.setPixels(0, 20, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msBtnAHit)
  {
    msSystem.msLEDs.setPixels(0, 20, 20, 0, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msBtnBLongHit)
  {
    msSystem.msLEDs.setPixels(2, 20, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msBtnBHit)
  {
    msSystem.msLEDs.setPixels(2, 20, 20, 0, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
}

void delayYield()
{
  int nYields = 150;  // todo: fix this magic number
  while(nYields--) {
    yield();
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
    msSystem.msLEDs.setPixels(7, 0, 10, 0, msGlobals.ggGS);
    msSystem.msLEDs.setPixels(8, 0, 10, 0, msGlobals.ggGS);
  }
  else
  {
    msSystem.msLEDs.setPixels(7, 10, 0, 0, msGlobals.ggGS);
    msSystem.msLEDs.setPixels(8, 10, 0, 0, msGlobals.ggGS);
  }

  if (!digitalRead(PIN_BUTTON_A))
  {
    msSystem.msLEDs.setPixels(9, 0, 10, 0, msGlobals.ggGS);
  }
  else
  {
    msSystem.msLEDs.setPixels(9, 10, 0, 0, msGlobals.ggGS);
  }

  if (!digitalRead(PIN_BUTTON_B))
  {
    msSystem.msLEDs.setPixels(6, 0, 10, 0, msGlobals.ggGS);
  }
  else
  {
    msSystem.msLEDs.setPixels(6, 10, 0, 0, msGlobals.ggGS);
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


void loop()
{

  msGlobals.ggLastMicros = msGlobals.ggCurrentMicros;
  msGlobals.ggCurrentMicros = micros();

  msSystem.loop();
  
  delayYield();

  msWebServer.HandleWebServer();
  delayYield();

  // do some tests
  // testAccelerometer();
  // testButtonForBOM_X();
  // testSimpleButtons();

  // inside time-frame
  if (msGlobals.ggLastFrameMicros + msGlobals.ggSpeedMicros 
    < msGlobals.ggCurrentMicros)
  {
    msGlobals.ggLFrameTime = msGlobals.ggCurrentMicros - msGlobals.ggLastFrameMicros;

    msGlobals.ggCurrentFrame++;

    msGlobals.ggLastFrameMicros = msGlobals.ggCurrentMicros;

    // pov msModeBouncingBall mode
    if (msGlobals.ggCurrentMode == 0)
    {
      msModeBouncingBall.simpleBouncingBall();
// // !J! 
// // hack to test timing..
//       static int c_loops; 
//       static int c_bright = 10; 
//       static byte c_r, c_g, c_b;
//       c_loops++;
//       c_r = c_loops & 1 ? c_bright : 0;  
//       c_g = c_loops & 2 ? c_bright : 0;  
//       c_b = c_loops & 4 ? c_bright : 0;  
//       fillPixels(c_r, c_g, c_b, 0xff);
//       msSystem.msLEDs.updatePixels();
//       delayYield();

// // end-of-hack
    }
    else 
      if (msGlobals.ggCurrentMode == 1)
      {
        msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
        msSystem.msLEDs.updatePixels();
      }
      else 
        if (msGlobals.ggCurrentMode == 2)
        {
          msModeShake.step();
        }
        else 
          if (msGlobals.ggCurrentMode == 3)
          {
      #define _MOD_LED(m,x) random(m,x)
            int rRed = _MOD_LED(0,255);
            int rGreen = _MOD_LED(0,255);
            int rBlue = _MOD_LED(0,255);
            int pause = random(1, 10000);

            if (random(0,100) < 60) {
              switch(random(0,3)) 
              {
                case 0 : rRed = 0; break;
                case 1 : rGreen = 0; break;
                case 2 : rBlue = 0; break;
              }
            }

            for (byte idx = 0; idx < MAX_LEDS; idx++)
            {
              for (byte idx2 = 0; idx2 < MAX_LEDS; idx2++) 
              {
                if (idx == idx2) 
                  msSystem.msLEDs.setPixels(idx2, rRed, rGreen, rBlue, 255);
                else
                  msSystem.msLEDs.setPixels(idx2, 0,0,0,0);
              }
              delay(1);
              delayMicroseconds(pause);
              msSystem.msLEDs.updatePixels();
            }
          }
          if (msGlobals.ggCurrentMode == 4)
          {
           long currentTime = msGlobals.ggTime + (millis() - msGlobals.ggTimePostedAt);
           long ms = currentTime % 86400000;
       //int millisss = ms % 1000;
           ms /= 1000;
           int seconds = ms % 60;
           ms /= 60;
           int minutes = ms % 60;
           ms /= 60;
           int hours = ms;

           int ledStart = 0;
           int ledEnd = MAX_LEDS - 1;

#if(HW_ID==HW_ID_RING)
           ledStart = 33;
           ledEnd = MAX_LEDS - 33;
#endif
           int ledLen = ledEnd - ledStart;

           msSystem.msLEDs.fillPixels(0,0,0,0);
           int c = 255;
           int b = 255;
           msSystem.msLEDs.setPixels(ledStart + ledLen * hours / 24., c, 0, 0, b);
           msSystem.msLEDs.setPixels(ledStart + ledLen * minutes / 60., 0, c, 0, b);
           msSystem.msLEDs.setPixels(ledStart + ledLen * seconds / 60., 0, 0, c, b);

           msSystem.msLEDs.updatePixels();

           Serial.println(currentTime);
           Serial.println(seconds);
           delay(100);
         }

  // outside time-frame
#ifdef CONFIG_ENABLE_ACCEL
         msSystem.msAccel.readAccelData(msGlobals.ggAccelCounts);
         delayYield();

         for (int i = 0 ; i < 3 ; i++)
         {
      msGlobals.ggAccel[i] = (float) msGlobals.ggAccelCounts[i] / ((1 << 12) / (2 * GSCALE)); // get actual g value, this depends on scale being set
    }
#endif

    float fX = msGlobals.ggAccel[0];
    float fY = msGlobals.ggAccel[1];

    //msModeBouncingBall.applyForce((msGlobals.ggCurrentMicros - msGlobals.ggLastMicros) / 1000.0, fX, fY);
    msModeBouncingBall.applyForce((msGlobals.ggCurrentMicros - msGlobals.ggLastMicros) / 1000.0, fX*3);

    delayYield();

    // !J! hack of timing .. 
    // delay(1); // if we lose this, we lose wifi .. grr .. 
    // delayMicroseconds(500);
  }

} // end of loop()
