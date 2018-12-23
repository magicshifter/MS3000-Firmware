// Implements a Persistence-of-Vision display, deriving the data
// from uploaded .magicBitmap files
// files may be uploaded/modified/deleted by the user through the
// web interface, so we refresh our onboard POV files with every
// user event (except of course, Shaking..)


#define FRAME_MULTIPLY 2

#define MS_SHAKEFILE_DEFAULT "nix"

// We use a bouncing ball during non-shake periods as a 'screensaver' to
// nevertheless indicate that the MagicShifter is operational in this
// mode
BouncingBallMode msModeBouncingBall(600);

class MagicShakeMode:public MagicShifterBaseMode {
private:
	// The direction through the filelist which the user is scrolling (-ve/+ve)
	int dirCursor = 0;
	int colorCursor = 0;

	// the last frame of the Shake
	MagicShifterImageText msMagicShakeText;

	MagicPOVMode lPOVMode;
	MagicShifterImage lLocalImage;

	MS3KG_App_Shake &_shake = msGlobals.pbuf.apps.shake;

	// the number of files discovered onboard during the scan for POV images..
	int numFiles = 0;
	bool correctBrightness = false;

public:

	MagicShakeMode() {
		modeName = "Image";
	}
	// Get a file from the list of onboard files, filtering only .magicBitmap files// fileIndex: the idx of the file in the list// maxFiles: returns the length of the list// return: filename when found, empty string when not found

	void emit () {
		
	}

	String getFileNameAtIndex(int fileIndex, int &maxFiles) {
		Dir POVDir;
		POVDir = SPIFFS.openDir("");

		int cnt = 0;

		String foundFilename = "";
		String currentFilename = "";

		while (1) {

			if (!POVDir.next())
				break;			// end of list

			currentFilename = POVDir.fileName();

			if (!currentFilename.endsWith(".magicBitmap"))
				continue;

			if (cnt == fileIndex)
				foundFilename = currentFilename;

			cnt++;
		}

		maxFiles = cnt;

		return foundFilename;
	}

	int getIndexOf(String fileName) {
		Dir POVDir;
		POVDir = SPIFFS.openDir("");

		int cnt = 0;

		String foundFilename = "";
		String currentFilename = "";

		while (1) {
			if (!POVDir.next())
				break;			// end of list

			currentFilename = POVDir.fileName();

			if (!currentFilename.endsWith(".magicBitmap"))
				continue;

			if (currentFilename.equals(fileName))
				return cnt;

			cnt++;
		}

		return -1;
	}

	// load a magic Shake file for display
	void loadShakeFile(const char *filename) {
		msSystem.slogln(String(modeName) + " load File:" + String(filename));

		lLocalImage.close();
		lLocalImage.LoadFile(filename);
		lPOVMode.setImage(&lLocalImage);
	}

	void loadImageByIndex(int idx) {
		dirCursor = idx;
		String toLoad = getFileNameAtIndex(dirCursor, numFiles);

		// out of bounds
		if (toLoad.length() == 0) {
			dirCursor = 0;
			toLoad = getFileNameAtIndex(0, numFiles);
			if (toLoad.length() == 0)	// !J! todo: default
				toLoad = String(DEFAULT_SHAKE_IMAGE);
		}

		if (toLoad.length() > 0) {
			loadShakeFile(toLoad.c_str());
		}
	}

	void loadImageByName(String fileName) {
		int idx = getIndexOf(fileName);
		loadImageByIndex(idx);
	}

	// Start the MagicShake mode:
	//  shake the last-uploaded .magicBitmap (if set)
	//  prime the file list, which may update dynamically during our session
	void start() {
		lPOVMode.setFrameMultiplier(6);
		lPOVMode.start();
		loadImageByIndex(0);
	}

	// stop the MagicShake mode
	void stop() {
		lLocalImage.close();
		lPOVMode.stop();
	}

	void reset() {
		stop();
		start();
	}

	bool step() {
		#define MAX_BALL_COLORS 16
		int newCursor = dirCursor;
		int colorcursor = colorCursor;

		if (msSystem.msButtons.msBtnALongHit == true) {
			msSystem.msButtons.msBtnALongHit = false;
			correctBrightness = !correctBrightness;
		}

		if (msSystem.msButtons.msBtnAHit == true) {
			msSystem.msButtons.msBtnAHit = false;
			newCursor++;
		}

		if (msSystem.msButtons.msBtnBHit == true) {
			msSystem.msButtons.msBtnBHit = false;
			newCursor--;
		}

		if (msSystem.msButtons.msBtnPwrHit == true) {
			msSystem.msButtons.msBtnPwrHit = false;
			colorcursor--;
		}

		if (colorcursor < 0)
			colorcursor = MAX_BALL_COLORS;

		if (newCursor != dirCursor) {
			if (newCursor >= numFiles)
				newCursor = 0;
			if (newCursor < 0)
				newCursor = numFiles - 1;
			loadImageByIndex(newCursor);
		}
		// !J! TODO: give modes an event queue ..
		if (msGlobals.ggShouldAutoLoad == 1) {
			loadImageByName(msGlobals.ggUploadFileName);
			msGlobals.ggShouldAutoLoad = 0;
		}

		if (lPOVMode.step()) {
			return true;
		} else {

			float fX = msGlobals.ggAccel[XAXIS];
			// float fY = msGlobals.ggAccel[YAXIS];
			msModeBouncingBall.
			applyForce((msGlobals.ggCurrentMicros -
				msGlobals.ggLastMicros) / 1000.0, fX * (_shake.bounce * 1.0)); // 3

			msModeBouncingBall.simpleBouncingBall(_shake.colorIndex);

			return false;
		}

	}

};
