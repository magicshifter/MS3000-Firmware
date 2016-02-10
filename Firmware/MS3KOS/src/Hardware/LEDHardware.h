//#include "os_type.h"

// LED Buffer functional interface to APA102 LED controller

#ifndef LEDHARDWARE_H
#define LEDHARDWARE_H

#define USE_HW_SPI
#ifdef USE_HW_SPI
#include <SPI.h>
#endif

#if (LED_TYPE == LED_TYPE_APA102)
#define SPI_FREQUENCY 20000000
#else
#define SPI_FREQUENCY 500000
#endif

#define PIN_LED_ENABLE 15

// PIN_LED_DATA an PIN_LED_CLOCK can not be changed since we use the hardware SPI
#define PIN_LED_DATA  13
#define PIN_LED_CLOCK 14

// taken from: 
//http://electronics.stackexchange.com/questions/1983/correcting-for-non-linear-brightness-in-leds-when-using-pwm/11100
byte lookupBrightness[16] = {
	B00000000, B00000010, B00000100, B00000111, B00001011, B00010010,
		B00011110, B00101000,
	B00110010, B01000001, B01010000, B01100100, B01111101, B10100000,
		B11001000, B11111111
};

// TODO : Flesh this out?
class MagicShifterLEDs {

  private:

	byte ledBuffer[RGB_BUFFER_SIZE + 8];
	byte clearBuffer[RGB_BUFFER_SIZE + 8];

	byte *RGB_COLORS = ledBuffer + 4;


  public:
	void saveBuffer(byte * buffer) {
		for (int i = 0; i < RGB_BUFFER_SIZE; i++) {
			buffer[i] = RGB_COLORS[i];
	}} void loadBuffer(byte * buffer) {
		for (int i = 0; i < RGB_BUFFER_SIZE; i++) {
			RGB_COLORS[i] = buffer[i];

			// i+=4..
			// RGB_COLORS[i] = buffer[i+3];
			// RGB_COLORS[i+1] = buffer[i+0];
			// RGB_COLORS[i+2] = buffer[i+1];
			// RGB_COLORS[i+3] = buffer[i+2];

		}
	}

	void loadBufferShort(byte * buffer) {
		for (int i = 0; i < RGB_BUFFER_SIZE; i++) {
			if (i & 4 == 0)
				RGB_COLORS[i] = buffer[i];
			else {
				RGB_COLORS[i] = lookupBrightness[(buffer[i]) & 0xF] >> 3;
			}

			// i+=4..
			// RGB_COLORS[i] = buffer[i+3];
			// RGB_COLORS[i+1] = buffer[i+0];
			// RGB_COLORS[i+2] = buffer[i+1];
			// RGB_COLORS[i+3] = buffer[i+2];

		}
	}

	void loadBufferLong(byte * buffer) {
		for (int i = 0; i < RGB_BUFFER_SIZE; i++) {
			if (i & 4 == 0)
				RGB_COLORS[i] = buffer[i];
			else {
				RGB_COLORS[i] = lookupBrightness[buffer[i] >> 4] >> 3;
			}

			// i+=4..
			// RGB_COLORS[i] = buffer[i+3];
			// RGB_COLORS[i+1] = buffer[i+0];
			// RGB_COLORS[i+2] = buffer[i+1];
			// RGB_COLORS[i+3] = buffer[i+2];

		}
	}

	void disableLEDHardware() {
		pinMode(PIN_LED_ENABLE, INPUT);
	}


	void initLEDHardware() {
		pinMode(PIN_LED_ENABLE, OUTPUT);
		digitalWrite(PIN_LED_ENABLE, HIGH);

#ifdef USE_HW_SPI
		SPI.begin();
		SPI.setFrequency(SPI_FREQUENCY);
#else
		pinMode(PIN_LED_DATA, OUTPUT);
		pinMode(PIN_LED_CLOCK, OUTPUT);
#endif
	}

	void bootSwipe() {

		// swipe colors
		for (byte idx = 0; idx < MAX_LEDS; idx++) {
			setLED(idx, (idx & 1) ? 255 : 0, (idx & 2) ? 255 : 0, (idx & 4) ? 255 : 0, 2);	//msGlobals.ggBrightness);
			updateLEDs();
			delay(30);
		}
		for (byte idx = 0; idx < MAX_LEDS; idx++) {
			setLED(idx, 0, 0, 0, 1);
			updateLEDs();
			delay(30);
		}
	}

	void errorSwipe() {

		// swipe colors
		for (byte idx = 0; idx < MAX_LEDS; idx++) {
			setLED(idx, (idx & 1) ? 255 : 0, 0, 0, msGlobals.ggBrightness);
			updateLEDs();
			delay(30);
		}
		for (byte idx = 0; idx < MAX_LEDS; idx++) {
			setLED(idx, 0, 0, 0, 1);
			updateLEDs();
			delay(30);
		}
	}


	// void fillLEDs(byte r, byte g, byte b, byte gs);

	void initLEDBuffer() {
		ledBuffer[0] = 0;
		ledBuffer[1] = 0;
		ledBuffer[2] = 0;
		ledBuffer[3] = 0;


		ledBuffer[RGB_BUFFER_SIZE + 0] = 0;
		ledBuffer[RGB_BUFFER_SIZE + 1] = 0;
		ledBuffer[RGB_BUFFER_SIZE + 2] = 0;
		ledBuffer[RGB_BUFFER_SIZE + 4] = 0;



		for (int i = 0; i < RGB_BUFFER_SIZE + 8; i += 4) {
			clearBuffer[i] = (i < 4
							  || i >= RGB_BUFFER_SIZE + 4) ? 0 : 0xFF;
			clearBuffer[i + 1] = 0;
			clearBuffer[i + 2] = 0;
			clearBuffer[i + 3] = 0;
		}

		fillLEDs(0, 0, 0, 0);
	}

	void setLEDChannel(int index, int channel, int value) {
#if (LED_TYPE == LED_TYPE_APA102)
		int idx = index << 2;
		if (channel == 3)
			RGB_COLORS[idx] = 0xE0 | channel;
		else
			RGB_COLORS[idx + 1 + channel] = value;
#else
		int idx = index * 3;
		RGB_COLORS[idx + channel] = value;
#endif
	}


	void setLED(int index, byte r, byte g, byte b, byte gs = 0x1F) {
#if (LED_TYPE == LED_TYPE_APA102)
		int idx = index << 2;
		RGB_COLORS[idx] = 0xE0 | gs;
		RGB_COLORS[idx + 1] = b;
		RGB_COLORS[idx + 2] = g;
		RGB_COLORS[idx + 3] = r;
#else
		int idx = index * 3;
		RGB_COLORS[idx + 0] = b;
		RGB_COLORS[idx + 1] = g;
		RGB_COLORS[idx + 2] = r;
#endif
	}

	void fillLEDs(byte r, byte g, byte b, byte gs = 0x1F) {
		for (int idx = 0; idx < MAX_LEDS; idx++) {
			setLED(idx, r, g, b, gs);
		}
	}

	void updateLEDs() {
#if (LED_TYPE == LED_TYPE_APA102)
		SPI.writeBytes(ledBuffer, RGB_BUFFER_SIZE + 8);
#else
		SPI.writeBytes(RGB_COLORS, RGB_BUFFER_SIZE);
#endif
	}

	void fastClear() {
		SPI.writeBytes(clearBuffer, RGB_BUFFER_SIZE + 8);
	}


	void setAllChannel(uint8_t channel, uint8_t value)
	{
		for (int i = 0; i < MAX_LEDS; i++) {
			setChannel(i, channel, value);
		}
	}

	void setChannel(uint8_t index, uint8_t channelIndex, uint8_t value)
	{
	  if (index >= 0 && index < MAX_LEDS && channelIndex >= 0 && channelIndex < 3)
	  {
#if (LED_TYPE == LED_TYPE_APA102)
			int idx = index << 2;
			RGB_COLORS[idx + 1 + channelIndex] = value;
#else
			int idx = index * 3;
			RGB_COLORS[idx + channelIndex] = value;
#endif
	  }
	}

};


#endif
// LEDHARDWARE_H
