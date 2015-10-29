//
// msConfig.h - basic, build oriented configuration options
//

#ifndef MS_CONFIG_H
#define MS_CONFIG_H

#undef DEBUG_OUTPUT

// v1 == breadboard pcb
// 2 == 0.9 pcb :)
#define VERSION 0.1

// MIDI enabled?
#define CONFIG_ENABLE_MIDI
// Accelerometer enabled?
#define CONFIG_ENABLE_ACCEL
//crashes on new esp aruino toolchain :\
//#define USE_MDNS
#define NUM_MS_MODES 3
// 192.168.4.1 is the IP it always has in softAP mode
#define FORCE_APMODE 1

#define CONNECTION_TIMEOUT 30000

#define POV_TIME_MICROSECONDS 1

#define MAX_LEDS 16
//#define MAX_LEDS 160
#define RGB_BUFFER_SIZE (4*MAX_LEDS)

#define MIN_TIME_CLICK 10000
#define MIN_TIME_LONG_CLICK 500000

#define MAX_FILENAME_LENGTH 32

// which MMA is in use
// #define CONFIG_MMA_NORMAL
#define CONFIG_MMA_FSOX

// Use pre-configured AP list
#undef CONFIG_USE_DEBUG_AP

#endif
