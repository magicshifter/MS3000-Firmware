
#include "BaseMode.h"
#include "Image.h"
#include "ShakeSync.h"

#define FRAME_MULTIPLY 4

#define MS_SHAKEFILE_DEFAULT "nix"

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
  char shakeFileName[MAX_FILENAME_LENGTH];
  MSImage activeImage;
  POVShakeSyncDummyMode shakeSync;

public:
  MagicShakeMode()
  {
    msSystem.msEEPROMs.safeStrncpy(shakeFileName, MS_SHAKEFILE_DEFAULT, MAX_FILENAME_LENGTH);

  }

  void start()
  {
    loadShakeFile(msGlobals.ggUploadFileName);
  } // todo: startActiveFile() with a default filename

  void stop()
  {
    activeImage.close();
    shakeSync.setFrames(0);
  }

  void step()
  {
    // !J! TODO: give modes an event queue ..
    if (msGlobals.ggShouldAutoLoad == 1) {
      loadShakeFile(msGlobals.ggUploadFileName);
      msGlobals.ggShouldAutoLoad = 0;
    }

    if (shakeSync.update(msGlobals.ggAccel[2]))
    {
      int index = shakeSync.getFrameIndex();

      if (index > 0)
      {

        byte povData[RGB_BUFFER_SIZE];

        activeImage.readFrame(index / FRAME_MULTIPLY, povData, 16);
        msSystem.msLEDs.loadBuffer(povData);
        msSystem.msLEDs.updatePixels();

        delayMicroseconds(POV_TIME_MICROSECONDS);
        msSystem.msLEDs.fastClear();
      }

    }
    
  }

  void loadShakeFile(char *filename)
  {
    activeImage.close();

    msSystem.msEEPROMs.safeStrncpy(shakeFileName, filename, MAX_FILENAME_LENGTH);

    activeImage.loadFile(filename);

    int w = activeImage.getWidth() * FRAME_MULTIPLY;
    
    shakeSync.setFrames(w);

  }
};
