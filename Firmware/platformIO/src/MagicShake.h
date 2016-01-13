
#include "Modes.h"
#include "Image.h"
#include "ShakeSync.h"

#define FRAME_MULTIPLY 2

#define MS_SHAKEFILE_DEFAULT "nix"

  BouncingBallMode msModeBouncingBall(600);

class MagicShakeMode : public MagicShifterBaseMode
{
private:
  char shakeFileName[MAX_FILENAME_LENGTH];
  MSImage activeImage;
  POVShakeSync shakeSync;
  int dirCursor = 0;
  int lastShakeFrame = 0;
  // QueueArray< String> dirList;

public:

  MagicShakeMode()
  {
    msSystem.msEEPROMs.safeStrncpy(shakeFileName, MS_SHAKEFILE_DEFAULT, MAX_FILENAME_LENGTH);

    // Debug
    // dirList.setPrinter(Serial);
  }

  String getFile(int fileIndex)
  {
    Dir POVDir;
    POVDir = SPIFFS.openDir("/");
    int cnt = 0;
    do {
        msSystem.log("DISP FILE:"); msSystem.logln(shakeFileName);
        if (cnt = fileIndex) return POVDir.fileName();
    } while (POVDir.next() && cnt++ && (cnt <= fileIndex));

    return String("");
  }

  void loadShakeFile(const char *filename)
  {
    msSystem.log("loadShakeFile:"); msSystem.logln(filename);

    activeImage.close();
    msSystem.msEEPROMs.safeStrncpy(shakeFileName, filename, MAX_FILENAME_LENGTH);

    activeImage.loadFile(filename);
    int w = activeImage.getWidth() * FRAME_MULTIPLY;
    shakeSync.setFrames(w);

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

    if (msSystem.msBtnPwrLongHit == true) {
      dirCursor++;
      String toLoad = getFile(dirCursor);
      if (toLoad.endsWith(".magicBitmap")) {
        msSystem.log("Would DISP:"); msSystem.logln(toLoad);
      }
      else {
        dirCursor = 0;
        msSystem.log("RESETDISP:"); msSystem.logln(toLoad);
      }
    }

    if (shakeSync.update(msGlobals.ggAccel[1]))
    {

      int index = shakeSync.getFrameIndex();

      if (index > 0)
      {

        lastShakeFrame = 0;

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
      else
      {
        yield();
      }

    }
    else
    {
        // if (lastShakeFrame > 500) {
      float fX = msGlobals.ggAccel[0];
      float fY = msGlobals.ggAccel[1];
      // msModeBouncingBall.applyForce((msGlobals.ggCurrentMicros - msGlobals.ggLastMicros) / 1000.0, fX, fY);
      msModeBouncingBall.applyForce((msGlobals.ggCurrentMicros - msGlobals.ggLastMicros) / 1000.0, fX*3);
      msModeBouncingBall.simpleBouncingBall();
      // } 
      // else
      //   lastShakeFrame++;

    }
    
  }

};
