//
// msConfig.h - basic, build oriented configuration options
//

#ifndef MS_CONFIG_H
#define MS_CONFIG_H

#undef DEBUG_OUTPUT

// MIDI and OSC enabled?
// #define CONFIG_ENABLE_OSC

//#define CONFIG_ENABLE_MIDI
// serial i/o is unavailable if hardware MIDI is enabled

#ifdef CONFIG_ENABLE_MIDI
#define CONFIG_ENABLE_MIDI_RTP
#define CONFIG_ENABLE_MIDI_SERIAL
#endif

// if MIDI-serial is configured, we will send messages to syslog only (if configured)
#ifdef CONFIG_ENABLE_MIDI_SERIAL
#warning "SERIAL OUTPUT is DISABLED : MIDI Serial is configured"
#undef _DO_SERIAL_OUTPUT
#else
#define _DO_SERIAL_OUTPUT
#endif

// Accelerometer enabled?
#define CONFIG_ENABLE_ACCEL

// Chip ID's's
// 16299018 // jv's proto
// 16298245 // 'p2' proto
//  1210204 //"in-production" Chip

#define AP_NAME_OVERRIDE "MS3000"

// HW_ID is passed one of these constants from Makefile -DHW_ID=.. 

// set of Hw configurations 
#define HW_ID_MS3000_FSOX8700CQ 	1	// MS3000 config magnetometer+accel
#define HW_ID_MS3000_MMA8452Q 		2	// accel only used in early prototypes
#define HW_ID_RING   				3	// with attached strip used in early prototypes
#define HW_ID_SABRE   				4	// with attached strip used in early prototypes

#define HW_ID HW_ID_MS3000_FSOX8700CQ

// set of LED types
#define LED_TYPE_APA102 1
#define LED_TYPE_WS2801 2

// define a default
#define LED_TYPE LED_TYPE_APA102

// v1 == breadboard pcb
// 2 == 0.9 pcb :)
#define MS3KOS_VERSION "1.3.1"

#define MIN_TIME_CLICK 10000
#define MIN_TIME_LONG_CLICK 1000 * 750

#define MAX_TIME_DOUBLE_CLICK 375

#define POWER_TIMEOUT 1000 * 1000 * 10 * 60

// Power management pin
#define PIN_PWR_MGT 16

#define MAX_FILENAME_LENGTH 32
#define MAX_TEXT_LENGTH 16


#define BUTTON_PWR_THRESHOLD_DEFAULT 970

// !J! we want to configure this
#undef CONFIG_PWR_FORCE 

#define CALIBRATION_FILENAME "settings/calibration.bin"

// 192.168.4.1 is the IP it always has in softAP mode
#define SCAN_FIRST_MODE 1

#define CONNECTION_TIMEOUT 30000

#define POV_TIME_MICROSECONDS 100

#define USE_MDNS

#define DEFAULT_SHAKE_IMAGE "heart.magicBitmap"

// which MMA is in use
#if(HW_ID==HW_ID_MS3000_FSOX8700CQ)
// #warning "HW ID :  HW_ID_MS3000_FSOX8700CQ "
#define MAX_LEDS 16
#define CONFIG_MMA_FSOX
#elif(HW_ID==HW_ID_MS3000_MMA8452Q)
// #warning "HW ID :  HW_ID_MS3000_MMA8452Q "
#define MAX_LEDS 16
#define CONFIG_MMA_NORMAL
#elif(HW_ID==HW_ID_RING)
// #warning "HW ID :  HW_ID_RING "
#define MAX_LEDS 160
#define CONFIG_MMA_FSOX
#undef LED_TYPE
#define LED_TYPE LED_TYPE_WS2801
#elif(HW_ID==HW_ID_SABRE)
// #warning "HW ID :  HW_ID_SABRE "
#define MAX_LEDS 48
#define CONFIG_MMA_FSOX
#else
#error "No HW_ID configured! abort!"
#endif

// !J! this enables quick spool hacks
//#define CONFIG_CUSTOM_MAX_LEDS
#ifdef CONFIG_CUSTOM_MAX_LEDS
#undef MAX_LEDS
#define MAX_LEDS 64
#endif

#define MAX_LEDS 60
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

#define BYTES_PER_LED 4
//#define MAX_LEDS 160
#define RGB_BUFFER_SIZE (BYTES_PER_LED*MAX_LEDS)

// double features as bootloader button
#define PIN_BUTTON_A 12
#define PIN_BUTTON_B 0

#define DEFAULT_FACTORY_MODE 0
//
// Global definitions and the master global struct for the system
//

#define COLUMNMULTIPLY 2

#define FAULT_NO_ACCELEROMETER 0xf1
#define FAULT_VERY_LOW_POWER 0xf2

#endif
