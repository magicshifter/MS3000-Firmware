find /home/wizard23/.platformio/ -name common.h -exec ls -l {} \;
-rwxrwx--- 2 wizard23 wizard23 2718 Apr 13 20:13 /home/wizard23/.platformio/packages/framework-arduinoespressif/variants/generic/common.h




ameworkArduino -I.pioenvs/esp12e/FrameworkArduinoVariant .pioenvs/esp12e/FrameworkArduino/HardwareSerial.cpp
In file included from .pioenvs/esp12e/FrameworkArduino/FS.h:25:0,
from src/MS3KOS.cpp:28:
/home/wizard23/.platformio/packages/toolchain-xtensa/xtensa-lx106-elf/include/c++/4.8.2/bits/stl_vector.h: In member function 'std::vector<_Tp, _Alloc>::size_type std::vector<_Tp, _Alloc>::_M_check_len(std::vector<_Tp, _Alloc>::size_type, const char*) const':
.pioenvs/esp12e/FrameworkArduino/Arduino.h:254:18: error: expected unqualified-id before '(' token
#define max(a,b) ((a)>(b)?(a):(b))
^
/home/wizard23/.platformio/packages/toolchain-xtensa/xtensa-lx106-elf/include/c++/4.8.2/bits/stl_bvector.h: In member function 'std::vector<bool, _Alloc>::size_type std::vector<bool, _Alloc>::_M_check_len(std::vector<bool, _Alloc>::size_type, const char*) const':
.pioenvs/esp12e/FrameworkArduino/Arduino.h:254:18: error: expected unqualified-id before '(' token
#define max(a,b) ((a)>(b)?(a):(b))
^
xtensa-lx106-elf-g++ -o .pioenvs/esp12e/F





#ifdef __cplusplus

#include "pgmspace.h"

#include "WCharacter.h"
#include "WString.h"

#include "HardwareSerial.h"
#include "Esp.h"
#include "Updater.h"
#include "debug.h"

// HACK wizard23
#undef min
#undef max
// END HACK

#ifndef _GLIBCXX_VECTOR
// arduino is not compatible with std::vector
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);