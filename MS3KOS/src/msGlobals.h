#ifndef _MSGLOBALS_H
#define _MSGLOBALS_H

#include "msTypes.h"
// #include "StackClass.h"

#include "MS3000.pb.h"

void l_safeStrncpy(char *dest, const char *source, int n)
{
	strncpy(dest, source, n);
	dest[n - 1] = '\0';
}

class MagicShifterGlobals {
  public:
	long ggFault = 0;			// TODO: reset-reason/test-fault/etc.
	int ggBrightness = 3;
	float ggAccel[3];			// Stores the real accel value in g's
	int ggMagnet[3];			// Stores the 16-bit signed value
	int ggAccelCount[3];		// Stores the 12-bit signed value
	long ggAccelTime;
	unsigned long ggLFrameTime;
	long ggCurrentMode = DEFAULT_FACTORY_MODE;	// POV mode
	int ggBtn1State = 0;
	long ggCurrentMicros = 0;
	unsigned long ggCurrentMillis = 0;
	unsigned long ggLastMicros = 0;
	unsigned long ggLastFrameMicros = 0;
	unsigned long ggCurrentFrame = 0;
	unsigned long ggBootTime = 0;
	bool ggModeAP = false;
	// make it larger to be on the safe side when base64 decoding
	byte ggRGBLEDBuf[RGB_BUFFER_SIZE + 4];
	int ggShouldAutoLoad = 0;	// is there a new file to activate?
	char ggUploadFileName[MAX_FILENAME_LENGTH + 1] = DEFAULT_SHAKE_IMAGE;
	File ggUploadFile;
	long ggTime = 76500000;
	long ggTimePostedAt = 0;
	bool ggDebugSerial = true;
	bool ggEnableWIFI = true;

	// center button is read by AD
	int ggLastADValue;		// last AD value sampled during button step

	int batVoltCalibration  = 0;

	struct APConfig ggAPConfig;
	struct APAuthHelper  ggSoftAP;

	UIConfig ggUIConfig;			// web-influenced config for GUI values

	// set of modes
	std::vector <MagicShifterBaseMode *> ggModeList;

	MSBitmap ggtBitmap4x5;
	MSBitmap ggtBitmap6x8;
	MSBitmap ggtBitmap10x16;
	MSBitmap ggtBitmap7x12;

	enum feedbackType {
		MODE_MENU = 1,
		OK = 2,
		NOT_OK = 3
	};

	MS3000GlobalPBuffer pbuf = MS3000GlobalPBuffer_init_default;

};

// #else
// extern MagicShifterGlobals msGlobals;
#endif
