#ifndef CONFIG_H
#define CONFIG_H



//#define MIDISHIFTER 1
// v1 == breadboard pcb
// 2 == 0.9 pcb :)
#define HWVERSION 2
#define VERSION 0.1

// remove define to enable ;)
//#define DISABLE_ACCEL

//crashes on new esp aruino toolchain :\
//#define USE_MDNS

#define MODES 3

// 192.168.4.1 is the IP it always has in softAP mode
const bool forceAPMode = true;


// with jqVic (Alpha, Beta)
///*
#define R601_VAL 270
#define R602_VAL 1000
//*/

// Metalab wih Flo (Gamma)
/*
#define R601_VAL 200
#define R602_VAL 820
//*/


/*
const char *jsonLastAP =
//"{\"ssid\":\"PACIFIC\", \"pwd\":\"AllesR0ger\"}";
//"{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"}";
"{\"ssid\":\"wizme\", \"pwd\":\"lolinternets\"}";

const char *jsonAPList = "{\"list\": [\
{\"ssid\":\"metalab\", \"pwd\":\"\"},\
{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"},\
{\"ssid\":\"wizme\", \"pwd\":\"lolinternets\"}\
]}";

const char *jsonSoftAP = "{\"ssid\":\"MagicShifter3000\", \"pwd\":\"\"}";
*/

#define LED_TYPE_APA102 1 
#define LED_TYPE_WS2801 2
#define LED_TYPE LED_TYPE_APA102



#define CONNECTION_TIMEOUT 30000

#if (LED_TYPE == LED_TYPE_APA102)
	#define SPI_FREQUENCY 20000000
#else
	#define SPI_FREQUENCY 500000
#endif

#define POV_TIME_MICROSECONDS 1



#define LEDS 16
//#define LEDS 160


#define PIN_LED_ENABLE 15

// PIN_LED_DATA an PIN_LED_CLOCK can not be changed since we use the hardware SPI
#define PIN_LED_DATA  13
#define PIN_LED_CLOCK 14

// double features as bootloader button
#define PIN_BUTTON_A  0
#define PIN_BUTTON_B 12

#define PIN_I2C_DATA 5 // 5 //blau // labeled 5 on esp12E!!!
#define PIN_I2C_CLOCK 4 //lila


#if (HWVERSION >= 2)
  //#define PIN_I2C_DATA 4 // 5 //blau // labeled 5 on esp12E!!!
  //#define PIN_I2C_CLOCK 5 //lila

  #define PWMGT_PIN 16
#else
  //#define PIN_I2C_DATA 4 // 5 //blau // labeled 5 on esp12E!!!
  //#define PIN_I2C_CLOCK 2 //lila
#endif

#define MIN_TIME_CLICK 10000
//#define MIN_TIME_LONG_CLICK 500000
//const int MIN_TIME_CLICK = 10000;
const int MIN_TIME_LONG_CLICK = 500000;

//#define MMA8452_ADDRESS 0x1D  // 0x1D if SA0 is high, 0x1C if low
#define MMA8452_ADDRESS 0x1C

#define MMA8452_ID 0x2A 

#define FILENAME_SIZE 40

#endif
