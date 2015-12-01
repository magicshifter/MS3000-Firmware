/*
 sysLog.cpp - Base class that provides log() and logln()
 * based on printf model
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Arduino.h"

#include "sysLog.h"
extern "C" {
#include "c_types.h"
#include "ets_sys.h"
}

// Public Methods //////////////////////////////////////////////////////////////


/** 
UDP syslog .. 
// #undef DEBUG_SYSLOG
// #ifdef DEBUG_SYSLOG
    WiFiUDP udp;
    // udp.beginPacket("192.168.43.151", __SYSLOG_PORT);
    // when we connect to MS3000 to test, we usually get .2 ..
    udp.beginPacket("192.168.4.2", 514); // wks port for syslog
    udp.print(msg);
    udp.endPacket();
// #endif

*/


/* default implementation: may be overridden */
size_t ICACHE_FLASH_ATTR MagicShifterSysLog::write(const uint8_t *buffer, size_t size) {
    size_t n = 0;
    while(size--) {
        n += write(*buffer++);
    }
    return n;
}

size_t MagicShifterSysLog::logf(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char temp[1460];
  size_t len = ets_vsnprintf(temp, 1460, format, arg);
  len = print(temp);
  va_end(arg);
  return len;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(const __FlashStringHelper *ifsh) {
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);

    size_t n = 0;
    while (1) {
        uint8_t c = pgm_read_byte(p++);
        if (c == 0) break;
        n += write(c);
    }
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(const String &s) {
    return write(s.c_str(), s.length());
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(const char str[]) {
    return write(str);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(char c) {
    return write(c);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(unsigned char b, int base) {
    return print((unsigned long) b, base);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(int n, int base) {
    return print((long) n, base);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(unsigned int n, int base) {
    return print((unsigned long) n, base);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(long n, int base) {
    if(base == 0) {
        return write(n);
    } else if(base == 10) {
        if(n < 0) {
            int t = print('-');
            n = -n;
            return printNumber(n, 10) + t;
        }
        return printNumber(n, 10);
    } else {
        return printNumber(n, base);
    }
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(unsigned long n, int base) {
    if(base == 0)
        return write(n);
    else
        return printNumber(n, base);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(double n, int digits) {
    return printFloat(n, digits);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(const __FlashStringHelper *ifsh) {
    size_t n = print(ifsh);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::log(const Printable& x) {
    return x.printTo(*this);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(void) {
    return print("\r\n");
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(const String &s) {
    size_t n = print(s);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(const char c[]) {
    size_t n = print(c);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(char c) {
    size_t n = print(c);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(unsigned char b, int base) {
    size_t n = print(b, base);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(int num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(unsigned int num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(long num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(unsigned long num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(double num, int digits) {
    size_t n = print(num, digits);
    n += println();
    return n;
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logln(const Printable& x) {
    size_t n = print(x);
    n += println();
    return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logNumber(unsigned long n, uint8_t base) {
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // prevent crash if called with base == 1
    if(base < 2)
        base = 10;

    do {
        unsigned long m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);

    return write(str);
}

size_t ICACHE_FLASH_ATTR MagicShifterSysLog::logFloat(double number, uint8_t digits) {
    size_t n = 0;

    if(isnan(number))
        return print("nan");
    if(isinf(number))
        return print("inf");
    if(number > 4294967040.0)
        return print("ovf");  // constant determined empirically
    if(number < -4294967040.0)
        return print("ovf");  // constant determined empirically

    // Handle negative numbers
    if(number < 0.0) {
        n += print('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for(uint8_t i = 0; i < digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long) number;
    double remainder = number - (double) int_part;
    n += print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if(digits > 0) {
        n += print(".");
    }

    // Extract digits from the remainder one at a time
    while(digits-- > 0) {
        remainder *= 10.0;
        int toPrint = int(remainder);
        n += print(toPrint);
        remainder -= toPrint;
    }

    return n;
}
