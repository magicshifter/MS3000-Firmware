
#define DEFAULT_FRAME_MULTIPLIER 4
#define MAX_FRAME_MULTIPLIER 12 

class MagicPOVMode:public MagicShifterBaseMode {

  private:
	MagicShifterImageAbstr * msImage = NULL;
	POVShakeSync shakeSync;
	bool correctBrightness = false;
	int frameMultiplier = DEFAULT_FRAME_MULTIPLIER;

  public:

  	void setFrameMultiplier(int newMult)
  	{
  		if ((newMult > 0) && (newMult < MAX_FRAME_MULTIPLIER)) {
  			frameMultiplier = newMult;
	  	}
	}

	void setImage(MagicShifterImageAbstr * lImage) {
		// if (msImage != NULL) 
		//   msImage->close();

		msImage = lImage;

		if (lImage != NULL) {
			shakeSync.setFrames(msImage->getWidth() * frameMultiplier);
		} else
			 shakeSync.setFrames(0);
	}

	void start() {
		// setImage(NULL);
	}

	// stop the mode
	void stop(void) {
		shakeSync.setFrames(0);
	}

	// step through a frame of the magic light mode
	// in this case we will attempt these dynamics:
	// a) just resting, a kind of 'spirit-level'
	// b) if the user shakes, we submit the POV image selected
	bool step() {
		if (msImage != NULL) {
			// check accelerometer
			if (shakeSync.update(msGlobals.ggAccel[YAXIS])) {
				int index = shakeSync.getFrameIndex();

				if (index > 0) {
					byte povData[RGB_BUFFER_SIZE];

					int frame_index = index / frameMultiplier;

					msImage->getFrameData(frame_index, povData);

					if (correctBrightness) {

						msSystem.msLEDs.loadBufferShort(povData);
						msSystem.msLEDs.updateLEDs();
						//delayMicroseconds(POV_TIME_MICROSECONDS);
						msSystem.msLEDs.loadBufferLong(povData);
						msSystem.msLEDs.updateLEDs();
					} else {
						msSystem.msLEDs.loadBuffer(povData);
						msSystem.msLEDs.updateLEDs();
						delayMicroseconds(msGlobals.ggBrightness * 16);

						if (msGlobals.ggBrightness < 31)
							msSystem.msLEDs.fastClear();
					}
				} else {
					msSystem.msLEDs.fastClear();
					yield();
				}
				return true;
			} else
				return false;
		}
		return false;
	}
};
