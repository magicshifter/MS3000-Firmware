class Accelerometer
{
  bool update();

  void setConfig();
  void getConfig();
  void defaulConfig(); // free running mode :)
  void getValues(float *accel); // 3 values
  void getValues(int *accel); // 3 values (fixed point almost raw data from sensor);

  void getAverage(int *accel);
}

class Leds
{
  void setPixelsHighRes();
  void setPixel();
  void update();
  void fastClear();
  void fastPulse(int microsShort, int microsLong);
  void getPixels();
}
