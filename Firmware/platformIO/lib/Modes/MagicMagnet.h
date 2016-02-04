#ifndef _MAGNETOMETER_MODE_H
#define _MAGNETOMETER_MODE_H

//
// render the magnetometer sensor data
// 

class MagicMagnetMode : public MagicShifterBaseMode 
{
private:
  int frame = 0;

  public:
    void start()
    {
    }

    void stop(void)
    {
    }

    bool step()
    {
      static  int autoCalResetCounter = 0;

      frame++;
      // AccelPoll();
                  
      msSystem.msSensor.readMagnetometerData(msGlobals.ggMagnet);

      if ((msSystem.msSensor.readRegister(0x5E) & 0x02)) {
        msSystem.logln("magnetometer:go!");
        autoCalResetCounter++;
        if (autoCalResetCounter > 10) {
          msSystem.logln("magnetometer:reset");
          // M_CTRL_REG2: Hybrid auto increment, Magnetic measurement min/max detection function reset
          msSystem.msSensor.writeRegister(0x5C, 0x24);
          autoCalResetCounter = 0;
        }
      }
      else {
        autoCalResetCounter = 0;
      }

      //    msSystem.msLEDs.fillPixels(frame % 300 == 0 ? v : 0, frame % 300 == 100 ? v : 0, frame % 300 == 200 ? v : 0);
      //    msSystem.msLEDs.setPixels(15, centerBtnPressed ? 100 : 0, powerBtnPressed ? 100 : 0, 0);
      //    updateLedsWithBlank();

      // if (!MagicShifter_Poll()) break;
      
      int degrees = int(atan2(msGlobals.ggMagnet[YAXIS], -msGlobals.ggMagnet[XAXIS]) * 180 / M_PI);
      msSystem.msLEDs.fillPixels(0,0,0);
      int lednr = map(abs(degrees), 0, 180, 0, 15);
      msSystem.msLEDs.setPixels(lednr, 0, 0, 255, msGlobals.ggBrightness);

      for (int lC=0;lC<lednr;lC++) 
        msSystem.msLEDs.setPixels(lC, 0, 255, 0, msGlobals.ggBrightness / 2); // !J! hack

      for (int lC=lednr+1;lC<MAX_LEDS;lC++) 
        msSystem.msLEDs.setPixels(lC, 255, 0, 0, msGlobals.ggBrightness / 2); // !J! hack

      // msSystem.log("LED: "); msSystem.logln(String(lednr));

      // updateLedsWithBrightness();
      msSystem.msLEDs.updatePixels();

      delay(75);

    }

  void update() {};
  void reset() {};

};

#endif
