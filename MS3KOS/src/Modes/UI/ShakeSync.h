/*
 * MagicShifter3000 OS, ShakeSync Core Class	
 * Copyright (c) by Wizards at Work GmbH 2016-2018
 *
 * Authors: wizard23(pt), seclorum(jv), jaeh(je)
 * TODOs: 
 *          All code is conflated by headers (.h):w
 *          Code prefix- is ms*, as in msConfig, msSystem, etc.
 *
 * The purpose of this code is to provide the MS3000 user with 
 * a unique and engaging user interface, with the available LED's 
 * and button configurations, as well as additional Web-based UI
 * for configuration and interaction.
 */

//
// provide a general-purpose framework for modules to submit a "shake image"
// .. POV/Magic Images, etc. through a common interface and share-able 
// buffer.
//

#ifndef _SHAKESYNC_H
#define _SHAKESYNC_H

#define PREDICT_SAME_DIRECTION true

// TODO: fix header mess
// #include "msGlobals.h"

/* 
POVShakeSyncDummyMode provides a dummy implementation if u have to satisfy the ShakeSyncInterface
For example if you want to refactor something

It allways returns -1 (meaning of -1: out of range frame slice)
and shaking active (maybe we should make this optionable...;)
*/ 
class POVShakeSyncDummyMode {
  public:
	POVShakeSyncDummyMode(void) {
	} 

	void setFrames(int w) {
	}

	int getFrameIndex() {
		return -1;
	}

	// returns true if POV shake is actve
	bool update(float g) {
		return true;
	}
};


class POVShakeSync {
	typedef struct struct_ShakePoint {
		float g;
		int micros;				// absolute
	} ShakePoint;

	const float hysteresis = 0.4;//0.4;

	// sensitivity is the min distance between min and max to start poving
	const float sensitivity = 2.0;

	// last time it took to go from acceleration minimum to maximum/ from max to min
	int min2maxDelta, max2minDelta;
	int min2maxFrames, max2minFrames;

	// we use these to track the timestamp of the current min resp. max, might change since we are not sure we have found it yet
	ShakePoint activeMin, activeMax;
	int activeFramesMin2Max, activeFramesMax2Min;
	// last detected timestamps of the last min resp. max,
	ShakePoint lastMin, lastMax;
	int press = 0;

	// at each moment we might be either searching for the next min (then searchMin == true) or max then searchMin == false
	bool searchMin = false;

	// used to make sure the events only fire once the first time their condition is met
	bool firedPredictedZero = false;
	bool firedPredictedExtremum = false;

	int sameCount = 0, diffCount = 0;

	int frames = 16;

	int frameStartTime;

	int frameIndex = 0;
	bool isActive = false;		// is shaking happening at all?
	// are we in a shaking position that has a frame to display? Is frameIndex valid?
	bool isFrameIndexActive = false;

  public:

	 POVShakeSync(void) {
	} void setFrames(int w) {
		frames = w;
	}

	int getFrameIndex() {
		if (!digitalRead(PIN_BUTTON_B) && searchMin) {
			return -1;
		}
		if (isActive && isFrameIndexActive) {
			return frameIndex;
		} else {
			return -1;
		}
	}

	// returns true if POV shake is actve
	bool update(float g) {
		// msGlobals.ggCurrentMicros = micros();

		// frame handling
		if (isActive && isFrameIndexActive) {
			frameIndex += searchMin ? -1 : 1;
			// under or overflow
			if (frameIndex < 0 || frameIndex >= frames) {
				isFrameIndexActive = false;
			}
		} else {

			// TODO: make values configurable
			if (lastMin.micros > msGlobals.ggCurrentMicros + 500 * 1000
                //|| lastMin.g + 1. > lastMax.g
				|| lastMin.g + sensitivity > lastMax.g)  // 1.0?

				isActive = false;
			else
				isActive = true;
		}

		// workaround for strage behaviour of shake code, final fix needs moar debugging ;)
		// sometimes frameIndex remains static but isActive is not set to false by above code
		// workaround just detects the situation and resets isActive
		static int lastFrameIndex = 0;
		static int lastFrameIndexStaticTime = 0;
		if (frameIndex == lastFrameIndex) {
			lastFrameIndexStaticTime +=
				msGlobals.ggCurrentMicros - msGlobals.ggLastMicros;
		} else {
			lastFrameIndexStaticTime = 0;
		}
		lastFrameIndex = frameIndex;
		if (lastFrameIndexStaticTime > 500 * 1000) {
			isActive = false;
		}
		// msSystem.msLEDs.fillLEDs(0, 0, 0); // only enable this for debugging shaking 

		activeFramesMin2Max++;
		activeFramesMax2Min++;

		if (searchMin) {
			if (g < activeMin.g) {
				activeMin.g = g;
				activeMin.micros = msGlobals.ggCurrentMicros;
				activeFramesMin2Max = 0;
				max2minFrames = activeFramesMax2Min;
			} else {
				// TODO: timeout
			}

			// could be timeout of condition above (g < activeMin.g) then we dont need hysteresis
			if (g > (activeMin.g + hysteresis)) {
				int lastV = max2minDelta;

				//max2minFrames = activeFramesMax2Min - activeFramesMin2Max;

				lastMin = activeMin;
				max2minDelta = lastMin.micros - lastMax.micros;
				searchMin = false;
				firedPredictedZero = false;
				firedPredictedExtremum = false;

				frameStartTime =
					lastMin.micros + min2maxDelta / 2 -
					((((float) min2maxDelta) / min2maxFrames) * frames) / 2;
					//((((float) min2maxFrames) / min2maxDelta) * frames) / 2;
				// end previous frame since it wont fit anyway (we are allready moving in other dir)
				isFrameIndexActive = false;

				int dSame = max2minDelta - lastV;
				if (dSame < 0)
					dSame = -dSame;
				int dDiff = max2minDelta - min2maxDelta;
				if (dDiff < 0)
					dDiff = -dDiff;
				if (dSame <= dDiff) {
					// msSystem.slogln("same");
					sameCount++;
				} else {
					// msSystem.slogln("diff");
					diffCount++;
				}
				// msSystem.slogln(String(sameCount-diffCount));
				// msSystem.slogln(dDiff);
				// msSystem.slogln(activeMin.g);

				// reset max
				activeMax.g = g;
				activeMax.micros = msGlobals.ggCurrentMicros;

				//fillLEDs(3, 0, 3);
			}
			// last this dir or last other dir
			//else if (!firedPredictedZero && (msGlobals.ggCurrentMicros > lastMax.micros + min2maxDelta/2)) // ikes
			//else if (!firedPredictedZero && (msGlobals.ggCurrentMicros > lastMax.micros + max2minDelta/2)) // correct
			else if (!firedPredictedZero
					 && msGlobals.ggCurrentMicros > frameStartTime) {
				frameIndex = frames - 1;
				isFrameIndexActive = true;
				firedPredictedZero = true;

				//fillLEDs(5, 0, 0);
			} else if (!firedPredictedExtremum
					   && msGlobals.ggCurrentMicros >=
					   lastMax.micros + max2minDelta) {
				firedPredictedExtremum = true;
				//fillLEDs(20, 0, 0);
			}
		} else {
			if (g > activeMax.g) {
				activeMax.g = g;
				activeMax.micros = msGlobals.ggCurrentMicros;
				activeFramesMax2Min = 0;
				min2maxFrames = activeFramesMin2Max;
			} else {
			}

			if (g < (activeMax.g - hysteresis)) {
				//min2maxFrames = activeFramesMin2Max - activeFramesMax2Min;
				
				lastMax = activeMax;
				min2maxDelta = lastMax.micros - lastMin.micros;
				searchMin = true;
				firedPredictedZero = false;
				firedPredictedExtremum = false;

				frameStartTime =
					lastMax.micros + max2minDelta / 2 -
					((((float) max2minDelta) / max2minFrames) * frames) / 2;
					//((((float) max2minFrames) / max2minDelta) * frames) / 2;
				// end previous frame since it wont fit anyway (we are allready moving in other dir)
				isFrameIndexActive = false;

				// reset min
				activeMin.g = g;
				activeMin.micros = msGlobals.ggCurrentMicros;

				//fillLEDs(0, 3, 3);
			}
			//else if (!firedPredictedZero && (msGlobals.ggCurrentMicros > lastMin.micros + max2minDelta/2)) // ikes
			//else if (!firedPredictedZero && (msGlobals.ggCurrentMicros > lastMin.micros + min2maxDelta/2)) // ok
			else if (!firedPredictedZero
					 && msGlobals.ggCurrentMicros > frameStartTime) {
				frameIndex = 0;
				isFrameIndexActive = true;
				firedPredictedZero = true;
				//fillLEDs(0, 5, 0);
			}
		}

		//msSystem.msLEDs.updateLEDs(); // // only enable this for debugging shaking 

		return isActive;
	}
};

#endif
