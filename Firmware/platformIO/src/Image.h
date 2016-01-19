#ifndef _IMAGE_H
#define _IMAGE_H

#define BYTESPERPIXEL 4

class MSImage {

  int width, height;

  MSBitmap _bitmap;
  //File file;

public:
  MSImage(void)
  {
    height = width = 0;
  }

  char sv_Filename[100];



  void dumpHeader(const MSBitmapHeader& header)
  {
    msSystem.logln("Header dump:");
    msSystem.log("fileSize:"); msSystem.logln(String(header.fileSize));
    msSystem.log("pixelFormat:"); msSystem.logln(String(header.pixelFormat));
    msSystem.log("maxFrame:"); msSystem.logln(String(header.maxFrame));
    msSystem.log("frameWidth:"); msSystem.logln(String(header.frameWidth));
    msSystem.log("frameHeight:"); msSystem.logln(String(header.frameHeight));
    msSystem.log("subType:"); msSystem.logln(String(header.subType));
    msSystem.log("firstChar:"); msSystem.logln(String(header.firstChar));
    msSystem.log("animationDelay:"); msSystem.logln(String(header.animationDelay));
  }

  static bool LoadBitmapFile(const char *filename, MSBitmap *bitmap)
  {
    int bmResult;

    bitmap->bmFile = SPIFFS.open(filename, "r");

    if (!bitmap->bmFile)
      return false;

    bmResult = bitmap->bmFile.read((uint8_t *)(&(bitmap->header)), sizeof(MSBitmapHeader));
    // bitmap->header.fileSize = MAGIC_BITMAP_PIXEL_OFFSET; // !J! ?

    bitmap->color.rgb.r = 255;
    bitmap->color.rgb.g = 255;
    bitmap->color.rgb.b = 255;

    return true;
  };



   void PlotBitmapColumn1Bit(const MSBitmap *bitmap, uint16_t absColumn, uint8_t ledIdx, byte *frameDest)
  {
    uint8_t bitBuffer[3];
    uint8_t bitBufferIdx = 0;

    uint16_t bitPos = absColumn * bitmap->header.frameHeight;
    uint32_t offset = MAGIC_BITMAP_PIXEL_OFFSET + (bitPos >> 3);
    // ReadBytes(offset, bitBuffer, 3); // this could be more efficient

// msSystem.log("1Bit:offset:"); msSystem.logln(String(offset));
// msSystem.log("1Bit:absColumn:"); msSystem.logln(String(absColumn));
// msSystem.log("1Bit:bitPos:"); msSystem.logln(String(bitPos));
// dumpHeader(bitmap->header);

    File lFile = bitmap->bmFile;
    lFile.seek(offset, SeekSet);
    // bitmap->bmFile.seek(offset, SeekSet);
    lFile.read(bitBuffer, 3);

// for (int x=0;x<3;x++) {
//  msSystem.log("xx:"); msSystem.logln(String(bitBuffer[x]));
// }

    uint8_t bitMask = 1 << (bitPos & 0x07);

     // this could be more efficient
    uint8_t r = bitmap->color.rgb.r;
    uint8_t g = bitmap->color.rgb.g;
    uint8_t b = bitmap->color.rgb.b;

  // msSystem.log("r:"); msSystem.logln(String(r));
  // msSystem.log("g:"); msSystem.logln(String(g));
  // msSystem.log("b:"); msSystem.logln(String(b));

    uint8_t endIndex = ledIdx + bitmap->header.frameHeight;
    if (endIndex > 16)
      endIndex = 16;

    do
    {
      uint8_t currentByte = bitBuffer[bitBufferIdx++];
      do
      {
        if (bitMask & currentByte)
        {
          msSystem.msLEDs.setPixels(ledIdx, r, g, b, 255); // this could be more efficient memcopy the structure
        }
        else
        {
          msSystem.msLEDs.setPixels(ledIdx, 0, 0, 0, 0); // this could be more efficient
        }
        bitMask <<= 1;
      } while (++ledIdx < endIndex && bitMask != 0);
      bitMask = 0x01;
    } while (ledIdx < endIndex); // this could be more efficient
  }


   void PlotBitmapColumn24Bit(const MSBitmap *bitmap, uint16_t absColumn, uint8_t startLed, byte *frameDest)
  {
    uint8_t nrOfBytes =  3 * bitmap->header.frameHeight;
    uint32_t offSet = MAGIC_BITMAP_PIXEL_OFFSET + absColumn * nrOfBytes;
    if (startLed + bitmap->header.frameHeight > 16)
      nrOfBytes = (16 - startLed)*3;
    // ReadBytes(offSet, msGlobals.ggRGBLEDBuf + 3*startLed, nrOfBytes); // never make startLed too big or it will corrupt mem

    File lFile = bitmap->bmFile;
    lFile.seek(offSet, SeekSet);
    lFile.read(frameDest, nrOfBytes);
    for(int x=MAX_LEDS - 1; x>=0; x--) 
    { 
      int id24 = x * 3;
      int id32 = x * 4;

      int r = frameDest[id24 + 0];
      int g = frameDest[id24 + 1];
      int b = frameDest[id24 + 2];

      frameDest[id32 + 3] = r;
      frameDest[id32 + 2] = g;
      frameDest[id32 + 1] = b;
      frameDest[id32 + 0] = 0xff;

    }
  }

   void PlotBitmapColumn(const MSBitmap *bitmap, uint8_t frame, uint8_t column, uint8_t startLed, byte *frameDest)
  {
    uint8_t frameIdx = frame - bitmap->header.firstChar;
    if (frameIdx <= bitmap->header.maxFrame)
    {
      uint16_t absColumn = ((uint16_t)frameIdx) * ((uint16_t)bitmap->header.frameWidth) + column;
      switch (bitmap->header.pixelFormat)
      {
        case 24:
          PlotBitmapColumn24Bit(bitmap, absColumn, startLed, frameDest);
          break;
        case 1:
          PlotBitmapColumn1Bit(bitmap, absColumn, startLed, frameDest);
          break;
      }
    }

  }

  void PlotText(const MSBitmap *bitmap, const char *text, uint16_t column, uint8_t startLed, byte *frameDest)
  {
    // if (!bitmap) bitmap = &font10x16;

    dumpHeader(bitmap->header);

    uint8_t ascii = text[column / bitmap->header.frameWidth];
    PlotBitmapColumn(bitmap, ascii, column % bitmap->header.frameWidth, startLed, frameDest);
  }



  void loadFile(const char *fileName)
  {
      msSystem.msEEPROMs.safeStrncpy(sv_Filename, fileName, MAX_FILENAME_LENGTH);


      LoadBitmapFile(sv_Filename, &_bitmap);

      height = _bitmap.header.frameHeight;
      width = _bitmap.header.frameWidth;

      // file = SPIFFS.open(fileName, "r");
      // if (file)
      // {
      //   // TODO: real file format
      //   height = MAX_LEDS;
      //   int size = file.size();
      //   int frameSize = height * BYTESPERPIXEL;

      //   if (size % frameSize != 0)
      //   {
      //     msSystem.log("File is no multiple of LED count: "); msSystem.logln(String(fileName));
      //   }
      //   width = size / frameSize;

      //   msSystem.log("loadFile: "); msSystem.logln(String(fileName));

      // }
      // else
      // {
      //   msSystem.log("File could not be opened: ");
      //   msSystem.logln(String(fileName));
      //   width = 0;
      //   height = MAX_LEDS;
      // }

      // msSystem.log("image width:"); msSystem.logln(String(width));;
      // msSystem.log("image height:"); msSystem.logln(String(height));;

      msSystem.log("loadShakeImage:"); msSystem.logln(String(sv_Filename));
  }

  MSImage(const char *fileName)
  {
    loadFile(fileName);
  }

  ~MSImage()
  {
    close();
  }

  int getWidth() { return width; }
  int getHeight() { return height; }

  bool readFrame(int frameIdx, byte *frameData, int maxHeight)
  {

    PlotBitmapColumn(&_bitmap, 0, frameIdx, 0, frameData);
//     if (file)
//     {

//       // !J! 16-byte OFFSET due to bug in bitmap generator code
// #define MAGIC_OFFSET 0
//       file.seek( (frameIdx * height * BYTESPERPIXEL) + MAGIC_OFFSET, SeekSet);

//       if (height < maxHeight) maxHeight = height;

//         int result = file.read(frameData, maxHeight * BYTESPERPIXEL);

// #if 0
// msSystem.log("framedata/"); msSystem.log(String(maxHeight * BYTESPERPIXEL)); msSystem.log("/");
// for (int x=0;x<maxHeight * BYTESPERPIXEL;x++) {
//   if (x % 4 == 0) msSystem.logln("");
//   msSystem.log(":"); Serial.print(frameData[x], HEX);
// }
// msSystem.logln("<<EOF");
// #endif

//         if (result < maxHeight * BYTESPERPIXEL)
//         {
//           return false;
//         }
      
//         return true;
//       }

//     return false;
  }

  void close()
  {

    _bitmap.bmFile.close();
  }
};

#endif

