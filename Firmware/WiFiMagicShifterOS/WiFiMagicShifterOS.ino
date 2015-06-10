// 2M / 1MB SPIFFS
#include "SPI.h"
#include <Ticker.h>
#include <math.h>
#include <Wire.h> // Used for I2C
#include <Arduino.h>
#include <FileSystem.h>

//#include <flashmem.h>
//#include "spi_flash.h"
//#include <spi_flash.h>


#include <Esp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <ArduinoJson.h>

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

#include "APA102.h"
#include "MMA8542.h"
#include "WebServer.h"

#include "CircleBall.h"


int shifterMode = 2;
int accelCount[3];  // Stores the 12-bit signed value
int oldButton1State = 0;
CircleBall ball(600.);
int currentMicros = 0, lastMicros = 0;
int speedMicros = 1000;
long lastFrameMicros = 0;
int frame = 0;
byte bright = 0x03;
byte gs = 0x1;
int loops = 0;

extern const char *uploadname;




uint32 spi_flash_get_id(void);

/*
SpiFlashOpResult spi_flash_erase_sector(uint16 sec);
SpiFlashOpResult spi_flash_write(uint32 des_addr, uint32 *src_addr, uint32 size);
SpiFlashOpResult spi_flash_read(uint32 src_addr, uint32 *des_addr, uint32 size);
*/

void setup()
{
  Serial.begin(115200);
  Serial.println("\r\nMagicShifter 3000 OS V0.24");


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
  Serial.println(ESP.getSketchSize());

  Serial.print("Free sketch space: ");
  Serial.println(ESP.getFreeSketchSpace());

  Serial.print("Reset info: ");
  Serial.println(ESP.getResetInfo());



  //delay(1000);

  spi_flash_get_id();



  char *filename = "bitmap1.magicbitmap";
  Serial.print("FS mount: ");
  Serial.println(FS.mount());
/*

    //void unmount();
    //bool format();
    //Serial.print("FS check: ");
    //Serial.println(FS.check());



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




  // init pinmodes
  pinMode(PIN_BUTTON1, INPUT);

  // leds
  InitSPI();

  // accel
  InitI2C();


  // init components
  InitMMA8452(); //Test and intialize the MMA8452
  StartWebServer();
}


bool loadFile = true;
FSFile povFile;

bool getNextPOVData(byte *buffer, int size)
{
  if (loadFile)
  {
    povFile = FS.open(uploadname, FSFILE_READ);
    loadFile = false;
    Serial.println("opened");
    delay(2);
  }

  if (povFile)
  {

      int result = povFile.read(buffer, size);

      if (result < size)
      {
        povFile.close();
        loadFile = true;
        Serial.println("closed");
        delay(2);
      }
  }
  else
  {
    povFile.close();
    loadFile = true;
    Serial.println("closeerror");
    delay(2);
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
      if (frame % 3 == 0)
        fillPixels(bright, 0, 0, gs);
      else if (frame % 3 == 1)
        fillPixels(0, 0, bright, gs);
      else
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