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

    const float hysteresis = 0.01;
    float avgMin, avgMax;
    ShakePoint activeMin, activeMax;
    bool searchMin;

    // sensitivity is the min distance between min and max to start poving
    ShakeSync(float sensitivity)
    {
      activeMin = activeMax = avgMin = avgMax = 0;
    }

    void applyForce(int micros, float g)
    {

      if (g > avgMax) avgMax = g;
      if (g < avgMin) avgMin = g;

      if (searchMin)
      {
        if (g < activeMin.g)
        {
          activeMin.g = g;
          activeMin.micros = micros;
        }

        if ()
      }
    }
}
