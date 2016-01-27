#ifndef _IMAGE_H
#define _IMAGE_H

#define BYTESPERPIXEL 4

class MagicShifterImage {
  //File file;
public:
  int width, height;
  MSBitmap _bitmap;

  MagicShifterImage()
  {
    height = width = 0;
  }

  char sv_Filename[100];

  void LoadFile(const char *fileName)
  {
      LoadBitmapFile(fileName, &_bitmap);
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
      //     mSystem.log("File is no multiple of LED count: "); mSystem.logln(String(fileName));
      //   }
      //   width = size / frameSize;

      //   mSystem.log("loadFile: "); mSystem.logln(String(fileName));

      // }
      // else
      // {
      //   mSystem.log("File could not be opened: ");
      //   mSystem.logln(String(fileName));
      //   width = 0;
      //   height = MAX_LEDS;
      // }

      // mSystem.log("image width:"); mSystem.logln(String(width));;
      // mSystem.log("image height:"); mSystem.logln(String(height));;

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

  MagicShifterImage(const char *fileName)
  {
    LoadFile(fileName);
  }

  ~MagicShifterImage()
  {
    close();
  }

  int getWidth() { return width; }
  int getHeight() { return height; }

  void close()
  {

    _bitmap.bmFile.close();
  }
};

#endif

