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

class MagicShifter
{
private:
  bool accelNeedsRefresh = true;
  bool adNeedsRefresh = true;
public:
  void setup()
  {
      pinMode(PWMGT_PIN, INPUT);
  }

  void powerDown()
  {
    pinMode(PWMGT_PIN, INPUT_PULLDOWN);
    //pinMode(PWMGT_PIN, OUTPUT);
    //digitalWrite(PWMGT_PIN, LOW);
    // now sleep forever...
    delay(1000);
  }

  void loop()
  {
    accelNeedsRefresh = true;
    adNeedsRefresh = true;
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
    return getADValue() > 890;
  }
};
