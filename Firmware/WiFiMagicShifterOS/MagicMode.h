#ifndef __MAGICMODE_H
#define __MAGICMODE_H

#include "tools.h"

#include "MagicShifterGlobals.h"

extern MagicShifterGlobals msGlobals;

#include "Image.h"

#include "ShakeSync.h"

#define FRAME_MULTIPLY 4


class IMode
{
public:
  void start(MagicShifterSystem *msSystem);
  void stop(void);
  void loop();
};

class BaseMode : public IMode
{
private:
  MagicShifterSystem *m_msSystem;

  public:
    void start(MagicShifterSystem *msSystem)
    {
      m_msSystem = msSystem;
    }

    void stop(void)
    {
      m_msSystem = NULL;
    }

    /*
    void loop()
    {
    }
    */
};

// make it larger to be on the save side when base64 decoding
/*
class RGBLightMode :MagicMS_Globals.shifterMode
{
private:
  loadBuffer(msGlobals.web_rgb_buffer);
  updatePixels();

public:
  void loop(void)
  {
    loadBuffer(msGlobals.web_rgb_buffer);
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
    if (shakeSync.update(msGlobals.accelG[2]))
    {
      int index = shakeSync.getFrameIndex();
      if (index > 0)
      {
        //// msSystem.logln(index);
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
    // msSystem.log("set frames to: ");
    // msSystem.logln(w);
  }
};


#endif
