#ifndef __MAGICMODE_H
#define __MAGICMODE_H

#include "tools.h"

// TODO: move to Accel object :)
extern float accelG[3];

#define FRAME_MULTIPLY 4

/*
class MagicShifterMode
{
private:

public:
  void activate(void);
  void deactivate(void);
  void loop();
};

class RGBLightMode :MagicShifterMode
{
private:
  loadBuffer(web_rgb_buffer);
  updatePixels();

public:
  void loop(void)
  {
    loadBuffer(web_rgb_buffer);
    updatePixels();
  }
}
*/

class MagicMode // : MagicShifterMode
{
private:
  char activeFilename[FILENAME_SIZE];
  MSImage activeImage;
  POVShakeSync shakeSync;

public:
  MagicMode(void)
  {

  }

  void loop()
  {
    if (shakeSync.update(accelG[2]))
    {
      int index = shakeSync.getFrameIndex();
      if (index > 0)
      {
        byte povData[RGB_BUFFER_SIZE];
        activeImage.readFrame(index / FRAME_MULTIPLY, povData, RGB_BUFFER_SIZE);
        loadBuffer(povData);
        updatePixels();

        delayMicroseconds(POV_TIME_MICROSECONDS);
        fastClear();
      }
    }
  }

  void setActiveFile(char *filename)
  {
    activeImage.close();

    safeStrncpy(activeFilename, filename, FILENAME_SIZE);
    activeImage = MSImage(activeFilename);
    int w = activeImage.getWidth() * FRAME_MULTIPLY;
    shakeSync.setFrames(w);
    Serial.print("set frames to: ");
    Serial.println(w);
  }

  void activate()
  {

  }

  void deactivate();
};


#endif
