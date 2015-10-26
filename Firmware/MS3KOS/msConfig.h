#ifndef MS_CONFIG_H
#define MS_CONFIG_H



// v1 == breadboard pcb
// 2 == 0.9 pcb :)
#define VERSION 0.1

// MIDI enabled?
#undef ENABLE_MIDI
// Accelerometer enabled?
#define ENABLE_ACCEL
//crashes on new esp aruino toolchain :\
//#define USE_MDNS
#define NUM_MS_MODES 3
// 192.168.4.1 is the IP it always has in softAP mode
#define FORCE_APMODE 1

#define CONNECTION_TIMEOUT 30000

#define POV_TIME_MICROSECONDS 1

#define MAX_LEDS 16
//#define MAX_LEDS 160


#define MIN_TIME_CLICK 10000
//#define MIN_TIME_LONG_CLICK 500000
//const int MIN_TIME_CLICK = 10000;
const int MIN_TIME_LONG_CLICK = 500000;

//#define MMA8452_ADDRESS 0x1D  // 0x1D if SA0 is high, 0x1C if low
#define MMA8452_ADDRESS 0x1C

#define MMA8452_ID 0x2A 

#define FILENAME_SIZE 40

#endif
