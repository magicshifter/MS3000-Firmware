#ifndef __MAGICSHIFTER_H
#define __MAGICSHIFTER_H

#include "Config.h"

#include "MagicShifterGlobals.h"

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
class MagicShifterSystem
{
private:
  bool accelNeedsRefresh = true;
  bool adNeedsRefresh = true;

public:

  void log(String msg, int level = ERROR)
  {
    if (msGlobals.DEBUG_LEVEL <= level)
    {
      #ifdef DEBUG_SERIAL
      // msSystem.log(msg)
      #endif

  // !J!
  #define DEBUG_SYSLOG 0
  #ifdef DEBUG_SYSLOG
  WiFiUDP udp;
  #define __SYSLOG_PORT 514
      // udp.beginPacket("192.168.43.151", __SYSLOG_PORT); //NTP requests are to port 123
      udp.beginPacket("192.168.0.24", __SYSLOG_PORT); //NTP requests are to port 123
      udp.print(msg);
      udp.endPacket();
      #endif
    }
  }

  void logln(String msg, int level = ERROR)
  {
    // if (msGlobals.DEBUG_LEVEL <= level)
    // msSystem.logln(msg);
  }


void TEST_SPIFFS_bug()
{

  const char* debugPath = "XXXXX";
  uint8_t testVals[] = {1,23, 3, 7};
  uint8_t readBuffer[] = {0,0,0,0};
  //File file = SPIFFS.open((char *)debugPath.c_str(), "w");
  
  // msSystem.log("openin for w: ");
  // msSystem.logln(debugPath);
  
  File file = SPIFFS.open(debugPath, "w");

  // msSystem.log("opended for w: ");
  // msSystem.logln((bool)file);

  // msSystem.log("writin: ");
  // msSystem.logln(testVals[1]);

  file.write((uint8_t *)testVals, sizeof testVals);
  file.close();

  // msSystem.log("openin for r: ");
  // msSystem.logln(debugPath);
  
  File fileR = SPIFFS.open(debugPath, "r");

  // msSystem.log("opended for r: ");
  // msSystem.logln((bool)fileR);

  // msSystem.log("readin: ");

  fileR.read((uint8_t *)readBuffer, sizeof readBuffer);
  fileR.close();

  // msSystem.log("readback: ");
  // msSystem.logln(readBuffer[1]);
}


void logSysInfo()
{
  // // DUMP sysinfo
  // msSystem.log("Vcc: ");
  // msSystem.logln(ESP.getVcc());
  // msSystem.log("Free heap: ");
  // msSystem.logln(ESP.getFreeHeap());
  // msSystem.log("Chip ID: ");
  // msSystem.logln(ESP.getChipId());
  // msSystem.log("SDK version: ");
  // msSystem.logln(ESP.getSdkVersion());
  // msSystem.log("Boot version: ");
  // msSystem.logln(ESP.getBootVersion());
  // msSystem.log("Boot mode: ");
  // msSystem.logln(ESP.getBootMode());
  // msSystem.log("CPU freq.: ");
  // msSystem.logln(ESP.getCpuFreqMHz());
  // msSystem.log("Flash chip ID: ");
  // msSystem.logln(ESP.getFlashChipId(), HEX);
  // // gets the actual chip size based on the flash id
  // msSystem.log("Flash real size: ");
  // msSystem.logln(ESP.getFlashChipRealSize());
  // msSystem.log("Flash real size (method b): ");
  // msSystem.logln(ESP.getFlashChipSizeByChipId());
  // // gets the size of the flash as set by the compiler
  // msSystem.log("flash configured size: ");
  // msSystem.logln(ESP.getFlashChipSize());
  // if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize())
  // {
  //   // msSystem.logln("WARNING: configured flash size does not match real flash size!");
  // }
  // msSystem.log("flash speed: ");
  // msSystem.logln(ESP.getFlashChipSpeed());
  // msSystem.log("flash mode: ");
  // msSystem.logln(ESP.getFlashChipMode());
  // msSystem.log("Sketch size: ");
  // msSystem.logln(ESP.getSketchSize());
  // msSystem.log("Free sketch space: ");
  // msSystem.logln(ESP.getFreeSketchSpace());
  // msSystem.log("Reset info: ");
  // msSystem.logln(ESP.getResetInfo());
  //// msSystem.log("FS mount: ");
  //// msSystem.logln(FS.mount() ? "OK" : "ERROR!");
  
  // chercking crashes the ESP so its disabled atm
  //// msSystem.log("FS check: ");
  //// msSystem.logln(FS.check() ? "OK" : "ERROR!");

}


  void setup()
  {
    pinMode(PWMGT_PIN, INPUT);
    pinMode(PIN_LED_ENABLE, INPUT);

      // init pinmodes
    pinMode(PIN_BUTTON_A, INPUT);
    pinMode(PIN_BUTTON_B, INPUT);

    enableLeds();   // we need this for MIDI optocouplers!!

    Serial.begin(115200);
    // msSystem.logln("\r\nMagicShifter 3000 OS V0.24");

    EEPROM.begin(512);
    // accel
    InitI2C();
      // leds
    InitSPI();
    // init components
    InitAPA102();

/*
    // swipe colors
    for (byte idx = 0; idx < LEDS; idx++)
    {
      setPixel(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, msGlobals.GLOBAL_GS);
      updatePixels();
      delay(30);
    }
    for (byte idx = 0; idx < LEDS; idx++)
    {
      setPixel(idx, 0, 0, 0, 1);
      updatePixels();
      delay(30);
    }
    */
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
        // msSystem.log("longClickedButtonA", INFO);
      }
      else if (buttonAPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonA = true;
        // msSystem.log("clickedButtonA", INFO);
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
        // msSystem.log("longClickedButtonB", INFO);

      }
      else if (buttonBPressedTime >= MIN_TIME_CLICK)
      {
        clickedButtonB = true;
        // msSystem.log("clickedButtonB", INFO);
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
      msGlobals.GLOBAL_GS+=2;
      if (msGlobals.GLOBAL_GS > 31)
      {
        msGlobals.GLOBAL_GS = 31;
      }

      //msGlobals.shifterMode = (msGlobals.shifterMode+1)%MODES;
    }
    if (longClickedButtonB)
    {
      msGlobals.GLOBAL_GS-=6;
      if (msGlobals.GLOBAL_GS < 1)
      {
        msGlobals.GLOBAL_GS = 1;
      }

      //msGlobals.shifterMode = (msGlobals.shifterMode+1)%MODES;
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

  float avg = 3.2;

  float getBatteryVoltage(void)
  {
    int adValue = getADValue();
    int ad1V = 1023;



    //float r1 = 180, r2 = 390, r3 = 330; // gamma??? or (not beta)
    float r1 = 270, r2 = 1000, r3 = 0; // alpha

    float voltage = ((float)(r1 + r2 + r3) * adValue) / (r1 * ad1V);

    static float p = 0.01;
    avg = p*0.02 + voltage * (1-p);

    return avg;
  }



  bool powerButtonPressed(void)
  {
    return getADValue() > 950;
  }

  IPAddress getIP()
  {
    if (msGlobals.apMode)
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
