#ifndef _MSGLOBALS_H
#define _MSGLOBALS_H

class MagicShifterGlobals {
public:
  // state !J! TODO: clean all this up 
  int ggFactoryIntensity = 10;
  float ggAccel[3];  // Stores the real accel value in g's
  long ggAccelTime;
  long ggLFrameTime;
  long ggCurrentMode = 0;  // POV mode
  long ggAccelCounts[3];  // Stores the 12-bit signed value
  long ggBtn1State = 0;
  long ggCurrentMicros = 0;
  long ggLastMicros = 0;
  long ggSpeedMicros = 800;
  long ggLastFrameMicros = 0;
  long ggCurrentFrame = 0;
  byte ggBright = 0xFF;
  byte gs = 0x1;
  long ggBootTime = 0;
  bool ggModeAP = false;
  // make it larger to be on the safe side when base64 decoding
  byte ggRGBLEDBuf[RGB_BUFFER_SIZE + 4];
  int ggShouldAutoLoad = 0; // is there a new file to activate?
  char ggUploadFileName[MAX_FILENAME_LENGTH + 1];
  File ggUploadFile;
  long ggTime = 76500000;
  long ggTimePostedAt = 0;
};




#define COLOR_CODING_RGB

#ifdef COLOR_CODING_RGB
  #define CHANNEL_RED     0
  #define CHANNEL_GREEN     1
  #define CHANNEL_BLUE    2
#endif
#ifdef COLOR_CODING_GRB
  #define CHANNEL_RED     1
  #define CHANNEL_GREEN     0
  #define CHANNEL_BLUE    2
#endif

#define COLUMNMULTIPLY 2


union MSColor {
#ifdef COLOR_CODING_RGB
  struct { uint8_t r, g, b; } rgb;
#endif
#ifdef COLOR_CODING_GRB
  struct { uint8_t g, r, b; } rgb;
#endif
  struct { uint8_t ch0, ch1, ch2; } ordered;
  uint8_t channels[3];
/*
  public MSColor(uint8_t _r, uint8_t _g, uint8_t _b)
  {
    rgb.r = _r;
    rgb.g = _g;
    rgb.b = _b;
  }
*/
 };

 #define MAGIC_BITMAP_PIXEL_OFFSET 16

// MagicShifter.cc
struct MSBitmapHeader
{
  uint32_t fileSize;

  uint8_t pixelFormat;
  uint8_t maxFrame;
  uint8_t frameWidth;
  uint8_t frameHeight;

  uint8_t subType;
  uint8_t firstChar;
  uint16_t animationDelay;
} __attribute__((packed));

struct MSBitmap
{
  MSBitmapHeader header;
  MSColor color;
  File bmFile;
};



// #else
// extern MagicShifterGlobals msGlobals;
#endif
