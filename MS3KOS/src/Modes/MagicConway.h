
class MagicConwayImage : public MagicShifterImageAbstr {

public:
	static const int width = 16;
	static const int height = 16;

	MagicConwayImage() {
		// height = width = 16;
	} 
	MagicConwayImage(const char *fileName) {
		// LoadFile(fileName);
	}

	~MagicConwayImage() {
		close();
	}

	int getWidth( ) { return width ; };
	int getHeight( ) { return height;  };

	void getFrameData(int frameIdx, byte * frameDest)
	{
		// for (int y = 0; y < height; y++)
			// frameDest[frameIdx][y] = universe[x][y] ? 0x1f : 0x00;

	};

	void close() {};

};



class MagicConwayMode : public MagicShifterBaseMode {

private:
	// MagicPOVMode lPOVMode;
	MagicConwayImage msImage;
	POVShakeSync shakeSync;

	int frameMultiplier = DEFAULT_FRAME_MULTIPLIER;

	int universe[MagicConwayImage::width][MagicConwayImage::height];

	bool correctBrightness = false;

	void setFrameMultiplier(int newMult)
	{
		if ((newMult > 0) && (newMult < MAX_FRAME_MULTIPLIER)) {
			frameMultiplier = newMult;
		}
	}

	void show_universe()
	{
		printf("-\n");

		// todo: fill in lPovMode buffer
		// todo: lPOVMode.loadBuffer();

		for (int x = 0; x < msImage.width; x++) {
			for (int y = 0; y < msImage.height; y++) 
				printf(universe[y][x] ? "1" : "0");
			printf("\n");
		}
		fflush(stdout);
	}


	void evolve_universe()
	{
		unsigned new_universe[msImage.height][msImage.width];

		for (int x = 0; x < msImage.width; x++) 
			for (int y = 0; y < msImage.width; y++) {
				int n = 0;
				for (int y1 = y - 1; y1 <= y + 1; y1++)
					for (int x1 = x - 1; x1 <= x + 1; x1++)
						if (universe[(y1 + msImage.height) % msImage.width][(x1 + msImage.width) % msImage.width])
							n++;

						if (universe[y][x]) n--;

						new_universe[y][x] = (n == 3 || (n == 2 && universe[y][x]));
			}

	
		for (int x = 0; x < msImage.width; x++) 
			for (int y = 0; y < msImage.height; y++) 
				universe[y][x] = new_universe[y][x];
	}



	void init_universe()
	{
		for (int x = 0; x < msImage.width; x++)
		for (int y = 0; y < msImage.height; y++)
			universe[y][x] = rand() < RAND_MAX / 10 ? 1 : 0;
	}

	void step_universe()
	{
		show_universe();
		evolve_universe();
	}

public:
	MagicConwayMode() {
		modeName = "life";
	}

	virtual void stop(void) {
		// lPOVMode.setImage(NULL);
	}

	virtual void start() {
		for(int i=0;i<RGB_BUFFER_SIZE;i+=4) {
			msGlobals.ggRGBLEDBuf[i] = msGlobals.ggBrightness | 0xe0;
			msGlobals.ggRGBLEDBuf[i+1] = i * 4;
			msGlobals.ggRGBLEDBuf[i+2] = 255 - i * 16;
			msGlobals.ggRGBLEDBuf[i+3] = 0;
		}
// lPOVMode.setFrameMultiplier(2);
		init_universe();

	}

	virtual bool step(void) {
		msSystem.msLEDs.loadBuffer(msGlobals.ggRGBLEDBuf);
		msSystem.msLEDs.updateLEDs();

// lPOVMode.setImage();
		step_universe();

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


		// lPOVMode.step();

		msSystem.msLEDs.updateLEDs();

		delay(10);

		return true;
	}


};
