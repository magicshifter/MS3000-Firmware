// Bitmap.h

#ifndef _MAGIC_SHAKETEXT_H
#define _MAGIC_SHAKETEXT_H


class MagicShakeText  : public MagicShifterBaseMode
{

private:

	uint8_t AccelShakeNrOfFrames = 0;
	uint8_t AccelShakeFrameNr = 0xFF;

	// MSBitmap font16px;
	// MSBitmap font12px;
	// MSBitmap font8px;
	// MSBitmap font6px;

	MSBitmap font10x16;
// font4x5.magicFont
// font6x8.magicFont
// font7x12.magicFont

public:

 	MagicShakeText()
	{
		// LoadBitmap(128, &font16px);
		// LoadBitmap(129, &font12px);
		// LoadBitmap(130, &font8px);
		// LoadBitmap(131, &font6px);
	}
#if 0

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

	void init()
	{
		// msSystem.log("Load font 16:"); 
		msSystem.logln(String(LoadBitmapFile("font10x16.magicFont", &font10x16)));
		
		// dumpHeader(font10x16.header);

	};


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

	// void LoadBitmap(uint8_t sector, MSBitmap *bitmap)
	// {
	// 	uint32_t eepromHeaderAddr = (uint32_t)sector*0x1000l;
	// 	ReadBytes(eepromHeaderAddr, (uint8_t *)(&(bitmap->header)), sizeof(MSBitmapHeader));

	// 	bitmap->header.fileSize = eepromHeaderAddr + 16;
	// 	color.rgb.r = 255;
	// 	color.rgb.g = 255;
	// 	color.rgb.b = 255;
	// }

	static void PlotBitmapColumn1Bit(const MSBitmap *bitmap, uint16_t absColumn, uint8_t ledIdx, byte *frameDest)
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
// 	msSystem.log("xx:"); msSystem.logln(String(bitBuffer[x]));
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


	static void PlotBitmapColumn24Bit(const MSBitmap *bitmap, uint16_t absColumn, uint8_t startLed, byte *frameDest)
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

			frameDest[id32 + 3] = frameDest[id24 + 2];
			frameDest[id32 + 2] = frameDest[id24 + 1];
			frameDest[id32 + 1] = frameDest[id24 + 0];
			frameDest[id32 + 0] = 0xff;


		}
	}

	static void PlotBitmapColumn(const MSBitmap *bitmap, uint8_t frame, uint8_t column, uint8_t startLed, byte *frameDest)
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

	void PlotText(const MSBitmap *bitmap, const char *text, uint16_t column, uint8_t startLed)
	{
		// if (!bitmap) bitmap = &font10x16;

		dumpHeader(bitmap->header);

		uint8_t ascii = text[column / bitmap->header.frameWidth];
		PlotBitmapColumn(bitmap, ascii, column % bitmap->header.frameWidth, startLed);
	}
#endif


	void Int2Str(int val, char *text) {
		char buffer[8];
		uint8_t len=0;

		//for (len = 0; text[len] != 0; len++);

		uint8_t i = 0;

		do {
			buffer[len] = '0' + (val % 10);
			val /= 10;
			len++;
		} while(val);

		for (i = 0; i < len; i++) {
			*text = buffer[(len-1)-i];
			text++;
		}
		*text = 0;

	}

};

#endif

