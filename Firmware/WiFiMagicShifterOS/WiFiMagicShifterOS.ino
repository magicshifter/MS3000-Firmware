/* NOTES

prevent restarts: Make sure GPIO 2 is not connected, i.e. float.
from https://github.com/esp8266/Arduino/issues/373
*/

#include "SPI.h"
#include <Ticker.h>
#include <math.h>
#include <Wire.h> // Used for I2C
#include <Arduino.h>
#include <FileSystem.h>
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

#define LEDS 48
//#define LEDS 16

#define PIN_LED_DATA  13
#define PIN_LED_CLOCK 14
// double features as bootloader button
#define PIN_BUTTON1   0

#define PIN_I2C_DATA 4 // 5 //blau // labeled 5 on esp12E!!!
#define PIN_I2C_CLOCK 2 //lila

//#define MMA8452_ADDRESS 0x1D  // 0x1D if SA0 is high, 0x1C if low
#define MMA8452_ADDRESS 0x1C

#include "Config.h"
#include "APA102.h"
#include "MMA8542.h"
#include "WebServer.h"

#include "CircleBall.h"

#include "PerformanceTests.h"


int shifterMode = 0;
int accelCount[3];  // Stores the 12-bit signed value
int oldButton1State = 0;
CircleBall ball(600.);
int currentMicros = 0, lastMicros = 0;
int speedMicros = 3000;
long lastFrameMicros = 0;
int frame = 0;
byte bright = 0x03;
byte gs = 0x1;
int loops = 0;

extern char uploadname[];

/*
SpiFlashOpResult spi_flash_erase_sector(uint16 sec);
SpiFlashOpResult spi_flash_write(uint32 des_addr, uint32 *src_addr, uint32 size);
SpiFlashOpResult spi_flash_read(uint32 src_addr, uint32 *des_addr, uint32 size);
*/

void setup()
{
  Serial.begin(115200);
  Serial.println("\r\nMagicShifter 3000 OS V0.24");


/*
  struct jsonparse_state jsonState;
  const char *jsonLastAP = "{\"last\":{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"}";
  const char *jsonAPList =  "{\"list\": [{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"}, {\"ssid\":\"PACIFIC\", \"pwd\":\"AllesR0ger\"}]}";

  jsonparse_setup(&jsonState, jsonAPList, strlen(jsonAPList));
  Serial.print("jsonparse_setup state:");
  Serial.println(jsonAPList);
  Serial.println(jsonState.pos);
  Serial.println(jsonState.len);
  Serial.println(jsonState.error);


  char copyBuffer[100];
  int copyResult, lenResult, typeResult, nextResult;


  for (int i = 0; i < 30; i++)
  {
    lenResult = jsonparse_get_len(&jsonState);
    Serial.print("len: ");
    Serial.println(lenResult);

    typeResult = jsonparse_get_type(&jsonState);
    Serial.print("type: ");
    Serial.print((char)typeResult);
    Serial.println(typeResult);

    copyResult = jsonparse_copy_value(&jsonState, copyBuffer, 100);
    Serial.print("copy: ");
    Serial.println(copyResult);
    Serial.println(copyBuffer);

    nextResult = jsonparse_next(&jsonState);
    Serial.print("next: ");
    Serial.print((char)nextResult);
    Serial.println(nextResult);
  }


while (1)
  delay(100);


*/

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

  Serial.print("flash speed: ");
  Serial.println(ESP.getFlashChipSpeed());

  Serial.print("flash mode: ");
  Serial.println(ESP.getFlashChipMode());

  Serial.print("Sketch size: ");
  //Serial.println(ESP.getSketchSize());

  Serial.print("Free sketch space: ");
  //Serial.println(ESP.getFreeSketchSpace());

  Serial.print("Reset info: ");
  //Serial.println(ESP.getResetInfo());



  Serial.print("FS mount: ");
  Serial.println(FS.mount());
/*
    //void unmount();
    //bool format();
    //Serial.print("FS check: ");
    //Serial.println(FS.check());

    char *filename = "bitmap1.magicbitmap";


    Serial.print("FS exists: ");
    Serial.println(FS.exists(filename));

    Serial.print("FS create: ");
    Serial.println(FS.create(filename));

    Serial.print("FS exists: ");
    Serial.println(FS.exists(filename));

    Serial.print("FS create: ");
    Serial.println(FS.create(filename));


  FSFile file = FS.open(filename, FSFILE_WRITE);
  Serial.print("file write: ");
  Serial.println(file.write(filename, 5));
  file.close();


  byte buffer[205];
  file = FS.open(filename, FSFILE_READ);

  int fileLen = file.available();
  Serial.print("file available: ");
  Serial.println(fileLen);

  if (fileLen >= 200 ) fileLen = 200;
  Serial.println("reading: ");
  Serial.println(file.read(buffer, fileLen));

  Serial.println("buffer: ");

  for (int i = 0; i < fileLen; i++) {
    Serial.println(buffer[i]);
  }

  file.close();

//*/

//TestLedTiming();





  // init pinmodes
  pinMode(PIN_BUTTON1, INPUT);

  // leds
  InitSPI();

  // accel
  InitI2C();

  // init components
  InitAPA102();
  InitMMA8452(); //Test and intialize the MMA8452


/*

  byte r = 255, g, b;
  byte gs = 0x04;
  int pos = 0;

  bright = 255;
  gs = 0x1F;
  float speed = 0.005;

  frame = 1;
  while (1)
  {
    lastMicros = currentMicros;
    currentMicros = micros();

    ball.applyForce((currentMicros - lastMicros)/1000.0, speed);
    for (byte idx = 0; idx < LEDS; idx++)
    {
      float scale = ball.getLedBright(idx, LEDS);
      scale *= 10;
      setPixel(idx, (frame & 1) ? bright*scale : 0, (frame & 2) ? bright*scale : 0, (frame & 4) ? bright*scale : 0, gs);
      //setPixel(idx, bright * scale, 0, bright * scale, gs);
    }
    updatePixels();
    delay(1);

    if (!digitalRead(PIN_BUTTON1))
    {
      oldButton1State++;
    }
    else
    {
      if (oldButton1State >= 500)
      {
        speed += 0.01;
        if (speed > 0.1)
        {
          speed = 0.003;
        }
      }
      else if (oldButton1State >= 200)
      {
        frame++;
        frame %= 8;
        if (frame == 0) frame++;
      }
      if (oldButton1State >= 5)
      {
        gs += 7;
        gs %= 0x20;
      }
      oldButton1State = 0;
    }
  }
*/



  StartWebServer();
}


bool loadFile = true;
bool closeErrorReported = false;
FSFile povFile;

bool getNextPOVData(byte *buffer, int size)
{
  if (loadFile)
  {
    povFile = FS.open(uploadname, FSFILE_READ);
    loadFile = false;
    //Serial.print("opened: ");
    //Serial.println(uploadname);
    delay(2);
  }

  if (povFile)
  {

      int result = povFile.read(buffer, size);

      if (result < size)
      {
        povFile.close();
        loadFile = true;
        //Serial.print("closed: ");
        //Serial.println(uploadname);
        delay(2);
      }
      else
      {
        closeErrorReported = false;
      }
  }
  else
  {
    povFile.close();
    loadFile = true;
    if (!closeErrorReported)
    {
      Serial.println("close error!");
      closeErrorReported = true;
      delay(20);
    }
  }
}

void loop()
{
  HandleWebServer();

  lastMicros = currentMicros;
  currentMicros = micros();
  loops++;

  if (loops % 500 == 0)
  {
    if (loops % 40000 == 0)
      Serial.println("x");
    Serial.print("_");
  }

  if (lastFrameMicros + speedMicros < currentMicros)
  {
    lastFrameMicros = currentMicros;
    frame++;

    // pov ball mode
    if (shifterMode == 0)
    {
      /*if (frame % 3 == 0)
        fillPixels(bright, 0, 0, gs);
      else if (frame % 3 == 1)
        fillPixels(0, 0, bright, gs);
      else
      */
      {
        for (byte idx = 0; idx < LEDS; idx++)
        {
          float scale = ball.getLedBright(idx, LEDS);
          scale *= 10;
          setPixel(idx, (frame & 1) ? bright*scale : 0, (frame & 2) ? bright*scale : 0, (frame & 4) ? bright*scale : 0, gs);
          setPixel(idx, bright * scale, 0, bright * scale, gs);
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
      byte povData[RGB_BUFFER_SIZE];
      getNextPOVData(povData, RGB_BUFFER_SIZE);
      loadBuffer(povData);
      updatePixels();

      delayMicroseconds(POV_TIME_MICROSECONDS);
      fastClear();
      // manual blank, could be faster...
      //fillPixels(0,0,0);
      //updatePixels();
    }
  }

  readAccelData(accelCount);  // Read the x/y/z adc values
  float accelG[3];  // Stores the real accel value in g's
  for (int i = 0 ; i < 3 ; i++)
  {
    accelG[i] = (float) accelCount[i] / ((1 << 12) / (2 * GSCALE)); // get actual g value, this depends on scale being set
  }


  float calX = 1;
  float calY = 0.87;

  float l = sqrt(calX * calX + calY * calY);
  calX /= l;
  calY /= l;

  float fX = calX * accelG[0] + calY * accelG[1];
  float fY = calY * accelG[0] - calX * accelG[1];

  fX = -fX;
  fY = -fY;

  ball.applyForce((currentMicros - lastMicros) / 1000.0, fX, fY);

  //ball.applyForce((m - lastM)/1000.0, 0.04);

  if (!digitalRead(PIN_BUTTON1))
  {
    oldButton1State++;
  }
  else
  {
    if (oldButton1State >= 400)
    {
      delay(100);
      float pos = 0;
      while (digitalRead(PIN_BUTTON1)) {
        float w = 1;
        for (byte idx = 0; idx < LEDS; idx++)
        {
          float scale = idx - pos;
          if (scale < 0) scale = -scale;
          if (scale < w) {
            scale = 1 - (scale / w);
          }
          else scale = 0;
          setPixel(idx, (frame & 1) ? bright*scale : 0, (frame & 2) ? bright*scale : 0, (frame & 4) ? bright*scale : 0, gs);
          setPixel(idx, (frame & 1) ? bright*scale : 0, 0 * ((frame & 2) ? bright*scale : 0), (frame & 4) ? bright*scale : 0, gs);
        }

        updatePixels();
        frame++;
        pos += 0.03;
        if (pos >= LEDS) pos -= LEDS;
        delay(0);
      }
    }
    else if (oldButton1State >= 5)
    {
      speedMicros += 200;
      if (speedMicros > 10000) speedMicros = 0;
    }
    oldButton1State = 0;
  }
}
