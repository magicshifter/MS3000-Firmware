#!/bin/sh
#export ESP8266COM_BASEDIR=/home/ibisum/Documents/arduino-1.6.6/hardware/esp8266com/esp8266 
#export MS3KOS_BASE_DIR=/home/ibisum/Documents/MagicShifter3000/Firmware/MS3KOS 
MS3KOS_BUILD_DIR=/home/ibisum/Documents/builds/MagicShifter3000/MS3KOS_BUILD 
#alias flashit="python ../../esptool/esptool.py -p /dev/tty.SLAB_USBtoUART write_flash 0x00 /tmp/MS3KOS_BUILD/MS3KOS.bin"
alias flashit="python ../../esptool/esptool.py -p /dev/ttyUSB0 write_flash 0x00 $MS3KOS_BUILD_DIR/MS3KOS.bin"
alias flashwait="while \$(inotifywait -q -q $MS3KOS_BUILD_DIR/MS3KOS.bin) ; do echo "flashingit::" && flashit; done"
alias codewait="while \$(inotifywait -e modify  -q -q -r MS3KOS/); do echo "buildingit::" && make all; done"
