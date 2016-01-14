// Bitmap.h

#ifndef _MAGIC_SHAKETEXT_H
#define _MAGIC_SHAKETEXT_H


#define COLOR_CODING_RGB

#ifdef COLOR_CODING_RGB
	#define CHANNEL_RED 		0
	#define CHANNEL_GREEN 		1
	#define CHANNEL_BLUE 		2
#endif
#ifdef COLOR_CODING_GRB
	#define CHANNEL_RED 		1
	#define CHANNEL_GREEN 		0
	#define CHANNEL_BLUE 		2
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


class MagicShakeTextMode  : public MagicShifterBaseMode
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

 	MagicShakeTextMode()
	{
		// LoadBitmap(128, &font16px);
		// LoadBitmap(129, &font12px);
		// LoadBitmap(130, &font8px);
		// LoadBitmap(131, &font6px);
	}


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
		msSystem.log("Load font 16:"); 
		msSystem.logln(String(LoadBitmapFile("font10x16.magicFont", &font10x16)));
		
		dumpHeader(font10x16.header);

	};


	bool LoadBitmapFile(const char *filename, MSBitmap *bitmap)
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

	void PlotBitmapColumn1Bit(const MSBitmap *bitmap, uint16_t absColumn, uint8_t ledIdx)
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


	void PlotBitmapColumn24Bit(const MSBitmap *bitmap, uint16_t absColumn, uint8_t startLed)
	{
		// uint8_t nrOfBytes =  3 * bitmap->header.frameHeight;
		// uint32_t addr = bitmap->header.fileSize + absColumn * nrOfBytes;
		// if (startLed + bitmap->header.frameHeight > 16)
		// 	nrOfBytes = (16 - startLed)*3;
		
		// ReadBytes(addr, msGlobals.ggRGBLEDBuf + 3*startLed, nrOfBytes); // never make startLed too big or it will corrupt mem

	}

	void PlotBitmapColumn(const MSBitmap *bitmap, uint8_t frame, uint8_t column, uint8_t startLed)
	{
		uint8_t frameIdx = frame - bitmap->header.firstChar;
		if (frameIdx <= bitmap->header.maxFrame)
		{
			uint16_t absColumn = ((uint16_t)frameIdx) * ((uint16_t)bitmap->header.frameWidth) + column;
			switch (bitmap->header.pixelFormat)
			{
				case 24:
					PlotBitmapColumn24Bit(bitmap, absColumn, startLed);
					break;
				case 1:
					PlotBitmapColumn1Bit(bitmap, absColumn, startLed);
					break;
			}
		}

	}

	void PlotText(const MSBitmap *bitmap, const char *text, uint16_t column, uint8_t startLed)
	{
		if (!bitmap) bitmap = &font10x16;

		// dumpHeader(bitmap->header);

		uint8_t ascii = text[column / bitmap->header.frameWidth];
		PlotBitmapColumn(bitmap, ascii, column % bitmap->header.frameWidth, startLed);
	}


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



	void displayText(const char *text,bool wait,MSBitmap *font)
	{
		int n;
		int frame = 0;

		//MSBitmap *font = &font16px;
		if (!font) font = &font10x16;



		// if (!wait && ( centerBtnClickedTime || powerBtnClickedTime || centerBtnPressed || powerBtnPressed))
	 //  			return;
// msSystem.msBtnPwrLongHit == true


		#define RGB_BUFFERSIZE (3*16)
		uint8_t backupLedValues[RGB_BUFFERSIZE];

		for (int i = 0; i < RGB_BUFFERSIZE; i++) {
			backupLedValues[i] = msGlobals.ggRGBLEDBuf[i];
		}

		while (text[n] != 0) n++;

		font->color.rgb.r = 128;
		font->color.rgb.g = 128;
		font->color.rgb.b = 255;


		AccelShakeNrOfFrames = n * COLUMNMULTIPLY*font->header.frameWidth;

		// if(wait)
		// 	WaitClearButtons();

		// while (1)
		{
			frame++;

			// if (!MagicShifter_Poll() || centerBtnPressed || powerBtnPressed)
	  // 			break;

			// AccelPoll();
			// UpdateAccelShakeCurrentColumn();

			if (AccelShakeFrameNr != 0xFF)
			{
				if (AccelShakeFrameNr < 0xFE)
				{
					uint8_t realFrame = AccelShakeFrameNr / COLUMNMULTIPLY;

					PlotText(font, text, realFrame, 0);
			        // msSystem.msLEDs.updatePixels();

				}
				else
				{
					msSystem.msLEDs.fillPixels(0,0,0);
			        // msSystem.msLEDs.updatePixels();

				}
			}
			else {
				for (int i = 0; i < RGB_BUFFERSIZE; i++) {
					msGlobals.ggRGBLEDBuf[i] = backupLedValues[i];
				}
		        // msSystem.msLEDs.updatePixels();

			}
		}

		for (int i = 0; i < RGB_BUFFERSIZE; i++) {
			msGlobals.ggRGBLEDBuf[i] = backupLedValues[i];
		}

	    msSystem.msLEDs.updatePixels();

		// RestoreAccelState();

		// if(wait)
		// 	WaitClearButtons();
	}

};

#endif

