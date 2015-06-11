//#include "os_type.h"
#ifndef APA102_H
#define APA102_H

#define USE_HW_SPI

#ifdef USE_HW_SPI
  #include "SPI.h"
#else

#endif

#define RGB_BUFFER_SIZE (4*LEDS)
byte ledBuffer[RGB_BUFFER_SIZE + 8];
byte clearBuffer[RGB_BUFFER_SIZE + 8];

byte *RGB_COLORS = ledBuffer + 4;;


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

void InitSPI()
{
  #ifdef USE_HW_SPI
    SPI.begin();
    SPI.setFrequency(20000000);
  #else
    pinMode(PIN_LED_DATA, OUTPUT);
    pinMode(PIN_LED_CLOCK, OUTPUT);
  #endif
}

void InitAPA102() {
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
}

void setPixel(int index, byte r, byte g, byte b, byte gs = 0x1F)
{
  int idx = index << 2;
  RGB_COLORS[idx] = 0xE0 | gs;
  RGB_COLORS[idx+1] = b;
  RGB_COLORS[idx+2] = g;
  RGB_COLORS[idx+3] = r;
}

void fillPixels(byte r, byte g, byte b, byte gs = 0x1F)
{
  for (int idx = 0; idx < LEDS; idx++)
  {
    setPixel(idx, r, g, b);
  }
}

void updatePixels()
{
  SPI.writeBytes(ledBuffer, RGB_BUFFER_SIZE + 8);
/*
  SPI.write32(0);

  for (int idx = 0; idx < RGB_BUFFER_SIZE; idx++)
  {
    SPI.write(RGB_COLORS[idx]);
  }

  SPI.write32(0);
*/
}

void fastClear()
{
  SPI.writeBytes(clearBuffer, RGB_BUFFER_SIZE + 8);
}
#endif
