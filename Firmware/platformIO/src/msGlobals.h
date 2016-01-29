#ifndef _MSGLOBALS_H
#define _MSGLOBALS_H

#define DEFAULT_FACTORY_MODE 0
//
// Global definitions and the master global struct for the system
//

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

void l_safeStrncpy(char * dest, const char *source, int n)
{
  strncpy(dest, source, n);
  dest[n-1] = '\0';
}

#define FAULT_NO_ACCELEROMETER 0xf1
#define FAULT_NEW_FILEUPLOAD    0xf2

class MagicShifterGlobals {
public:
  // state !J! TODO: clean all this up 
  long ggFault = 0;  // TODO: reset-reason/test-fault/etc.
  int ggFactoryIntensity = 10;
  float ggAccel[3];  // Stores the real accel value in g's
  long ggAccelTime;
  long ggLFrameTime;
  long ggCurrentMode = DEFAULT_FACTORY_MODE;  // POV mode
  int ggAccelCounts[3];  // Stores the 12-bit signed value
  int ggBtn1State = 0;
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
  MSBitmap tBitmap4x5;
  MSBitmap tBitmap6x8;
  MSBitmap tBitmap10x16;
  MSBitmap tBitmap7x12;

};



// #else
// extern MagicShifterGlobals msGlobals;
#endif
