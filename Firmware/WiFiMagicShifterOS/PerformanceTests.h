#include "APA102.h"

void TestLedTiming() {
long frequ = 0;
int fcount = 0;
  while (1)
  {
    fcount++;
    if (fcount > 40) fcount = 1;
    frequ = fcount * 1000000;
    SPI.setFrequency(frequ);

    // msSystem.logln(frequ);

    byte data[RGB_BUFFER_SIZE];
    int t0 = micros();
    SPI.writeBytes(data, RGB_BUFFER_SIZE);
    /*
    for (int i = 0; i < 0; i++)
    {
      updatePixels();
    }*/
    int t1 = micros();

    // msSystem.logln(t1-t0);
    delay(100);

  }

}
