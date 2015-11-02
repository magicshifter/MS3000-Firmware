dumpname=firmware-0x0000.bin
buildname=./build/WiFiMagicShifterOS.cpp.bin
targetdir=./messedUp/$1

./esptool.py --baud 921600 write_flash 0x00000 ./build/WiFiMagicShifterOS.cpp.bin
