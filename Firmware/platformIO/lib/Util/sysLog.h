/*
 sysLog.h - Base class that provides log() and logln()
 */

#ifndef sysLog_h
#define sysLog_h

#include <stdint.h>
#include <stddef.h>

#include "WString.h"
#include "Printable.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class MagicShifterSysLog {
    private:
        int write_error;
        size_t logNumber(unsigned long, uint8_t);
        size_t logFloat(double, uint8_t);
    protected:
        void setWriteError(int err = 1) {
            write_error = err;
        }
    public:
        MagicShifterSysLog() :
                write_error(0) {
        }

        int getWriteError() {
            return write_error;
        }
        void clearWriteError() {
            setWriteError(0);
        }

        virtual size_t write(uint8_t) = 0;
        size_t write(const char *str) {
            if(str == NULL)
                return 0;
            return write((const uint8_t *) str, strlen(str));
        }
        virtual size_t write(const uint8_t *buffer, size_t size);
        size_t write(const char *buffer, size_t size) {
            return write((const uint8_t *) buffer, size);
        }

        size_t logf(const char * format, ...)  __attribute__ ((format (logf, 2, 3)));
        size_t log(const __FlashStringHelper *);
        size_t log(const String &);
        size_t log(const char[]);
        size_t log(char);
        size_t log(unsigned char, int = DEC);
        size_t log(int, int = DEC);
        size_t log(unsigned int, int = DEC);
        size_t log(long, int = DEC);
        size_t log(unsigned long, int = DEC);
        size_t log(double, int = 2);
        size_t log(const Printable&);

        size_t logln(const __FlashStringHelper *);
        size_t logln(const String &s);
        size_t logln(const char[]);
        size_t logln(char);
        size_t logln(unsigned char, int = DEC);
        size_t logln(int, int = DEC);
        size_t logln(unsigned int, int = DEC);
        size_t logln(long, int = DEC);
        size_t logln(unsigned long, int = DEC);
        size_t logln(double, int = 2);
        size_t logln(const Printable&);
        size_t logln(void);
};

#endif
