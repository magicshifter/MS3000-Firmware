//#include "os_type.h"
#ifndef APA102_H
#define APA102_H

#define USE_HW_SPI

#ifdef USE_HW_SPI
  #include "SPI.h"
#else

#endif

#define RGB_BUFFER_SIZE (4*LEDS)
byte RGB_COLORS[RGB_BUFFER_SIZE ];

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
  #else
    pinMode(PIN_LED_DATA, OUTPUT);
    pinMode(PIN_LED_CLOCK, OUTPUT);
  #endif

  SPI.setFrequency(40000000);
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
  SPI.write32(0);

  for (int idx = 0; idx < LEDS*4; idx++)
  {
    SPI.write(RGB_COLORS[idx]);
  }

/*
  for (int idx = 0; idx < LEDS; idx++)
  {
    SPI.write32(((long*)RGB_COLORS)[idx]);
  }

//  */
  SPI.write32(0);
}
#endif
