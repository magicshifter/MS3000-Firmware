class ModeSelectorMode : public MagicShifterBaseMode {

private:
  // MagicShifterImageAbstr *msImage;
  POVShakeSync shakeSync;
  MagicShifterImageText msMagicShakeText;
  int currentIdx = 0;

public:
  const int maxModes = 5;
  char *modeNames[5]= {"Magic", "Remote", "RGB", "SysVals",  "Compass"};


  void setText(  char *label)
  {
    MSColor aRED = {0xff,0x00,0x00};
    MSColor aWhile = {0xFF,0xFF,0xFF};
    Coordinate_s tPos;

    msMagicShakeText.resetTexts();
    
    tPos.x = 0; tPos.y = 0;
    msGlobals.tBitmap6x8.color = aWhile;
    msMagicShakeText.plotTextString( label, msGlobals.tBitmap6x8, tPos);

    shakeSync.setFrames(msMagicShakeText.getWidth() * FRAME_MULTIPLY);
  }

  void start()
  {
    setIndex(0);
  }

// stop the mode
  void stop(void)
  {
    shakeSync.setFrames(0);
  }

  void setIndex(int idx) {
    if (idx < 0) {
      idx = maxModes - 1; 
    }
    if (idx >= maxModes) {
      idx = 0;
    }
    currentIdx = idx;
    setText(modeNames[currentIdx]);
  }

// step through a frame of the mode 
  int step()
  {
    int index = currentIdx;
    int posIdx = index;

    // button handling
    if (msSystem.msButtons.msBtnPwrHit) {
      for (int i = 0; i < 4; i++) {
        msSystem.msLEDs.setLED(posIdx, 255, 255, 255, msGlobals.ggBrightness);
        msSystem.msLEDs.updateLEDs();
        delay(50);
        msSystem.msLEDs.setLED(posIdx, 0, 0, 0);
        msSystem.msLEDs.updateLEDs();
        delay(50);
      }
      return index;
    }

    // cycle through the texts ..
    if (msSystem.msButtons.msBtnAHit) {
      msSystem.msButtons.msBtnAHit = false; // !J! todo: button callbacks
      setIndex(currentIdx + 1);
    }

    // cycle through the texts ..
    if (msSystem.msButtons.msBtnBHit) {
      msSystem.msButtons.msBtnBHit = false; // !J! todo: button callbacks
      setIndex(currentIdx - 1);
    }

    // check accelerometer
    if (shakeSync.update(msGlobals.ggAccel[1]))
    {
      int index = shakeSync.getFrameIndex();

      if (index > 0)
      {
        byte povData[RGB_BUFFER_SIZE];

       
        for (int i=0; i<MAX_LEDS * 4; i+=4) 
        {
          povData[i] = 0xff;
          povData[i+1] = 0x00;
          povData[i+2] = 0x00;
          povData[i+3] = 0x00;
        }
        
        int frame_index = index / FRAME_MULTIPLY;

        msMagicShakeText.getFrameData(frame_index, povData);

        msSystem.msLEDs.loadBuffer(povData);
        msSystem.msLEDs.updateLEDs();
        delayMicroseconds(POV_TIME_MICROSECONDS);
        msSystem.msLEDs.fastClear();
      }
      else
      {
        msSystem.msLEDs.fastClear();
        yield();
      }
    }
    else 
    {
      int colIdx = 1 + (index % 7);
      int r = (colIdx >> 0) % 2 ? 255 : 0;
      int g = (colIdx >> 1) % 2 ? 255 : 0;
      int b = (colIdx >> 2) % 2 ? 255 : 0;

      for (byte i = 0; i < 16; i++) {
        if (i == posIdx) {
          msSystem.msLEDs.setLED(i, r, g, b, msGlobals.ggBrightness);
        } 
        else {
          msSystem.msLEDs.setLED(i, 0, 0, 0);
        }
      }

      msSystem.msLEDs.updateLEDs();
    }

    return -1;
  }
};


