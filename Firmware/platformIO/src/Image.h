#ifndef _IMAGE_H
#define _IMAGE_H

#define BYTESPERPIXEL 4

class MSImage {

  int width, height;
  File file;

public:
  MSImage(void)
  {
    height = width = 0;
  }

  char sv_Filename[100];

  void loadFile(char *fileName)
  {
      msSystem.msEEPROMs.safeStrncpy(sv_Filename, fileName, 100);

      file = SPIFFS.open(fileName, "r");
      if (file)
      {
        // TODO: real file format
        height = MAX_LEDS;
        int size = file.size();
        int frameSize = height * BYTESPERPIXEL;

        if (size % frameSize != 0)
        {
          msSystem.log("File is no multiple of LED count: "); msSystem.logln(String(fileName));
        }
        width = size / frameSize;

        msSystem.log("File could be opened: "); msSystem.logln(String(fileName));

      }
      else
      {
        msSystem.log("File could not be opened: ");
        msSystem.logln(String(fileName));
        width = 0;
        height = MAX_LEDS;
      }

      msSystem.log("image width:"); msSystem.logln(String(width));;
      msSystem.log("image height:"); msSystem.logln(String(height));;

  }

  MSImage(char *fileName)
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
    if (file)
    {

      // !J! 16-byte OFFSET due to bug in bitmap generator code
#define MAGIC_OFFSET 16 
      file.seek( (frameIdx * height * BYTESPERPIXEL) + MAGIC_OFFSET, SeekSet);

      if (height < maxHeight) maxHeight = height;

        int result = file.read(frameData, maxHeight * BYTESPERPIXEL);

#if 0
msSystem.log("framedata/"); msSystem.log(String(maxHeight * BYTESPERPIXEL)); 
for (int x=0;x<maxHeight * BYTESPERPIXEL;x++) {
  msSystem.log(":"); Serial.print(frameData[x], HEX);;
}
msSystem.logln("<<EOF");
#endif

        if (result < maxHeight * BYTESPERPIXEL)
        {
          return false;
        }
      
        return true;
      }

    return false;
  }


  void close()
  {
    if (file)
      file.close();
  }
};

#endif

