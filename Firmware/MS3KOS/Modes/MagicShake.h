
#include "BaseMode.h"
#include "Image.h"
#include "ShakeSync.h"

#define FRAME_MULTIPLY 4


// make it larger to be on the save side when base64 decoding
/*
class RGBLightMode : .. shiftermode
{
private:
  loadBuffer(msGlobals.web_rgb_buffer);
  updatePixels();

public:
  void step(void)
  {
    loadBuffer(msGlobals.web_rgb_buffer);
    updatePixels();
  }
}
*/

class MagicShakeMode : public MagicShifterBaseMode
{
private:
  char activeFilename[MAX_FILENAME_LENGTH];
  MSImage activeImage;
  POVShakeSyncDummyMode shakeSync;

public:
  MagicShakeMode()
  {
  }

  void start()
  {
    setActiveFile(msGlobals.uploadFileName);
    
  } // todo: startActiveFile() with a default filename

  void stop()
  {}

  void step()
  {
    if (shakeSync.update(msGlobals.accelG[2]))
    {
      int index = shakeSync.getFrameIndex();
      if (index > 0)
      {
        //msSystem.logln(index);
        byte povData[RGB_BUFFER_SIZE];
        activeImage.readFrame(index / FRAME_MULTIPLY, povData, RGB_BUFFER_SIZE);
        loadBuffer(povData);
        updatePixels();

        delayMicroseconds(POV_TIME_MICROSECONDS);
        fastClear();
        //delay(10);
      }
    }
    // !J! TODO: give modes an event queue ..
    if (msGlobals.setActiveFile == 1) {
      setActiveFile(msGlobals.uploadFileName);
      msGlobals.setActiveFile = 0;
    }
  }

  void setActiveFile(char *filename)
  {
    activeImage.close();

    safeStrncpy(activeFilename, filename, MAX_FILENAME_LENGTH);
    activeImage = MSImage(activeFilename);
    int w = activeImage.getWidth() * FRAME_MULTIPLY;
    shakeSync.setFrames(w);
    msSystem.logln("set frames to: ");
    msSystem.logln(String(w));
  }
};
