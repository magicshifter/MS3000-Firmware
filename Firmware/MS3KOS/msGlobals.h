#ifndef _MSGLOBALS_H
#define _MSGLOBALS_H

class MagicShifterGlobals {
public:
  // state !J! TODO: clean all this up 
  int GLOBAL_GS = 5;
  float accelG[3];  // Stores the real accel value in g's
  int shifterMode = 0;
  int accelCount[3];  // Stores the 12-bit signed value
  int oldButton1State = 0;
  int currentMicros = 0, lastMicros = 0;
  int speedMicros = 1000;
  long lastFrameMicros = 0;
  int currentFrame = 0;
  byte bright = 0xFF;
  byte gs = 0x1;
  int loops = 0;
  long bootTime = 0;
  bool apMode = false;
  // make it larger to be on the safe side when base64 decoding
  byte web_rgb_buffer[RGB_BUFFER_SIZE + 4];
  int setActiveFile = 0; // is there a new file to activate?
  char uploadFileName[];
  File uploadFile;
};

// #else
// extern MagicShifterGlobals msGlobals;
#endif