
#include "BaseMode.h"
#include "Image.h"
#include "ShakeSync.h"

#define FRAME_MULTIPLY 4


// make it larger to be on the save side when base64 decoding
/*
class RGBLightMode : .. shiftermode
{
private:
  loadBuffer(msGlobals.ggRGBLEDBuf);
  msSystem.msLEDs.updatePixels();

public:
  void step(void)
  {
    loadBuffer(msGlobals.ggRGBLEDBuf);
    msSystem.msLEDs.updatePixels();
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
    loadAutoFile(msGlobals.ggUploadFileName);
    
  } // todo: startActiveFile() with a default filename

  void stop()
  {}

  void step()
  {
    if (shakeSync.update(msGlobals.ggAccel[2]))
    {
      int index = shakeSync.getFrameIndex();
      if (index > 0)
      {
        //msSystem.logln(index);
        byte povData[RGB_BUFFER_SIZE];
        activeImage.readFrame(index / FRAME_MULTIPLY, povData, RGB_BUFFER_SIZE);
        msSystem.msLEDs.loadBuffer(povData);
        msSystem.msLEDs.updatePixels();

        delayMicroseconds(POV_TIME_MICROSECONDS);
        msSystem.msLEDs.fastClear();
        //delay(10);
      }
    }
    // !J! TODO: give modes an event queue ..
    if (msGlobals.ggShouldAutoLoad == 1) {
      loadAutoFile(msGlobals.ggUploadFileName);
      msGlobals.ggShouldAutoLoad = 0;
    }
  }

  void loadAutoFile(char *filename)
  {
    activeImage.close();

    msSystem.msEEPROMs.safeStrncpy(activeFilename, filename, MAX_FILENAME_LENGTH);
    activeImage = MSImage(activeFilename);
    int w = activeImage.getWidth() * FRAME_MULTIPLY;
    shakeSync.setFrames(w);
    msSystem.log("set frames to: ");
    msSystem.logln(String(w));
  }
};
