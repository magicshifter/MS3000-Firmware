
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
    // todo: startActiveFile() with a default filename
    // initialize the shakefile
    if (SPIFFS.exists(msGlobals.ggUploadFileName)) {
      msSystem.logln("modeShake go");
      ggAFileSet(msGlobals.ggUploadFileName);
    }
    else {
      msSystem.logln("modeShake no-go");
    } 
  }

  void stop()
  {}

  void step()
  {
    if (shakeSync.update(msGlobals.ggAccel[2]))
    {
      msSystem.logln("**sAccels");
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
    else
      msSystem.logln("*");

    // !J! TODO: give modes an event queue ..
    if (msGlobals.ggAFileSet == 1) {
      ggAFileSet(msGlobals.ggUploadFileName);
      msGlobals.ggAFileSet = 0;
      msSystem.logln("**setfile");
    }
  }

  void ggAFileSet(char *filename)
  {
    activeImage.close();

    msSystem.msEEPROMs.safeStrncpy(activeFilename, filename, MAX_FILENAME_LENGTH);
    activeImage = MSImage(activeFilename);
    int w = activeImage.getWidth() * FRAME_MULTIPLY;
    shakeSync.setFrames(w);
    msSystem.logln("set frames to: ");
    msSystem.logln(String(w));
  }
};
