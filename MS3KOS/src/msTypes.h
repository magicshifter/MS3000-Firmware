#ifndef _MSTYPES_H
#define  _MSTYPES_H

#include "Modes/BaseMode.h"

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

union MSColor {
#ifdef COLOR_CODING_RGB
	struct {
		uint8_t r, g, b;
	} rgb;
#endif
#ifdef COLOR_CODING_GRB
	struct {
		uint8_t g, r, b;
	} rgb;
#endif
	struct {
		uint8_t ch0, ch1, ch2;
	} ordered;
	uint8_t channels[3];
};

#define MAGIC_BITMAP_PIXEL_OFFSET 16

// MagicShifter.cc
struct MSBitmapHeader {
	uint32_t fileSize;

	uint8_t pixelFormat;
	uint8_t maxFrame;
	uint8_t frameWidth;
	uint8_t frameHeight;

	uint8_t subType;
	uint8_t firstChar;
	uint16_t animationDelay;
} __attribute__ ((packed));

struct MSBitmap {
	MSBitmapHeader header;
	MSColor color;
	File bmFile;
	byte *bmBuffer;
};

#define MAX_AP_LEN 48

struct APAuth {
	char ssid[MAX_AP_LEN];
	char password[MAX_AP_LEN];
};

struct APAuthHelper {
	APAuth auth;

  public:
	 APAuthHelper() {
		clear();
	} 
	void clear(void) {
		memset(auth.ssid, 0, sizeof(auth.ssid));
		memset(auth.password, 0, sizeof(auth.password));
	}
};

struct ServerConfig {
	char hostname[MAX_AP_LEN];
	int port;

  public:
	 ServerConfig() {
		clear();
	} void clear(void) {
		memset(hostname, 0, sizeof(hostname));
		port = 80;
	}
};

struct APConfig {
	struct APAuthHelper apInfo;
	bool forceAPMode;
	void clear(void) {
		apInfo.clear();
		forceAPMode = false;
	}
};

struct UIConfig {
	uint currentMode;
	unsigned long timeoutHighPower;
	unsigned long timeoutLowPower;
	int defaultBrightness;
};


typedef struct {
	int x;
	int y;
} Coordinate_s;



#endif
