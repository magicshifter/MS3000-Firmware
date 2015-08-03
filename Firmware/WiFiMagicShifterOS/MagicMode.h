#ifndef __MAGICMODE_H
#define __MAGICMODE_H

#include "tools.h"

// TODO: move to Accel object :)
extern float accelG[3];

#define FRAME_MULTIPLY 4


class IMode
{
public:
  void start(MagicShifter *shifter);
  void stop(void);
  void loop();
};

class BaseMode : public IMode
{
private:
  MagicShifter *m_shifter;

  public:
    void start(MagicShifter *shifter)
    {
      m_shifter = shifter;
    }

    void stop(void)
    {
      m_shifter = NULL;
    }

    /*
    void loop()
    {
    }
    */
};

// make it larger to be on the save side when base64 decoding
/*
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

class DebugMode : public BaseMode
{};

class MagicMode : public BaseMode
{
private:
  char activeFilename[FILENAME_SIZE];
  MSImage activeImage;
  POVShakeSyncDummy shakeSync;

public:
  MagicMode()
  {

  }

  void loop()
  {
    if (shakeSync.update(accelG[2]))
    {
      int index = shakeSync.getFrameIndex();
      if (index > 0)
      {
        //Serial.println(index);
        byte povData[RGB_BUFFER_SIZE];
        activeImage.readFrame(index / FRAME_MULTIPLY, povData, RGB_BUFFER_SIZE);
        loadBuffer(povData);
        updatePixels();

        delayMicroseconds(POV_TIME_MICROSECONDS);
        fastClear();
        //delay(10);
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
};


#endif
