#define BYTESPERPIXEL 4

class Image {

  int width, height;
  FSFile file;

public:
  Image(char *fileName)
  {
      file = FS.open(fileName, FSFILE_READ);
      if (file)
      {
        // TODO: real file format
        height = LEDS;
        int size = file.size();
        int frameSize = height * BYTESPERPIXEL;

        if (size % frameSize != 0)
        {
          Serial.print("File is no multiple of LED count: ");
          Serial.println(fileName);
        }
        width = size / frameSize;
      }
      else
      {
        Serial.print("File could not be opened: ");
        Serial.println(fileName);
        width = 0;
        height = LEDS;
      }
  }

  int getWidth() { return width; }
  int getHeight() { return height; }

  bool readFrame(int frameIdx, byte *frameData, int maxHeight)
  {
    if (file)
    {
      file.seek(frameIdx * height * BYTESPERPIXEL);

      if (height < maxHeight) maxHeight = height;
      int result = povFile.read(frameData, maxHeight * BYTESPERPIXEL);
      
      if (result < size)
      {
        return false;
      }
      return true;
    }
    return false;
  }
}
