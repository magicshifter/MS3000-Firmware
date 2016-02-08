
#define NUM_SYS_TEXTS 2

class MagicSystemTextMode : public MagicShifterBaseMode {

private:
  // MagicShifterImageAbstr *msImage;
  POVShakeSync shakeSync;
  MagicShifterImageText msMagicShakeText;

  bool  correctBrightness = false;
  int sysCursor = 0;
  bool shouldBlackenBackground = true;

public:
  const char *modeName="SystemText";

  void setText(  char *label,   char *value)
  {
    MSColor aRED = {0xff,0x00,0x00};
    MSColor aBLUE = {0x00,0x00,0xFF};
    Coordinate_s tPos;

    msMagicShakeText.resetTexts();
    
    tPos.x = 0; tPos.y = 0;
    msGlobals.tBitmap4x5.color = aBLUE;
    msMagicShakeText.plotTextString( label, msGlobals.tBitmap4x5, tPos);

    tPos.y += msGlobals.tBitmap4x5.header.frameHeight;
    msGlobals.tBitmap4x5.color = aRED;

    msMagicShakeText.plotTextString( value, msGlobals.tBitmap4x5, tPos);
    shakeSync.setFrames(msMagicShakeText.getWidth() * FRAME_MULTIPLY);

    Serial.print("label:"); Serial.println(label);
    Serial.print("value:"); Serial.println(value);

  }

  void start()
  {
    setText((char *)"SYSTEM", (char *)"VALUES");
  }

// stop the mode
  void stop(void)
  {
    shakeSync.setFrames(0);
  }

// step through a frame of the mode 
  bool step()
  {

    // cycle through the texts ..
    if (msSystem.msButtons.msBtnAHit) {
      msSystem.msButtons.msBtnAHit = false; // !J! todo: button callbacks
      sysCursor++;
      if (sysCursor > 2) sysCursor = 0;
      msSystem.slog("cursor:"); msSystem.slogln(String(sysCursor));

      if (sysCursor==0){
        setText((char *)String("IP").c_str(), (char *)WiFi.localIP().toString().c_str());
      }
      else
      if (sysCursor==1){
        setText((char *)String("SOFTIP").c_str(), (char *)WiFi.softAPIP().toString().c_str());
      }
      else
      if (sysCursor==2){
        setText((char *)String("SSID").c_str(), (char *)WiFi.SSID().c_str());
      }

    }

    // check accelerometer
    if (shakeSync.update(msGlobals.ggAccel[1]))
    {
      int index = shakeSync.getFrameIndex();

      if (index > 0)
      {
        byte povData[RGB_BUFFER_SIZE];

        if (shouldBlackenBackground)
        {        // paint a background ..
          for (int i=0; i<MAX_LEDS * 4; i+=4) 
          {
            povData[i] = 0xff;
            povData[i+1] = 0x00;
            povData[i+2] = 0x00;
            povData[i+3] = 0x00;
          }
        }
        int frame_index = index / FRAME_MULTIPLY;

        msMagicShakeText.getFrameData(frame_index, povData);

        if (correctBrightness) {

          msSystem.msLEDs.loadBufferShort(povData);
          msSystem.msLEDs.updateLEDs();
          //delayMicroseconds(POV_TIME_MICROSECONDS);
          msSystem.msLEDs.loadBufferLong(povData);
          msSystem.msLEDs.updateLEDs();
        }
        else
        {
          msSystem.msLEDs.loadBuffer(povData);
          msSystem.msLEDs.updateLEDs();
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


