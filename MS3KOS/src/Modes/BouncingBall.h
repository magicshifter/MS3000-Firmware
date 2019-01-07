#include <math.h>

//float PI = 3.1415;

/*
 float abs(float val)
{
  if (val < 0) return -val;
  return val;
}
//*/


class CircleBall {
  public:
	float angle;
	float velocity;
	float radius;

  public:
	 CircleBall(float r) {
		angle = .0;
		velocity = .0;
		radius = r;
	};

	float PI_HACK = 3.1415;

	float calX = 1;
	float calY = 0;

	void calibrate(float gx, float gy) {
		calX = gx;
		calY = gy;
	}

	void applyForce(float sec, float gx, float gy) {
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
		while (angle < 0)
			angle += 2 * PI_HACK;
		while (angle > 2 * PI_HACK)
			angle -= 2 * PI_HACK;
	}

	float pos = 0.5;
	float vel = 0;

	// void applyForceMB(float sec, float g)
	// {
	//   vel *= 0.99;
	//   vel += sec*g;
	//   pos += vel;

	//   if (pos < 0)
	//   {
	//     pos = 0;
	//     vel = -vel;
	//   }
	//   else if (pos > 1)
	//   {
	//     pos = 1;
	//     vel = -vel;
	//   }
	// }


	void applyForce(float sec, float g) {
		sec *= 0.5;

		angle += sec * g;
		while (angle < 0)
			angle += 2 * PI_HACK;
		while (angle > 2 * PI_HACK)
			angle -= 2 * PI_HACK;
	}

	float getLedBright(int idx, int leds) {
		float pos = leds * angle / (2 * PI_HACK);

		float delta = pos - idx;
		if (delta < 0)
			delta = -delta;

		if (delta < 1) {
			return 1 - delta;
		}

		return 0;
	}
};


class BouncingBallMode {
  public:
	float pos = 0.5;
	float vel = 0;

  public:
  	
	BouncingBallMode(float r) {};

	void flash(int start, int end) {
		int delta = 1;

		if (start > end) {
			delta = -1;
		}

		bool stopLoop = false;
		for (byte idx = start; !stopLoop; idx += delta) {
			msSystem.msLEDs.setLED(idx, 
										// default-bright white
										// 255, 255, 255,
										// safety orange
										255, 121, 0,
								   msGlobals.ggBrightness);
			msSystem.msLEDs.updateLEDs();
			delay(2);

			if (idx == end) {
				stopLoop = true;
			}
		}
	}

  public:
	int allowFlashTimer = 0;
	bool allowFlash = false;
	bool smoothLanding = false;

	const float minFlashSpeed = 1.0;
	const float maxFlashLandingSpeed = 0.1;
	const float minFastPeriod = 0.00005;
	const float flashDifficulty = 1;

	uint8_t colors[6][3] = { {255, 0, 0}, {0, 255, 160}, {255, 121, 2}, {0, 2, 1}, {1, 2, 0}, {2, 1, 0} };

	// do a simple bouncing ball .. 
	void simpleBouncingBall(int colorIndex) {

		for (byte idx = 0; idx < MAX_LEDS; idx++) {
			float scale = getLedBright(idx, MAX_LEDS);
			scale *= 0.5;
			int _cindex = colorIndex;

			if (_cindex > 5) 
				_cindex = 5;

			/*if (allowFlash && smoothLanding)
			   {
			   }
			   else
			   {
			   scale *= 0.25;
			   }
			 */

			//  msGlobals.ggBrightness = 1;
			//scale *= 10;
			//msSystem.msLEDs.setLED(idx, (msGlobals.ggCurrentFrame & 1) ? msGlobals.ggBrightness*scale : 0, (msGlobals.ggCurrentFrame & 2) ? msGlobals.ggBrightness*scale : 0, (msGlobals.ggCurrentFrame & 4) ? msGlobals.ggBrightness*scale : 0, msGlobals.gs);

			if (allowFlash) {
				if (smoothLanding) {
					msSystem.msLEDs.setLED(idx, 0, 255 * scale, 0,
										   msGlobals.ggBrightness);
				} else {

					msSystem.msLEDs.setLED(idx, 
// safety orange
255 * scale, 121 * scale, 0,
											// default bright white
											// 255 * scale, 255 * scale, 255 * scale,
											// colors[0][0], colors[0][1], colors[0][2],
											msGlobals.ggBrightness);
				}
			} else {
				msSystem.msLEDs.setLED(idx, 255 * scale, 0, 128 * scale,
									   msGlobals.ggBrightness);
			}
		}
		msSystem.msLEDs.updateLEDs();
	}

	void applyForce(float sec, float g) {

		// msSystem.slogln("bounce: " + String(bounce));
		g = -g;

		vel *= 0.9996;
		vel += sec * g * 0.004;
		pos += vel * 0.004;

		float velAbs = vel < 0 ? -vel : vel;

		if (velAbs < (minFlashSpeed * flashDifficulty)) {
			//allowFlashTimer += sec;
			//if (allowFlashTimer > minFastPeriod)
			allowFlash = true;
		} else {
			//allowFlashTimer -= sec;
			//if (allowFlashTimer < 0) allowFlashTimer = 0;
		}

		smoothLanding = velAbs < (maxFlashLandingSpeed / flashDifficulty);
		
		if (pos < 0) {
			pos = 0;
			vel = -vel * 0.9;

			if (allowFlash && smoothLanding) {
				flash(0, MAX_LEDS - 1);
				pos = 1;
				vel = 0;
			}
			allowFlash = 0;
			allowFlashTimer = 0;
		} else if (pos > 1) {
			pos = 1;
			vel = -vel * 0.9;

			if (allowFlash && smoothLanding) {
				flash(MAX_LEDS - 1, 0);
				pos = 0;
				vel = 0;
			}
			allowFlash = 0;
			allowFlashTimer = 0;
		}
	}

	float getLedBright(int idx, int leds) {
		float scale = 1;
		float i = (leds - 1) * pos;

		if (i < 0)
			i = 0;
		if (i > leds - 1)
			i = leds - 1;

		float delta = i - idx;
		if (delta < 0)
			delta = -delta;

		if (delta < 1) {
			return (1 - delta) * scale;
		}

		return 0;
	}
};
