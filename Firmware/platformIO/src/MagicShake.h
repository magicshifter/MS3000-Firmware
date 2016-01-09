
#include "BaseMode.h"
#include "Image.h"
#include "ShakeSync.h"

#define FRAME_MULTIPLY 1

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
  POVShakeSync shakeSync;

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

  void hexDump(int len, byte *buf, const char*label) {
    msSystem.log(label); 
    msSystem.log(String(len)); 
    msSystem.log("/");

    for (int x=0;x<len;x++) {
      msSystem.log(":"); Serial.print(buf[x], HEX);; 
      if ((x) && (x % 4 == 0)) msSystem.log(" ");
    }
    msSystem.logln("<<EOF");
  }

  void step()
  {
    // !J! TODO: give modes an event queue ..
    if (msGlobals.ggShouldAutoLoad == 1) {
      loadShakeFile(msGlobals.ggUploadFileName);
      msGlobals.ggShouldAutoLoad = 0;
    }

// msSystem.log("accel:"); msSystem.logln(String(msGlobals.ggAccel[1]));

    if (shakeSync.update(msGlobals.ggAccel[1]))
    {

      int index = shakeSync.getFrameIndex();

      if (index > 0)
      {

        byte povData[RGB_BUFFER_SIZE];

        int frame_index = index / FRAME_MULTIPLY;

// msSystem.log("i:"); msSystem.logln(String(index));
// msSystem.log("fi:"); msSystem.logln(String(frame_index));

        // frame_index = 0; // debug

        activeImage.readFrame(frame_index, povData, MAX_LEDS);
        
// hexDump(RGB_BUFFER_SIZE, povData, "povData1/");

        msSystem.msLEDs.loadBuffer(povData);

// hexDump(RGB_BUFFER_SIZE, povData, "povData2/");

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
