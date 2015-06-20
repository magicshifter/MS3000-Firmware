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

    const float hysteresis = 0.3, avgFalloff = 0.001, sensitivity = 0.4;
    float avgMin, avgMax;
    ShakePoint activeMin, activeMax;
    ShakePoint lastMin, lastMax;
  public:
    bool searchMin = false;

    bool firedPredictedZero = false;

    // sensitivity is the min distance between min and max to start poving
    POVShakeSync(void)
    {
      avgMin = avgMax = 0;
    }

    void applyForce(int micros, float g)
    {
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
          activeMin.micros = micros;
        }
        else if (g > activeMin.g + hysteresis)
        {
            lastMin = activeMin;
            searchMin = false;
			firedPredictedZero = false;

            // reset max
            activeMax.g = g;
            activeMax.micros = micros;
        }

        if (!firedPredictedZero && (micros > lastMax.micros + (lastMax.micros - lastMin.micros)/2))
        {
          firedPredictedZero = true;
		  firedPredictedZero = false;

          fillPixels(1, 0, 0);
        }
      }
      else {
        if (g > activeMax.g)
        {
          activeMax.g = g;
          activeMax.micros = micros;
        }

        if (g < activeMax.g - hysteresis)
        {
            lastMax = activeMax;
            searchMin = true;

            // reset min
            activeMin.g = g;
            activeMin.micros = micros;
        }

        if (!firedPredictedZero && (micros > lastMin.micros + (lastMin.micros - lastMax.micros)/2))
        {
          firedPredictedZero = true;

          fillPixels(0, 1, 0);
        }
      }

      updatePixels();
    }
};
