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
#include "WebServer/WebServer.h" 

// MIDI can be configured on or off 
#ifdef CONFIG_ENABLE_MIDI
#include "MidiShifter/MidiShifter.h"
#endif

// GUI modes, well actually .. modes are more of an 'app' ..
#include "Modes/Modes.h"

BouncingBallMode msBouncingBallMode(600);
MagicShakeMode msShakeMode;
POVShakeSyncDummyMode msPOVShakeSyncMode;

double avgVal = 800;
double avgF = 0.01;

int cnt = 0;

int minV = 1000;
int maxV = 0;

// Begin MagicShifter3000 operation
void setup()
{
  msGlobals.bootTime = millis();
  msSystem.setup();
  
  // boot that we are alive
  msSystem.do_debug_swipe();

  // get the web interface started
  StartWebServer();

  // todo: move to module.
  msPOVShakeSyncMode.setFrames(32);

  // start Modes as necessary ..
  loadString(msGlobals.uploadFileName, MAX_FILENAME_LENGTH);
  if (!SPIFFS.exists(msGlobals.uploadFileName))
  {
    msSystem.logln("could not find: ");
    msSystem.logln(msGlobals.uploadFileName);
    // strcpy(msGlobals.uploadFileName, "big_smile_gif.magicBitmap");
  }
  else 
  {
    msSystem.logln("using POV file: ");
    msSystem.logln(msGlobals.uploadFileName);
    msShakeMode.start();
  }

  // // debug output so we know we're alive in case a mode dies ..
  // for (byte idx = 0; idx < MAX_LEDS; idx++)
  // {
  //   setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
  // }

  updatePixels();

}

void TestAccelerometer()
{
  Serial.println("trying accel!");
  resetAccelerometer(); //Test and intialize the MMA8452

  int d = 500;
  int b = 20;
  while (1)
  {
    if (msSystem.accelerometerWorking) 
    {
      fillPixels(0, b, 0, 0xff);
    }
    else
    {
       fillPixels(b, 0, 0, 0xff);
    }
    updatePixels();
    delay(d);

    fillPixels(b, b, b, 0xff);
    updatePixels();
    delay(d);
  }
}

// do a simple bouncing ball .. 
void simpleBouncingBall()
{
  for (byte idx = 0; idx < MAX_LEDS; idx++)
  {
    float scale = msBouncingBallMode.getLedBright(idx, MAX_LEDS);

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
  updatePixels();
}


void testButtonForBOM_X()
{
  if (msGlobals.loops % 1000 == 0)
  {
    msSystem.logln("_");
  }

  if (msSystem.longClickedButtonPower)
  {
    setPixel(1, 0, 0, 20, 20);
    updatePixels();
    delay(200);
  }
  if (msSystem.clickedButtonPower)
  {
    setPixel(1, 20, 20, 0, 15);
    updatePixels();
    delay(200);
  }
  if (msSystem.longClickedButtonA)
  {
    setPixel(0, 20, 0, 20, 20);
    updatePixels();
    delay(200);
  }
  if (msSystem.clickedButtonA)
  {
    setPixel(0, 20, 20, 0, 20);
    updatePixels();
    delay(200);
  }
  if (msSystem.longClickedButtonB)
  {
    setPixel(2, 20, 0, 20, 20);
    updatePixels();
    delay(200);
  }
  if (msSystem.clickedButtonB)
  {
    setPixel(2, 20, 20, 0, 20);
    updatePixels();
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

void loop()
{

  msGlobals.lastMicros = msGlobals.currentMicros;
  msGlobals.currentMicros = micros();
  msGlobals.loops++;

  // testButtonForBOM_X()

  msSystem.loop();
  delayYield();

  HandleWebServer();
  delayYield();

  // do some tests
// #ifdef CONFIG_ENABLE_ACCEL
  // TestAccelerometer();
// #endif

  // inside time-frame
  if (msGlobals.lastFrameMicros + msGlobals.speedMicros 
                < msGlobals.currentMicros)
  {
    msGlobals.loopFrameTime = msGlobals.currentMicros - msGlobals.lastFrameMicros;

    msGlobals.currentFrame++;

    msGlobals.lastFrameMicros = msGlobals.currentMicros;

    // pov msBouncingBallMode mode
    if (msGlobals.shifterMode == 0)
    {
      simpleBouncingBall();
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
//       updatePixels();
//       delayYield();

// // end-of-hack
    }
    else 
    if (msGlobals.shifterMode == 1)
    {
      loadBuffer(msGlobals.web_rgb_buffer);
      updatePixels();
    }
    else 
    if (msGlobals.shifterMode == 2)
    {
      msShakeMode.step();
    }
    else 
    if (msGlobals.shifterMode == 3)
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
            setPixel(idx2, rRed, rGreen, rBlue, 255);
          else
            setPixel(idx2, 0,0,0,0);
        }
        delay(1);
        delayMicroseconds(pause);
        updatePixels();
      }
    }
    if (msGlobals.shifterMode == 4)
    {
       long currentTime = msGlobals.time + (millis() - msGlobals.timePostedAt);
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

       fillPixels(0,0,0,0);
       int c = 255;
       int b = 255;
       setPixel(ledStart + ledLen * hours / 24., c, 0, 0, b);
       setPixel(ledStart + ledLen * minutes / 60., 0, c, 0, b);
       setPixel(ledStart + ledLen * seconds / 60., 0, 0, c, b);

       updatePixels();

      Serial.println(currentTime);
       Serial.println(seconds);
       delay(100);
    }

  // outside time-frame
#ifdef CONFIG_ENABLE_ACCEL
    readAccelData(msGlobals.accelCount);
    delayYield();

    for (int i = 0 ; i < 3 ; i++)
    {
      msGlobals.accelG[i] = (float) msGlobals.accelCount[i] / ((1 << 12) / (2 * GSCALE)); // get actual g value, this depends on scale being set
    }
#endif

    float fX = msGlobals.accelG[0];
    float fY = msGlobals.accelG[1];

    //msBouncingBallMode.applyForce((msGlobals.currentMicros - msGlobals.lastMicros) / 1000.0, fX, fY);

    msBouncingBallMode.applyForce((msGlobals.currentMicros - msGlobals.lastMicros) / 1000.0, fX);

    delayYield();

    // !J! hack of timing .. 
    // delay(1); // if we lose this, we lose wifi .. grr .. 

    // delayMicroseconds(500);
  }

} // end of loop()
