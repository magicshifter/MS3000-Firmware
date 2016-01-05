// tests.h
void testAccelerometer()
{
  Serial.println("trying accel!");

  int d = 500;
  int b = 20;
  while (1)
  {
    if (msSystem.accelerometerWorking) 
    {
      msSystem.msLEDs.fillPixels(0, b, 0, 0xff);
    }
    else
    {
     msSystem.msLEDs.fillPixels(b, 0, 0, 0xff);
   }
   msSystem.msLEDs.updatePixels();
   delay(d);

   msSystem.msLEDs.fillPixels(b, b, b, 0xff);
   msSystem.msLEDs.updatePixels();
   delay(d);
 }
}


void testButtonForBOM_X()
{
  if (msGlobals.currentFrame % 1000 == 0)
  {
    msSystem.logln("_");
  }

  if (msSystem.longClickedButtonPower)
  {
    msSystem.msLEDs.setPixels(1, 0, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.clickedButtonPower)
  {
    msSystem.msLEDs.setPixels(1, 20, 20, 0, 15);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.longClickedButtonA)
  {
    msSystem.msLEDs.setPixels(0, 20, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.clickedButtonA)
  {
    msSystem.msLEDs.setPixels(0, 20, 20, 0, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.longClickedButtonB)
  {
    msSystem.msLEDs.setPixels(2, 20, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.clickedButtonB)
  {
    msSystem.msLEDs.setPixels(2, 20, 20, 0, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
}
