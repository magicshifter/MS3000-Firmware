//
// msConfig.h - basic, build oriented configuration options
//

#ifndef MS_CONFIG_H
#define MS_CONFIG_H

#undef DEBUG_OUTPUT

// Chip ID's's
// 16299018 // jv's proto
// 16298245 // 'p2' proto
//  1210204 //"in-production" Chip

#define AP_NAME_OVERRIDE "MS3000.prototype"

// HW_ID is passed one of these constants from Makefile -DHW_ID=.. 

// set of Hw configurations	
#define HW_ID_MS3000_FSOX8700CQ 	1		// magnetometer+accel
#define HW_ID_MS3000_MMA8452Q 		2		// accel
#define HW_ID_RING   				3		// on the wall
#define HW_ID_SABRE   				4		// proto-j-beta light wandstrip, new ESP model

#define HW_ID HW_ID_MS3000_FSOX8700CQ
// #define HW_ID HW_ID_MS3000_MMA8452Q

// set of LED types
#define LED_TYPE_APA102 1 
#define LED_TYPE_WS2801 2

// define a default
#define LED_TYPE LED_TYPE_APA102

// v1 == breadboard pcb
// 2 == 0.9 pcb :)
#define VERSION 0.1

#define MIN_TIME_CLICK 10000
#define MIN_TIME_LONG_CLICK 2000000

#define MAX_FILENAME_LENGTH 32

#define NUM_MS_MODES 2

// MIDI enabled?
//#define CONFIG_ENABLE_MIDI
// Accelerometer enabled?
#define CONFIG_ENABLE_ACCEL

// 192.168.4.1 is the IP it always has in softAP mode
// #define SCAN_FIRST_MODE 1

#define CONNECTION_TIMEOUT 30000

#define POV_TIME_MICROSECONDS 1

#define USE_MDNS

// which MMA is in use
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
#define CONFIG_MMA_FSOX
#undef LED_TYPE
#define LED_TYPE LED_TYPE_WS2801
#elif(HW_ID==HW_ID_SABRE)
#warning "HW ID :  HW_ID_SABRE "
#define MAX_LEDS 48
#define CONFIG_MMA_FSOX
#else
#error "No HW_ID configured! abort!"
#endif

// #define MMA8452_ADDRESS 0x1C
// #define MMA8452_ID 0x2A

// normal MMA 
#ifdef CONFIG_MMA_NORMAL
  #define MMA8452_ADDRESS 0x1C
  #define MMA8452_ID 0x2A
#else
  #ifdef CONFIG_MMA_FSOX
// magnet sensor version
    #define MMA8452_ADDRESS 0x1E
    #define MMA8452_ID 0xC7
  #endif
#endif

//#define MAX_LEDS 160
#define RGB_BUFFER_SIZE (4*MAX_LEDS)

// Use pre-configured AP list
#undef CONFIG_USE_DEBUG_AP

#endif
