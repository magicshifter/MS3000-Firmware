/* NOTES

prevent restarts: Make sure GPIO 2 is not connected, i.e. float.
from https://github.com/esp8266/Arduino/issues/373
*/




/*
// FS hack in FS.h/FS.c
//////////////////////////////////////
// hacked by wizard23
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
////////////////////////////////////////
*/









#include "SPI.h"
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

#include <EEPROM.h>

//#define LEDS 48
//#define LEDS 16

//#define MMA8452_ADDRESS 0x1D  // 0x1D if SA0 is high, 0x1C if low
//#define MMA8452_ADDRESS 0x1C

#include "tools.h"
#include "Config.h"
#include "APA102.h"
#include "MMA8542.h"

#include "MagicShifter.h"

#include "ShakeSync.h"
#include "Image.h"
#include "MagicMode.h"
#include "CircleBall.h"

#include "WebServer.h"

#ifdef MIDISHIFTER
#include "MidiShifter/MidiShifter.h"
#endif


// state
MagicShifter shifter;

MagicMode magicMode;
DebugMode debugMode;
//POVShakeSync shakeSync;
POVShakeSyncDummy shakeSync;

//CircleBall ball(600);
BouncingBall ball(600);


float accelG[3];  // Stores the real accel value in g's

int shifterMode = 0;
int accelCount[3];  // Stores the 12-bit signed value
int oldButton1State = 0;

int currentMicros = 0, lastMicros = 0;
int speedMicros = 1000;
long lastFrameMicros = 0;
int frame = 0;
byte bright = 0xFF;
byte gs = 0x1;
int loops = 0;
long bootTime = 0;
bool apMode = false;
// make it larger to be on the save side when base64 decoding
byte web_rgb_buffer[RGB_BUFFER_SIZE + 4];

extern char uploadname[];

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
  shifter.setup();
  


  bootTime = millis();


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

  loadString(uploadname, FILENAME_LENGTH);
  //if (!FS.exists(uploadname))
  {
    Serial.print("could not find: ");
    Serial.println(uploadname);
    strcpy(uploadname, "big_smile_gif.magicBitmap");
  }
  Serial.print("using POV file: ");
  Serial.println(uploadname);
  magicMode.start(&shifter);
  magicMode.setActiveFile(uploadname);

  for (byte idx = 0; idx < LEDS; idx++)
  {
    setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 1);
  }
  updatePixels();
  //saveBuffer(web_rgb_buffer);



 // delay(1000)

// while (1)
// {
//   // swipe colors
//     for (byte idx = 0; idx < LEDS; idx++)
//     {
//       setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, GLOBAL_GS);
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
    MSImage activeImage = MSImage(uploadname);
    Serial.print("loaded: ");
    Serial.println(uploadname);

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
}

void loop()
{
  pinMode(PIN_BUTTON_B, INPUT);
  shifter.loop();

  HandleWebServer();

  lastMicros = currentMicros;
  currentMicros = micros();
  loops++;

  shakeSync.setFrames(32);

  if (loops % 1000 == 0)
  {
    Serial.print("_");
  }

  if (lastFrameMicros + speedMicros < currentMicros)
  {
    lastFrameMicros = currentMicros;
    frame++;

    // pov ball mode
    if (shifterMode == 0)
    {
      {
        for (byte idx = 0; idx < LEDS; idx++)
        {
          float scale = ball.getLedBright(idx, LEDS);

          scale *= 0.5;

          /*if (ball.allowFlash && ball.smoothLanding)
          {


          }
          else
          {
            scale *= 0.25;
          }
          */

         // int bright = 1;
          //scale *= 10;
          //setPixel(idx, (frame & 1) ? bright*scale : 0, (frame & 2) ? bright*scale : 0, (frame & 4) ? bright*scale : 0, gs);
          
          if (ball.allowFlash)
          {
            if (ball.smoothLanding)
            {
              setPixel(idx, 0, bright * scale, 0, GLOBAL_GS);
            }
            else
            {
              setPixel(idx, bright * scale, bright * scale, bright * scale, GLOBAL_GS);
            }
          }
          else
          {  
            setPixel(idx, bright * scale, 0, 0.5 * bright * scale, GLOBAL_GS);
          }
        }
      }
      updatePixels();
    }
    else if (shifterMode == 1)
    {
      loadBuffer(web_rgb_buffer);
      updatePixels();
    }
    else if (shifterMode == 2)
    {
      magicMode.loop();
    }
  }

#ifndef DISABLE_ACCEL
  readAccelData(accelCount);

  for (int i = 0 ; i < 3 ; i++)
  {
    accelG[i] = (float) accelCount[i] / ((1 << 12) / (2 * GSCALE)); // get actual g value, this depends on scale being set
  }
#endif



  float fX = accelG[0];
  float fY = accelG[1];

  //ball.applyForce((currentMicros - lastMicros) / 1000.0, fX, fY);
  ball.applyForce((currentMicros - lastMicros) / 1000.0, fX);
}
