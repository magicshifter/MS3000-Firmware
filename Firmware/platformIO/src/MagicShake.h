// Implements a Persistence-of-Vision display, deriving the data
// from uploaded .magicBitmap files
// files may be uploaded/modified/deleted by the user through the
// web interface, so we refresh our onboard POV files with every
// user event (except of course, Shaking..)
 
#include "Modes.h"
#include "Image.h"

#define FRAME_MULTIPLY 2

#define MS_SHAKEFILE_DEFAULT "nix"

// We use a bouncing ball during non-shake periods as a 'screensaver' to
// nevertheless indicate that the MagicShifter is operational in this
// mode
BouncingBallMode msModeBouncingBall(600);

class MagicShakeMode : public MagicShifterBaseMode
{
private:
  // the currently active shake Image
  MSImage activeImage;

  // the sync object used to keep the Image in POV
  POVShakeSync shakeSync;

  // The direction through the filelist which the user is scrolling (-ve/+ve)
  int dirCursor = 0;

  // the last frame of the Shake
  MagicShakeText msModeShakeText;

  // the number of files discovered onboard during the scan for POV images..
  int numFiles = 0;

  bool correctBrightness = false;


public:

  MagicShakeMode()
  {
  }


  // Get a file from the list of onboard files, filtering only .magicBitmap files
  // fileIndex: the idx of the file in the list
  // maxFiles: returns the length of the list
  // return: filename when found, empty string when not found
  String getFileNameAtIndex(int fileIndex, int &maxFiles)
  {
    Dir POVDir;
    msSystem.log("getFileNameAtIndex:"); msSystem.logln(String(fileIndex));
    POVDir = SPIFFS.openDir("");

    int cnt = 0;

    while(cnt <= fileIndex)
    {
      if (!POVDir.next()) break; // end of list
      String foundFilename;
      foundFilename = POVDir.fileName();
      if (!foundFilename.endsWith(".magicBitmap")) continue;

      if (cnt == fileIndex)
        return foundFilename;

      cnt++;
    }

    maxFiles = cnt;

    return "";
  }

  // load a magic Shake file for display
  void loadShakeFile(const char *filename)
  {
    msSystem.log("loadShakeFile:"); msSystem.logln(filename);
    activeImage.close();

    activeImage.loadFile(filename);
    int w = activeImage.getWidth() * FRAME_MULTIPLY;
    shakeSync.setFrames(w);

  }

  // Start the MagicShake mode:
  //  shake the last-uploaded .magicBitmap (if set)
  //  prime the file list, which may update dynamically during our session
  void start()
  {
    if (String(msGlobals.ggUploadFileName).endsWith(".magicBitmap")) {
      loadShakeFile(msGlobals.ggUploadFileName);
    }
    else {
      loadShakeFile(DEFAULT_SHAKE_IMAGE); // !J! todo: move to default ..
    }

    // prime numFiles at Start
    dirCursor = 999999;// !J! grr ..
    getFileNameAtIndex(dirCursor, numFiles);
    msSystem.log("numFiles:"); msSystem.logln(String(numFiles));
    dirCursor = 0;// !J! grr ..
  } 

  // stop the MagicShake mode
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

// msSystem.log("accel:"); msSystem.logln(String(msGlobals.ggAccel[1]));

    if (msSystem.msButtons.msBtnAHit == true) {
      msSystem.msButtons.msBtnAHit = false;

      dirCursor++;
      if (dirCursor >= numFiles) dirCursor = 0;

      msSystem.log("A cursor:"); msSystem.logln(String(dirCursor));

      String toLoad = getFileNameAtIndex(dirCursor, numFiles);
      msSystem.log("Would DISP:"); msSystem.logln(toLoad);

      // out of bounds
      if (toLoad.length() == 0) { 
        msSystem.log("RESETDISP:"); msSystem.logln(toLoad);
        dirCursor = 0;
        toLoad = getFileNameAtIndex(0, numFiles);
        if (toLoad.length() == 0) // !J! todo: default
          toLoad = String("blueghost_png.magicBitmap");
      }


      if (toLoad.length() > 0) {
msSystem.log("Would DISP:"); msSystem.logln(toLoad);
        loadShakeFile(toLoad.c_str());
      }

    }

    if (msSystem.msButtons.msBtnBHit == true) {
      msSystem.msButtons.msBtnBHit = false;

      dirCursor--;
      if (dirCursor < 0) dirCursor = numFiles - 1; // !J!

      msSystem.log("B cursor:"); msSystem.logln(String(dirCursor));

      String toLoad = getFileNameAtIndex(dirCursor, numFiles);
      msSystem.log("Would DISP:"); msSystem.logln(toLoad);

      // out of bounds
      if (toLoad.length() == 0) { 
        msSystem.log("RESETDISP:"); msSystem.logln(toLoad);
        dirCursor = numFiles;
        toLoad = getFileNameAtIndex(numFiles, numFiles);
        if (toLoad.length() == 0) // !J! todo: default
          toLoad = String("blueghost_png.magicBitmap");
      }


      if (toLoad.length() > 0) {
msSystem.log("Would DISP:"); msSystem.logln(toLoad);
        loadShakeFile(toLoad.c_str());
      }
    }

    if (msSystem.msButtons.msBtnALongHit == true) {
      msSystem.msButtons.msBtnALongHit = false;

      correctBrightness = !correctBrightness;
    }

    // msSystem.log("numFiles:"); msSystem.logln(String(numFiles));

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

        if (correctBrightness) {

          msSystem.msLEDs.loadBufferShort(povData);
          msSystem.msLEDs.updatePixels();
          //delayMicroseconds(POV_TIME_MICROSECONDS);
          msSystem.msLEDs.loadBufferLong(povData);
          msSystem.msLEDs.updatePixels();
        }
        else
        {
          msSystem.msLEDs.loadBuffer(povData);
          msSystem.msLEDs.updatePixels();
          delayMicroseconds(POV_TIME_MICROSECONDS);
          msSystem.msLEDs.fastClear();
        }

      }
      else
      {
        msSystem.msLEDs.fastClear();
        yield();
      }

    }
    else
    {
      float fX = msGlobals.ggAccel[0];
      float fY = msGlobals.ggAccel[1];
      msModeBouncingBall.applyForce((msGlobals.ggCurrentMicros - msGlobals.ggLastMicros) / 1000.0, fX*3);
      msModeBouncingBall.simpleBouncingBall();

      delay(30);

      // static int cIdx = 0;
      // msModeShakeText.PlotText(NULL, "helloshifter", cIdx++, 0);
      // if (cIdx > 100) cIdx = 0;

      // msSystem.msLEDs.updatePixels();
      // delayMicroseconds(POV_TIME_MICROSECONDS);
      // msSystem.msLEDs.fastClear();

    }
    
  }

};
