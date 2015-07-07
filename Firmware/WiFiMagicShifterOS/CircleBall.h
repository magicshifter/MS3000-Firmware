#include <math.h>

//float PI = 3.1415;

/*
 float abs(float val)
{
  if (val < 0) return -val;
  return val;
}
//*/


class CircleBall
{
  public:
  float angle;
  float velocity;
  float radius;

  public:
  CircleBall(float r)
  {
    angle = .0;
    velocity = .0;
    radius = r;
  }

  float PI_HACK = 3.1415;

  float calX = 1;
  float calY = 0;

  void calibrate(float gx, float gy)
  {
      calX = gx;
      calY = gy;
  }

  void applyForce(float sec, float gx, float gy)
  {
    sec *= 0.5;
    float posx = cos(angle);
    float posy = sin(angle);

    float px = gx * calX + gy * calY;
    float py = gx * calY - gy * calX;
    float accel = -posy * px + posx * py;

    //float accel = -posy * gx + posx * gy;

    //accel = -accel;
    velocity += sec * accel;

    velocity *= 0.995;

    angle += sec * velocity / radius;
    while (angle < 0) angle += 2*PI_HACK;
    while (angle > 2*PI_HACK) angle -= 2*PI_HACK;
  }

  void applyForce(float sec, float g)
  {
    sec *= 0.5;

    angle += sec * g;
    while (angle < 0) angle += 2*PI_HACK;
    while (angle > 2*PI_HACK) angle -= 2*PI_HACK;
  }

  float getLedBright(int idx, int leds)
  {
    float pos = leds * angle / (2*PI_HACK);

    float delta = pos-idx;
    if (delta < 0) delta = -delta;

    if (delta < 1)
    {
      return 1 - delta;
    }

    return 0;
  }
};
