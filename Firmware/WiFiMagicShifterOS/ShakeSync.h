class POVShakeSync {
  /*
    void applyForce(float sec, float gx, float gy)
    {
      sec *= 0.5;
      float posx = cos(angle);
      float posy = sin(angle);

      float accel = -posy * gx + posx * gy;

      //accel = -accel;
      velocity += sec * accel;

      velocity *= 0.995;

      angle += sec * velocity / radius;
      while (angle < 0) angle += 2*PI_HACK;
      while (angle > 2*PI_HACK) angle -= 2*PI_HACK;
    }
    */

    typedef struct struct_ShakePoint
    {
      float g;
      int micros; // absolute
    } ShakePoint;

    const float hysteresis = 0.5, avgFalloff = 0.001, sensitivity = 0.4;
    float avgMin, avgMax;
    ShakePoint activeMin, activeMax;
    ShakePoint lastMin, lastMax;
	int press = 0;

  public:
    bool searchMin = false;

    bool firedPredictedZero = false;

    // sensitivity is the min distance between min and max to start poving
    POVShakeSync(void)
    {
      avgMin = avgMax = 0;
    }

    void applyForce(float g)
    {
	  int currentMicros = micros();
      fillPixels(0, 0, 0);


      avgMax -= avgFalloff;
      avgMin += avgFalloff;
      if (g > avgMax) avgMax = g;
      if (g < avgMin) avgMin = g;

      if (searchMin)
      {
        if (g < activeMin.g)
        {
          activeMin.g = g;
          activeMin.micros = currentMicros;
        }
        
		if (g > (activeMin.g + hysteresis))
        {
			Serial.println("min");
			Serial.println(lastMax.micros - lastMin.micros);
			Serial.println(currentMicros - lastMax.micros);
			Serial.println(activeMin.micros);
            lastMin = activeMin;
			Serial.println(lastMin.micros);
            searchMin = false;
			firedPredictedZero = false;

            // reset max
            activeMax.g = g;
            activeMax.micros = currentMicros;

			fillPixels(3, 0, 3);
        }
        else if (!firedPredictedZero && (currentMicros > lastMax.micros + (lastMax.micros - lastMin.micros)/2))
        {
          firedPredictedZero = true;

          fillPixels(5, 0, 0);
        }
		
		if (!digitalRead(PIN_BUTTON1))
  		{
			press++;
		}
		else {
			if (press > 30)
			{
				Serial.println("x");
				Serial.println(lastMax.micros - lastMin.micros);
				Serial.println(currentMicros - lastMax.micros);
				/*Serial.println(currentMicros);
				Serial.println(lastMax.micros);
				Serial.println(lastMin.micros);
				Serial.println(lastMax.micros + (lastMax.micros - lastMin.micros)/2);
*/
			}
			press = 0;
		}
      }
      else {
        if (g > activeMax.g)
        {
          activeMax.g = g;
          activeMax.micros = currentMicros;
        }
        
		if (g < (activeMax.g - hysteresis))
        {
            lastMax = activeMax;
            searchMin = true;
			firedPredictedZero = false;

            // reset min
            activeMin.g = g;
            activeMin.micros = currentMicros;

			fillPixels(0, 3, 3);
        }
        else if (!firedPredictedZero && (currentMicros > lastMin.micros + (lastMin.micros - lastMax.micros)/2))
        {
          firedPredictedZero = true;

          fillPixels(0, 5, 0);
        }
      }

      updatePixels();
    }
};
