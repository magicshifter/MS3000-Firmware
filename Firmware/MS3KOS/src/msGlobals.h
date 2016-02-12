#ifndef _MSGLOBALS_H
#define _MSGLOBALS_H

#include "msTypes.h"
#include "StackClass.h"

#define DEFAULT_FACTORY_MODE 0
//
// Global definitions and the master global struct for the system
//

#define COLUMNMULTIPLY 2


void l_safeStrncpy(char *dest, const char *source, int n)
{
	strncpy(dest, source, n);
	dest[n - 1] = '\0';
}

#define FAULT_NO_ACCELEROMETER 0xf1
#define FAULT_NEW_FILEUPLOAD    0xf2

class MagicShifterGlobals {
  public:
	// state !J! TODO: clean all this up 
	long ggFault = 0;			// TODO: reset-reason/test-fault/etc.
	int ggBrightness = 3;
	float ggAccel[3];			// Stores the real accel value in g's
	int ggMagnet[3];			// Stores the 16-bit signed value
	int ggAccelCount[3];		// Stores the 12-bit signed value
	long ggAccelTime;
	long ggLFrameTime;
	long ggCurrentMode = DEFAULT_FACTORY_MODE;	// POV mode
	int ggBtn1State = 0;
	long ggCurrentMicros = 0;
	long ggLastMicros = 0;
	long ggSpeedMicros = 800;
	long ggLastFrameMicros = 0;
	long ggCurrentFrame = 0;
	long ggBootTime = 0;
	bool ggModeAP = false;
	// make it larger to be on the safe side when base64 decoding
	byte ggRGBLEDBuf[RGB_BUFFER_SIZE + 4];
	int ggShouldAutoLoad = 0;	// is there a new file to activate?
	char ggUploadFileName[MAX_FILENAME_LENGTH + 1] = DEFAULT_SHAKE_IMAGE;
	File ggUploadFile;
	long ggTime = 76500000;
	long ggTimePostedAt = 0;
	bool ggDebugSerial = true;

	struct APConfig ggAPConfig;

	MSBitmap tBitmap4x5;
	MSBitmap tBitmap6x8;
	MSBitmap tBitmap10x16;
	MSBitmap tBitmap7x12;

	enum feedbackType {
		MODE_MENU = 1,
		OK = 2,
		NOT_OK = 3
	};


};


// #else
// extern MagicShifterGlobals msGlobals;
#endif
