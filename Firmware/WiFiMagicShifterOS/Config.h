#ifndef CONFIG_H
#define CONFIG_H

// remove define to enable ;)
//#define DISABLE_ACCEL

const bool forceAPMode = false;

const char *jsonLastAP =
//"{\"ssid\":\"PACIFIC\", \"pwd\":\"AllesR0ger\"}";
//"{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"}";
"{\"ssid\":\"wizme\", \"pwd\":\"lolinternets\"}";

const char *jsonAPList = "{\"list\": [\
{\"ssid\":\"metalab\", \"password\":\"\"},\
{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"},\
{\"ssid\":\"wizme\", \"pwd\":\"lolinternets\"}\
]}";

const char *jsonSoftAP = "{\"ssid\":\"MagicShifter3000\", \"pwd\":\"\"}";


#define POV_TIME_MICROSECONDS 50

#define LEDS 48
//#define LEDS 16

// PIN_LED_DATA an PIN_LED_CLOCK can not be changed since we use the hardware SPI
#define PIN_LED_DATA  13
#define PIN_LED_CLOCK 14
// double features as bootloader button
#define PIN_BUTTON1   0

#define PIN_I2C_DATA 4 // 5 //blau // labeled 5 on esp12E!!!
#define PIN_I2C_CLOCK 2 //lila

//#define MMA8452_ADDRESS 0x1D  // 0x1D if SA0 is high, 0x1C if low
#define MMA8452_ADDRESS 0x1C

#endif
