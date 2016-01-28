

class MagicSystemTextMode : public MagicShifterBaseMode {

private:
  // MagicShifterImageAbstr *msImage;
  POVShakeSync shakeSync;
  MagicShifterImageText msMagicShakeText;

  bool  correctBrightness = false;
    Coordinate_s tPos;

public:

  void start()
  {
    // setImage(NULL);
    tPos.x = 0; tPos.y = 0;

    msMagicShakeText.plotTextString( (char *)"SYSTEM", msGlobals.tBitmap4x5, tPos);
    tPos.y += msGlobals.tBitmap4x5.header.frameHeight;
    MSColor red = {0xff,0x00,0x00};
    msGlobals.tBitmap4x5.color = red;
    msMagicShakeText.plotTextString( (char *)"TEXT", msGlobals.tBitmap4x5, tPos);
    shakeSync.setFrames(msMagicShakeText.getWidth() * FRAME_MULTIPLY);
  }

// stop the mode
  void stop(void)
  {
    shakeSync.setFrames(0);
  }

// step through a frame of the mode 
  bool step()
  {
    // check accelerometer
    if (shakeSync.update(msGlobals.ggAccel[1]))
    {
      int index = shakeSync.getFrameIndex();

      if (index > 0)
      {
        byte povData[RGB_BUFFER_SIZE];

        int frame_index = index / FRAME_MULTIPLY;

        msMagicShakeText.getFrameData(frame_index, povData);

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

      return true;
    }

    return false;
  }
};


