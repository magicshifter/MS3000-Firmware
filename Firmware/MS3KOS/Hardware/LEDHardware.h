//#include "os_type.h"

// LED Buffer functional interface to APA102 LED controller

#ifndef LEDHARDWARE_H
#define LEDHARDWARE_H

#define USE_HW_SPI
#ifdef USE_HW_SPI
  #include <SPI.h>
#endif

#define LED_TYPE_APA102 1 
#define LED_TYPE_WS2801 2

#define LED_TYPE LED_TYPE_APA102

#if(HW_ID==HW_ID_RING)
	#define LED_TYPE LED_TYPE_WS2801
#endif

#if (LED_TYPE == LED_TYPE_APA102)
  #define SPI_FREQUENCY 20000000
#else
  #define SPI_FREQUENCY 500000
#endif

#define PIN_LED_ENABLE 15

// PIN_LED_DATA an PIN_LED_CLOCK can not be changed since we use the hardware SPI
#define PIN_LED_DATA  13
#define PIN_LED_CLOCK 14


// TODO : Flesh this out?
class Leds
{
  void setPixelsHighRes();
  void setPixel();
  void update();
  void fastClear();
  void fastPulse(int microsShort, int microsLong);
  void getPixels();
};


byte ledBuffer[RGB_BUFFER_SIZE + 8];
byte clearBuffer[RGB_BUFFER_SIZE + 8];

byte *RGB_COLORS = ledBuffer + 4;


void saveBuffer(byte *buffer)
{
  for (int i = 0; i < RGB_BUFFER_SIZE ; i++)
  {
    buffer[i] = RGB_COLORS[i];
  }
}

void loadBuffer(byte *buffer)
{
  for (int i = 0; i < RGB_BUFFER_SIZE ; i++)
  {
    RGB_COLORS[i] = buffer[i];
  }
}

void disableLEDHardware()
{
  pinMode(PIN_LED_ENABLE, INPUT);
}


void initLEDHardware()
{
  pinMode(PIN_LED_ENABLE, OUTPUT);
  digitalWrite(PIN_LED_ENABLE, HIGH);

  #ifdef USE_HW_SPI
    SPI.begin();
    SPI.setFrequency(SPI_FREQUENCY);
  #else
    pinMode(PIN_LED_DATA, OUTPUT);
    pinMode(PIN_LED_CLOCK, OUTPUT);
  #endif
}

void fillPixels(byte r, byte g, byte b, byte gs);

void initLEDBuffer() {
  ledBuffer[0] = 0;
  ledBuffer[1] = 0;
  ledBuffer[2] = 0;
  ledBuffer[3] = 0;


  ledBuffer[RGB_BUFFER_SIZE + 0] = 0;
  ledBuffer[RGB_BUFFER_SIZE + 1] = 0;
  ledBuffer[RGB_BUFFER_SIZE + 2] = 0;
  ledBuffer[RGB_BUFFER_SIZE + 4] = 0;



  for (int i = 0; i < RGB_BUFFER_SIZE + 8; i+=4)
  {
    clearBuffer[i] = (i < 4 || i >= RGB_BUFFER_SIZE + 4) ? 0 : 0xFF;
    clearBuffer[i+1] = 0;
    clearBuffer[i+2] = 0;
    clearBuffer[i+3] = 0;
  }

  fillPixels(0,0,0, 0);
}

void setPixel(int index, byte r, byte g, byte b, byte gs = 0x1F)
{
#if (LED_TYPE == LED_TYPE_APA102)
    int idx = index << 2;
    RGB_COLORS[idx] = 0xE0 | gs;
    RGB_COLORS[idx+1] = b;
    RGB_COLORS[idx+2] = g;
    RGB_COLORS[idx+3] = r;
#else
    int idx = index * 3;
    RGB_COLORS[idx+0] = b;
    RGB_COLORS[idx+1] = g;
    RGB_COLORS[idx+2] = r;
#endif
}

void fillPixels(byte r, byte g, byte b, byte gs = 0x1F)
{
  for (int idx = 0; idx < MAX_LEDS; idx++)
  {
    setPixel(idx, r, g, b, gs);
  }
}

void updatePixels()
{
#if (LED_TYPE == LED_TYPE_APA102)
    SPI.writeBytes(ledBuffer, RGB_BUFFER_SIZE + 8);
#else
    SPI.writeBytes(RGB_COLORS, RGB_BUFFER_SIZE);
#endif
}

void fastClear()
{
  SPI.writeBytes(clearBuffer, RGB_BUFFER_SIZE + 8);
}

#endif
// LEDHARDWARE_H
