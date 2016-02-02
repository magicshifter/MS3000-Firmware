/*
 * MagicShifter3000 OS, Copyright (c) wizards@Work
 * Authors: wizard23(pt), seclorum(jv)
 * Notes: 
 *          All code is conflated by headers (.h):w
 *          Code prefix- is ms*, as in msConfig, msSystem, etc.
 */

extern "C" {
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
#include <json/json.h>
#include <json/jsonparse.h>
#include <json/jsontree.h>
}

//compiler.c.elf.libs=-lm -lgcc -lhal -lphy -lnet80211 -llwip -lwpa -lmain -lpp -lsmartconfig -lc -ljson

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
MagicSystemTextMode msSysText;

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

  msSystem.showBatteryStatus(true);

  // initialize the modules ..
  msMagicShake.start();
  msSysText.start();
}

void loop()
{

  msGlobals.ggLastMicros = msGlobals.ggCurrentMicros;
  msGlobals.ggCurrentMicros = micros();

  msSystem.loop();

  msWebServer.loop();

  // inside time-frame
  if (msGlobals.ggLastFrameMicros + msGlobals.ggSpeedMicros < msGlobals.ggCurrentMicros)
  {
    msGlobals.ggLFrameTime = msGlobals.ggCurrentMicros - msGlobals.ggLastFrameMicros;
    msGlobals.ggCurrentFrame++;
    msGlobals.ggLastFrameMicros = msGlobals.ggCurrentMicros;

    // dispatch to the mode handler .. 
    if (msGlobals.ggCurrentMode == 0)
    {

      if (msGlobals.ggFault == FAULT_NEW_FILEUPLOAD)
      {
        msGlobals.ggFault = 0;
        msMagicShake.reset();
        
      }

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
        if (idx % 8 == 0) {
          msSystem.msLEDs.setPixels(idx, 255, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
        }
      else
        msSystem.msLEDs.setPixels(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
        // super-bright
        // msSystem.msLEDs.setPixels(idx, 255,255,255,20);

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
    // else 
    // if (msGlobals.ggCurrentMode = 3)
    // {
    //   msSysText.step();
    // }
    // else { // mode out of bounds failure
    // }

  }

  // fault-checks
  if (msGlobals.ggFault > 0)
  {
    {
      Serial.print("FAULT:"); Serial.println(String(msGlobals.ggFault));
      msSystem.infinite_swipe();
    }
  }

}

