// Implements a Persistence-of-Vision display, deriving the data
// from uploaded .magicBitmap files
// files may be uploaded/modified/deleted by the user through the
// web interface, so we refresh our onboard POV files with every
// user event (except of course, Shaking..)
 

#define FRAME_MULTIPLY 2

#define MS_SHAKEFILE_DEFAULT "nix"

// We use a bouncing ball during non-shake periods as a 'screensaver' to
// nevertheless indicate that the MagicShifter is operational in this
// mode
BouncingBallMode msModeBouncingBall(600);

class MagicShakeMode : public MagicShifterBaseMode
{
private:
  // the sync object used to keep the Image in POV
  POVShakeSync shakeSync;

  // The direction through the filelist which the user is scrolling (-ve/+ve)
  int dirCursor = 0;

  // the last frame of the Shake
  MagicShifterImageText msMagicShakeText;

  // the number of files discovered onboard during the scan for POV images..
  int numFiles = 0;

  bool correctBrightness = false;
  MSBitmap tBitmap4x5;
  MSBitmap tBitmap6x8;
  MSBitmap tBitmap10x16;
  MSBitmap tBitmap7x12;

  bool shouldDisplayText=false;

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
    msSystem.closeActiveImage();
    msSystem.loadActiveImage(filename);
    int w = msSystem.getActiveWidth() * FRAME_MULTIPLY;
    shakeSync.setFrames(w);
  }

  void loadText()
  {

  }

  // Start the MagicShake mode:
  //  shake the last-uploaded .magicBitmap (if set)
  //  prime the file list, which may update dynamically during our session
  void start()
  {

    // 0 = picshow, 1 = textshow
    shouldDisplayText = false;

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
msSystem.log("MSMGetWidth:"); msSystem.log(String(msMagicShakeText.getWidth()));
msSystem.log("MSMGetHeight:"); msSystem.log(String(msMagicShakeText.getHeight()));

    loadText();

    MagicShifterImage::LoadBitmapFile("font4x5.magicFont", &tBitmap4x5);
    MagicShifterImage::LoadBitmapFile("font6x8.magicFont", &tBitmap6x8);
    MagicShifterImage::LoadBitmapFile("font7x12.magicFont", &tBitmap7x12);
    MagicShifterImage::LoadBitmapFile("font10x16.magicFont", &tBitmap10x16);

    Coordinate_s tPos;
    tPos.x = 0; tPos.y = 0;

    msMagicShakeText.plotTextString( (char *)"PRESS", tBitmap4x5, tPos);
    tPos.y += tBitmap4x5.header.frameHeight;
    MSColor red = {0xff,0x00,0x00};
    tBitmap4x5.color = red;
    msMagicShakeText.plotTextString( (char *)"BUTTONS", tBitmap4x5, tPos);

    shakeSync.setFrames(msMagicShakeText.getWidth() * FRAME_MULTIPLY);

#if 0
    msSystem.dumpActiveHeader(tBitmap4x5.header);
    msSystem.dumpActiveHeader(tBitmap6x8.header);
    msSystem.dumpActiveHeader(tBitmap10x16.header);
    msSystem.dumpActiveHeader(tBitmap7x12.header);
#endif

  } 

  // stop the MagicShake mode
  void stop()
  {
    msSystem.closeActiveImage();
    shakeSync.setFrames(0);
  }

  
  void step()
  {

    // toggle text
    if (msSystem.msButtons.msBtnPwrHit) {
      msSystem.msButtons.msBtnPwrHit = false; // !J! todo: button callbacks
      if (shouldDisplayText)
        shouldDisplayText=false;
      else
        shouldDisplayText=true;
    }

    // !J! TODO: give modes an event queue ..
    if (msGlobals.ggShouldAutoLoad == 1) {
      loadShakeFile(msGlobals.ggUploadFileName);
      msGlobals.ggShouldAutoLoad = 0;
    }

    if (msSystem.msButtons.msBtnALongHit == true) {
      msSystem.msButtons.msBtnALongHit = false;
      correctBrightness = !correctBrightness;
    }
    else // either a long or a short hit .. 
    if (msSystem.msButtons.msBtnAHit == true) {
      msSystem.msButtons.msBtnAHit = false;

      dirCursor++;
      if (dirCursor >= numFiles) dirCursor = 0;


      String toLoad = getFileNameAtIndex(dirCursor, numFiles);

      // out of bounds
      if (toLoad.length() == 0) { 
        dirCursor = 0;
        toLoad = getFileNameAtIndex(0, numFiles);
        if (toLoad.length() == 0) // !J! todo: default
          toLoad = String("blueghost_png.magicBitmap");
      }

      if (toLoad.length() > 0) {
        loadShakeFile(toLoad.c_str());
      }

    }

    if (msSystem.msButtons.msBtnBHit == true) {
      msSystem.msButtons.msBtnBHit = false;

      dirCursor--;
      if (dirCursor < 0) dirCursor = numFiles - 1; // !J!

      // msSystem.log("B cursor:"); msSystem.logln(String(dirCursor));

      String toLoad = getFileNameAtIndex(dirCursor, numFiles);
      // msSystem.log("Would DISP:"); msSystem.logln(toLoad);

      // out of bounds
      if (toLoad.length() == 0) { 
        dirCursor = numFiles;
        toLoad = getFileNameAtIndex(numFiles, numFiles);
        if (toLoad.length() == 0) // !J! todo: default
          toLoad = String("blueghost_png.magicBitmap");
      }

      if (toLoad.length() > 0) {
        loadShakeFile(toLoad.c_str());
      }
    }

    // msSystem.log("numFiles:"); msSystem.logln(String(numFiles));

    if (shakeSync.update(msGlobals.ggAccel[1]))
    {

      int index = shakeSync.getFrameIndex();

      if (index > 0)
      {

        byte povData[RGB_BUFFER_SIZE];
        // !J! todo: compiler do:
        for (int i=0; i<MAX_LEDS * 4; i+=4) 
        {
          povData[i] = 0xff;
          povData[i+1] = 0x00;
          povData[i+2] = 0x20;
          povData[i+3] = 0x00;

          // memset(povData, 0xff, sizeof(povData));
        }

        int frame_index = index / FRAME_MULTIPLY;

// msSystem.log("i:"); msSystem.logln(String(index));
// msSystem.log("fi:"); msSystem.logln(String(frame_index));
// frame_index = 0; // debug
// shouldDisplayText=true;

        if (shouldDisplayText) {
          msMagicShakeText.getFrameData(frame_index, povData);
        }
        else 
          msSystem.setCurrentFrame(frame_index, povData, MAX_LEDS);

        msSystem.log("shouldDisplayText:"); msSystem.logln(String(shouldDisplayText));

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

      if (1) {
        float fX = msGlobals.ggAccel[0];
        float fY = msGlobals.ggAccel[1];
        msModeBouncingBall.applyForce((msGlobals.ggCurrentMicros - msGlobals.ggLastMicros) / 1000.0, fX*3);
        msModeBouncingBall.simpleBouncingBall();

        delay(30);
      }
      else
      { // !j! text bollox
          byte txtData[RGB_BUFFER_SIZE];
          static int cIdx = 0;
          // msSystem.PlotText(NULL, "helloshifter", cIdx++, 0, txtData);
          if (cIdx > 100) cIdx = 0;

          msSystem.msLEDs.loadBuffer(txtData);

          msSystem.msLEDs.updatePixels();
          delayMicroseconds(POV_TIME_MICROSECONDS);
          msSystem.msLEDs.fastClear();
      }

    }
    
  }

};
