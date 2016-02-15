#!/bin/sh
# INFO: uploads the firmware (firmwareName) to the MS3000, uses correct resetmethods so it's hads free
# ATTN expects https://github.com/igrr/esptool-ck in the parent directory. You have to "make" this to work
# baudrates: 115200 460800 921600

serialPort=/dev/ttyUSB0
firmwareName=MS3000_Firmware.bin

./esptool-ck/esptool -vv -cd ck -cb 460800 -cp /dev/ttyUSB0 -cf $firmwareName


