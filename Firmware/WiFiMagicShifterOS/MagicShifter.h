#ifndef __MAGICSHIFTER_H
#define __MAGICSHIFTER_H

#include "Config.h"

extern bool apMode;
extern int shifterMode;

class Accelerometer
{
  bool update();

  void setConfig();
  void getConfig();
  void defaulConfig(); // free running mode :)
  void getValues(float *accel); // 3 values
  void getValues(int *accel); // 3 values (fixed point almost raw data from sensor);

  void getAverage(int *accel);
};

class Leds
{
  void setPixelsHighRes();
  void setPixel();
  void update();
  void fastClear();
  void fastPulse(int microsShort, int microsLong);
  void getPixels();
};

// TODO: all init and all sensors and leds in here :)
// (accelerometer wuld also be a class but the MAgicShifter object has one ;)
class MagicShifter
{
private:
  bool accelNeedsRefresh = true;
  bool adNeedsRefresh = true;
public:
  void setup()
  {
    pinMode(PWMGT_PIN, INPUT);
    pinMode(PIN_LED_ENABLE, INPUT);

      // init pinmodes
    pinMode(PIN_BUTTON_A, INPUT);
    pinMode(PIN_BUTTON_B, INPUT);

    enableLeds();   // we need this for MIDI optocouplers!!

    Serial.begin(115200);
    Serial.println("\r\nMagicShifter 3000 OS V0.24");

    EEPROM.begin(512);
    // accel
    InitI2C();
      // leds
    InitSPI();
    // init components
    InitAPA102();

    // swipe colors
    for (byte idx = 0; idx < LEDS; idx++)
    {
      setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, GLOBAL_GS);
      updatePixels();
      delay(30);
    }
    for (byte idx = 0; idx < LEDS; idx++)
    {
      setPixel(idx, 0, 0, 0, 1);
      updatePixels();
      delay(30);
    }
  }

  void powerDown()
  {
    // works even with pulldown but output seems to make more sense
    //pinMode(PWMGT_PIN, INPUT_PULLDOWN);
    pinMode(PWMGT_PIN, OUTPUT);
    digitalWrite(PWMGT_PIN, LOW);
    // now sleep forever...
    delay(1000);
  }

  void restart()
  {
    ESP.restart();
  }

  void enableLeds()
  {
    pinMode(PIN_LED_ENABLE, OUTPUT);
    digitalWrite(PIN_LED_ENABLE, HIGH);
  }

  void disableLeds()
  {
    pinMode(PIN_LED_ENABLE, INPUT);
  }

  int currentTimeStamp = 0;
  int lastTimeStamp = 0;
  int microsSinceLast = 0;

  void loop()
  {
    //delay(0);
    lastTimeStamp = currentTimeStamp;
    currentTimeStamp = micros();
    microsSinceLast = currentTimeStamp - lastTimeStamp;

    accelNeedsRefresh = true;
    adNeedsRefresh = true;
    handleButtons();
    //delay(0);
  }

  int bFrame = 0;

// TODO: private state
// state for button timing
  int buttonAPressedTime = 0;
  int buttonPowerPressedTime = 0;
  int buttonBPressedTime = 0;

// state for double click timing
  int timeToLastClickedButtonA = 0;
  int timeToLastClickedButtonPower = 0;
  int timeToLastClickedButtonB = 0;

  bool m_enableLongClicks = true;

// todo public properties? Logic for consuming buttons?
// events for consumers true/false;
  bool clickedButtonA = false;
  bool clickedButtonPower = false;
  bool clickedButtonB = false;

  bool longClickedButtonA = false;
  bool longClickedButtonPower = false;
  bool longClickedButtonB = false;
  
  bool doubleClickedButtonA = false;
  bool doubleClickedButtonPower = false;
  bool doubleClickedButtonB = false;

  void handleButtons()
  {
    // reset public btton state
    clickedButtonA = longClickedButtonA = false;
    if (!digitalRead(PIN_BUTTON_A))
    {
      if (buttonAPressedTime)
        buttonAPressedTime += microsSinceLast;
      else
        buttonAPressedTime = 1;
    }
    else
    {
      if (m_enableLongClicks && buttonAPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonA = true;
        log("longClickedButtonA", INFO);
      }
      else if (buttonAPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonA = true;
        log("clickedButtonA", INFO);
      }

      buttonAPressedTime = 0;
    }


    // reset public btton state
    clickedButtonB = longClickedButtonB = false;
    if (!digitalRead(PIN_BUTTON_B))
    {
      if (buttonBPressedTime)
        buttonBPressedTime += microsSinceLast;
      else
        buttonBPressedTime = 1;
    }
    else
    {
      if (m_enableLongClicks && buttonBPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonB = true;
        log("longClickedButtonB", INFO);

      }
      else if (buttonBPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonB = true;
        log("clickedButtonB", INFO);
      }

      buttonBPressedTime = 0;
    }


    // reset public btton state
    clickedButtonPower = longClickedButtonPower = false;
/*
    if (bFrame++ % 10 == 0)
    if (powerButtonPressed())
    {
      if (buttonPowerPressedTime)
        buttonPowerPressedTime += microsSinceLast;
      else
        buttonPowerPressedTime = 1;
    }
    else
    {
      if (buttonPowerPressedTime >= MIN_TIME_LONG_CLICK)
      {
        longClickedButtonPower = true;
      }
      else if (buttonPowerPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonPower = true;
      }

      buttonPowerPressedTime = 0;
    }
    //*/

// internal button usage
    if (longClickedButtonA)
    {
      powerDown();
    }

    if (clickedButtonB)
    {
      GLOBAL_GS+=2;
      if (GLOBAL_GS > 31)
      {
        GLOBAL_GS = 31;
      }

      //shifterMode = (shifterMode+1)%MODES;
    }
    if (longClickedButtonB)
    {
      GLOBAL_GS-=6;
      if (GLOBAL_GS < 1)
      {
        GLOBAL_GS = 1;
      }

      //shifterMode = (shifterMode+1)%MODES;
    }
  }

  void enableLongClicks(bool enable)
  {
    m_enableLongClicks = enable;
  }

  int getADValue(void)
  {
    return analogRead(A0);
  }

  float getBatteryVoltage(void)
  {
    int adValue = getADValue();
    int ad1V = 1023;
    float r1 = 180, r2 = 390, r3 = 330;
    float voltage = ((float)(r1 + r2 + r3) * adValue) / (r1 * ad1V);
    return voltage;
  }

  bool powerButtonPressed(void)
  {
    return getADValue() > 950;
  }

  IPAddress getIP()
  {
    if (apMode)
    {
      return WiFi.softAPIP();
    }
    else
    {
      return WiFi.localIP();
    }
  }
};
#endif
