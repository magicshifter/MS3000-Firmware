//
// msConfig.h - basic, build oriented configuration options
//

#ifndef MS_CONFIG_H
#define MS_CONFIG_H

#undef DEBUG_OUTPUT

// set of Hw configurations
#define HW_ID_MS3000_FSOX8700CQ 1	// magnetometer+accel
#define HW_ID_MS3000_MMA8452Q 	2	// accel
#define HW_ID_RING   		3		// on the wall

// TODO: pass this default from the Makefile
//#define HW_ID HW_ID_MS3000_FSOX8700CQ
//#define HW_ID HW_ID_MS3000_MMA8452Q

// set of LED types
#define LED_TYPE_APA102 1 
#define LED_TYPE_WS2801 2
#define LED_TYPE LED_TYPE_APA102

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


//#define MAX_LEDS 160
#define RGB_BUFFER_SIZE (4*MAX_LEDS)

#define MIN_TIME_CLICK 10000
#define MIN_TIME_LONG_CLICK 2000000

#define MAX_FILENAME_LENGTH 32

// which MMA is in use

// Use pre-configured AP list
#undef CONFIG_USE_DEBUG_AP

#if(HW_ID==HW_ID_MS3000_FSOX8700CQ)
#warning "HW ID :  HW_ID_MS3000_FSOX8700CQ "
#define MAX_LEDS 16
#define CONFIG_MMA_FSOX
#elif(HW_ID==HW_ID_MS3000_MMA8452Q)
#warning "HW ID :  HW_ID_MS3000_MMA8452Q "
#define MAX_LEDS 16
#define CONFIG_MMA_NORMAL
#elif(HW_ID==HW_ID_RING)
#warning "HW ID :  HW_ID_RING "
#define MAX_LEDS 160
#define CONFIG_MMA_NONE
#define LED_TYPE LED_TYPE_WS2801
#else
#error "No HW_ID configured! abort!"
#endif


#if(HW_ID==HW_ID_RING)
#endif




#endif
