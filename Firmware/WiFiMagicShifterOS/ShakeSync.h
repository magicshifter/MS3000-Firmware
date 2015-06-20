#define PREDICT_SAME_DIRECTION true

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

    const float hysteresis = 0.4, avgFalloff = 0.001, sensitivity = 0.4;

	int min2maxDelta, max2minDelta;
    float avgMin, avgMax;
    ShakePoint activeMin, activeMax;
    ShakePoint lastMin, lastMax;
	int press = 0;

    bool searchMin = false;
    bool firedPredictedZero = false;
	bool firedPredictedExtremum = false;
	int sameCount = 0, diffCount = 0;

public: 
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
		  //fillPixels(1, 0, 0);
        }
		else
		{
			//fillPixels(0, 1, 0);
		}
        
		if (g > (activeMin.g + hysteresis))
        {			
			int lastV = max2minDelta;
            lastMin = activeMin;
			max2minDelta = lastMin.micros - lastMax.micros;
            searchMin = false;
			firedPredictedZero = false;
			firedPredictedExtremum = false;


			int dSame = max2minDelta - lastV;
			if (dSame < 0) dSame = -dSame;
			int dDiff = max2minDelta - min2maxDelta;
			if (dDiff < 0) dDiff = -dDiff;

						

			if (dSame <= dDiff) 
			{
				Serial.println("same");
				sameCount++;
			}
			else 
			{
				Serial.println("diff");
				diffCount++;
			}
			Serial.println(sameCount-diffCount);
			//Serial.println(dDiff);
			//Serial.println(activeMin.g);

            // reset max
            activeMax.g = g;
            activeMax.micros = currentMicros;

			//fillPixels(3, 0, 3);
        }
		// last this dir or last other dir 
        //else if (!firedPredictedZero && (currentMicros > lastMax.micros + min2maxDelta/2))
		else if (!firedPredictedZero && (currentMicros > lastMax.micros + max2minDelta/2))
        {
          firedPredictedZero = true;

          fillPixels(5, 0, 0);
        }
		else if (!firedPredictedExtremum && currentMicros >= lastMax.micros + max2minDelta)
		{
			firedPredictedExtremum = true;
			//fillPixels(20, 0, 0);
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
			//fillPixels(0, 1, 0);
        }
		else
		{
		}
        
		if (g < (activeMax.g - hysteresis))
        {
            lastMax = activeMax;
			min2maxDelta = lastMax.micros - lastMin.micros;
            searchMin = true;
			firedPredictedZero = false;
			firedPredictedExtremum = false;

            // reset min
            activeMin.g = g;
            activeMin.micros = currentMicros;

			//fillPixels(0, 3, 3);
        }
		//else if (!firedPredictedZero && (currentMicros > lastMin.micros + max2minDelta/2))
        else if (!firedPredictedZero && (currentMicros > lastMin.micros + min2maxDelta/2))
        {
          firedPredictedZero = true;

          fillPixels(5, 0, 0);
        }
      }

      updatePixels();
    }
};
