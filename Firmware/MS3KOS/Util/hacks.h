


#if 0 
//saveBuffer(msGlobals.web_rgb_buffer);
// TEST CODE
while (0)
{
  float voltage = msSystem.getBatteryVoltage();

  msSystem.logln(voltage);
  msSystem.logln("V");

  for (int i = 0; i < 10; i ++)
  {
    msSystem.getBatteryVoltage();
    delay(1); 
  }

  int bbb = 255;

  for (byte idx = 0; idx < MAX_LEDS; idx++)
  {
    msSystem.msLEDs.setPixels(idx, ((idx % 3)  == 0) ? bbb : 0, ((idx  % 3) == 1 ) ? bbb : 0, ((idx %  3) == 2) ? bbb : 0, 0);

    msSystem.msLEDs.setPixels((MAX_LEDS + idx - 16)%MAX_LEDS, 0, 0, 0, 0);
    msSystem.msLEDs.updatePixels();
    delay(100);
    msSystem.getBatteryVoltage();
  }

  delay(1);

   // swipe colors
  for (byte idx = 0; idx < MAX_LEDS; idx++)
  {
    msSystem.msLEDs.setPixels(idx, (idx & 1) ? bbb : 0, (idx & 2) ? bbb : 0, (idx & 4) ? bbb : 0, 0);
    msSystem.msLEDs.updatePixels();
    delay(20);
    msGlobals.msSystem.getBatteryVoltage();
  }
}
 // delay(1000)

while (1)
{
  // swipe colors
    for (byte idx = 0; idx < MAX_LEDS; idx++)
    {
      msSystem.msLEDs.setPixels(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, msGlobals.GLOBAL_GS);
      msSystem.msLEDs.updatePixels();
      delay(30);
    }
    for (byte idx = 0; idx < MAX_LEDS; idx++)
    {
      msSystem.msLEDs.setPixels(idx, 0, 0, 0, 1);
      msSystem.msLEDs.updatePixels();
      delay(30);
    }
}
  
while (0)
{
  MSImage activeImage = MSImage(msGlobals.uploadFileName);
  msSystem.logln("loaded: ");
  msSystem.logln(msGlobals.uploadFileName);

  msSystem.logln("width: ");
  msSystem.logln(activeImage.getWidth());

  for (int i = 0; i < activeImage.getWidth(); i++)
  {
    byte povData[RGB_BUFFER_SIZE];
    activeImage.readFrame(i, povData, RGB_BUFFER_SIZE);
    loadBuffer(povData);
    msSystem.msLEDs.updatePixels();
    delay(1);
  }
  activeImage.close();
} 

#endif