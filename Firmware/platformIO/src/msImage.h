#ifndef _IMAGE_H
#define _IMAGE_H

#define BYTESPERPIXEL 4

#define MAX_SHAKE_TEXT 4

typedef struct  {
  int x;
  int y;
} Coordinate_s;

class MagicShifterImageAbstr {
public:
  virtual int getWidth();
  virtual void getFrameData(int frameIdx, byte *frameDest);
  virtual void close();
};

void PlotBitmapColumn1Bit(const MSBitmap *bitmap, uint16_t absColumn, uint8_t ledIdx, byte *frameDest)
{
    uint8_t bitBuffer[3];
    uint8_t bitBufferIdx = 0;

    uint16_t bitPos = absColumn * bitmap->header.frameHeight;
    uint32_t offset = MAGIC_BITMAP_PIXEL_OFFSET + (bitPos >> 3);
    // ReadBytes(offset, bitBuffer, 3); // this could be more efficient

    uint8_t ledOffs = ledIdx;

// log("1Bit:offset:"); logln(String(offset));
// log("1Bit:absColumn:"); logln(String(absColumn));
// log("1Bit:bitPos:"); logln(String(bitPos));
// dumpActiveHeader(bitmap->header);

    if (bitmap->bmBuffer == NULL) {
      File lFile = bitmap->bmFile;
      lFile.seek(offset, SeekSet);
      // bitmap->bmFile.seek(offset, SeekSet);
      lFile.read(bitBuffer, 3);
    }
    else {
      for (int i = 0; i < 3; i++) {
        bitBuffer[i] = bitmap->bmBuffer[offset + i];
      }
    }

// for (int x=0;x<3;x++) {
//  log("xx:"); logln(String(bitBuffer[x]));
// }

    uint8_t bitMask = 1 << (bitPos & 0x07);

     // this could be more efficient
    uint8_t r = bitmap->color.rgb.r;
    uint8_t g = bitmap->color.rgb.g;
    uint8_t b = bitmap->color.rgb.b;

  // log("r:"); logln(String(r));
  // log("g:"); logln(String(g));
  // log("b:"); logln(String(b));

    uint8_t endIndex = ledIdx + bitmap->header.frameHeight;
    if (endIndex > 16)
      endIndex = 16;

    do
    {
      uint8_t currentByte = bitBuffer[bitBufferIdx++];
      do
      {
      
        int id32 = ledIdx * 4;


        if (bitMask & currentByte)
        {
          // msLEDs.setLED(ledIdx, r, g, b, 255); // this could be more efficient memcopy the structure
          frameDest[id32 + 3] = r;
          frameDest[id32 + 2] = g;
          frameDest[id32 + 1] = b;
          frameDest[id32 + 0] = 0xff;
        }
        else
        {
          // msLEDs.setLED(ledIdx, 0, 0, 0, 0); // this could be more efficient
          frameDest[id32 + 3] = 0;
          frameDest[id32 + 2] = 0;
          frameDest[id32 + 1] = 0;
          frameDest[id32 + 0] = 0xff;
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




  if (bitmap->bmBuffer == NULL) {
    File lFile = bitmap->bmFile;
    lFile.seek(offSet, SeekSet);
    lFile.read(frameDest, nrOfBytes);
  }
  else {
    for (int i = 0; i < nrOfBytes; i++) {
      frameDest[i] = bitmap->bmBuffer[offSet + i];
    }
  }



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
  uint8_t ascii = text[column / bitmap->header.frameWidth];
  PlotBitmapColumn(bitmap, ascii, column % bitmap->header.frameWidth, startLed, frameDest);
}


// textPlotting .. 
class MagicShifterImageText : public MagicShifterImageAbstr {

private:

  int txtCount;
  int txtWidth;
  char txtCollection[MAX_SHAKE_TEXT][MAX_TEXT_LENGTH];
  // !w! todo: should be ref/ptr 
  MSBitmap txtFonts[MAX_SHAKE_TEXT];
  int txtSizes[MAX_SHAKE_TEXT];

  Coordinate_s txtPositions[MAX_SHAKE_TEXT];

public:
  
  void close () {} 

  void resetTexts()
  {
    txtCount = 0;
    txtWidth = 0;
  }


  void plotTextString( char *text, MSBitmap &font,  Coordinate_s pos)
  {
    if (txtCount < MAX_SHAKE_TEXT) {
      int lineStart = 0;
      for (int i = 0; ; i++) {
        if (text[i] == '\n' || text[i] == 0) {
          if (i != lineStart) {
            int len =  i-lineStart+1;
            if (len > MAX_TEXT_LENGTH)
              len = MAX_TEXT_LENGTH;

            l_safeStrncpy(txtCollection[txtCount], text + lineStart, len);

            txtFonts[txtCount] = font;
            txtPositions[txtCount] = pos;

            int ts = font.header.frameWidth * strlen(text);
            ts += pos.x;

            txtSizes[txtCount] = ts;

            if (ts > txtWidth) {
              txtWidth = ts;
            }

            pos.y += font.header.frameHeight;

            txtCount++;

            if (txtCount >= MAX_SHAKE_TEXT) 
              break;
          }
        }
        if (text[i] == 0) {
          break;
        }
      }
    } 
  };

  int getWidth()
  {
    return txtWidth;
  };

  int getHeight()
  {
    // return txtHeight;
  };

  void getFrameData(int frameIdx, byte *frameDest)
  {
    for (int i=0; i<MAX_LEDS * 4; i+=4) 
    {
      frameDest[i] = 0xff;
      frameDest[i+1] = 0x00;
      frameDest[i+2] = 0x00;
      frameDest[i+3] = 0x00;
    }
    
    for (int i = 0; i < txtCount; i++) {
      int idx = frameIdx - txtPositions[i].x;

      if (idx >= 0  && idx < txtSizes[i]) {
        PlotText(&txtFonts[i], txtCollection[i], idx, txtPositions[i].y, frameDest);
      }
    }
  };
};


class MagicShifterImage : public MagicShifterImageAbstr {

//File file;
public:

  MSBitmap _bitmap;
  char sv_Filename[100];
  int width, height;

  MagicShifterImage()
  {
    height = width = 0;
  }

  MagicShifterImage(const char *fileName)
  {
    LoadFile(fileName);
  }

  ~MagicShifterImage()
  {
    close();
  }


  int getHeight() { return height; }
  int getWidth() { return width; }
  void getFrameData(int frameIdx, byte *frameDest)
  {
    PlotBitmapColumn(&_bitmap, 0, frameIdx, 0, frameDest);
  };


  void LoadFile(const char *fileName)
  {

    Serial.print("filename:"); Serial.println(fileName);
    
    l_safeStrncpy(sv_Filename, fileName, MAX_FILENAME_LENGTH);

    if (LoadBitmapFile(fileName, &_bitmap) == true)
    {
      // Serial.println("loadbitmapfile: 0000");
      height = _bitmap.header.frameHeight;
      width = _bitmap.header.frameWidth;
    }
    else {
      // Serial.println("loadbitmapfile: not okay");
      height = 0;
      width = 0;
    }
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

  static bool LoadBitmapBuffer(const char *filename, MSBitmap *bitmap)
  {
    int bmResult;
    int bmSize;

    if (LoadBitmapFile(filename, bitmap)) {
      bmSize = bitmap->bmFile.size();

      bitmap->bmBuffer = (byte *)malloc(bmSize);

      if (bitmap->bmBuffer) {
        bitmap->bmFile.seek(0, SeekSet);;
        bmResult = bitmap->bmFile.read(bitmap->bmBuffer, bmSize);
      }

      bitmap->bmFile.close();

      return true;
    }

    return false;
  };

  void close()
  {
    // Serial.print("closefile:"); Serial.println(String(_bitmap.bmFile));
    if (_bitmap.bmFile)
      _bitmap.bmFile.close();
    if(_bitmap.bmBuffer != NULL) {
      free(_bitmap.bmBuffer);
      _bitmap.bmBuffer = NULL;
    }
  }
};

#endif

