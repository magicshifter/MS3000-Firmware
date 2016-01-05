#ifndef _MSGLOBALS_H
#define _MSGLOBALS_H

class MagicShifterGlobals {
public:
  // state !J! TODO: clean all this up 
  int ggGS = 10;
  float ggAccel[3];  // Stores the real accel value in g's
  long ggAccelTime;
  long ggLFrameTime;
  int ggCurrentMode = 0;
  int ggAccelCounts[3];  // Stores the 12-bit signed value
  int ggBtn1State = 0;
  int ggCurrentMicros = 0;
  int ggLastMicros = 0;
  int ggSpeedMicros = 1000;
  long ggLastFrameMicros = 0;
  int ggCurrentFrame = 0;
  byte bright = 0xFF;
  byte gs = 0x1;
  long ggBootTime = 0;
  bool ggModeAP = false;
  // make it larger to be on the safe side when base64 decoding
  byte ggRGBLEDBuf[RGB_BUFFER_SIZE + 4];
  int ggAFileSet = 0; // is there a new file to activate?
  char ggUploadFileName[MAX_FILENAME_LENGTH + 1];
  File ggUploadFile;
  long time = 76500000;
  long ggPostTime = 0;
};

// #else
// extern MagicShifterGlobals msGlobals;
#endif
