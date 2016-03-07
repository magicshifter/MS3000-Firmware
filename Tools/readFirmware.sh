#!/bin/sh
# INFO: dumps all 4MB of the MS3000 and stores it in a file (firmwareName)
# ATTN: expects https://github.com/themadinventor/esptool in the parent directory
# HOWTO use: you have to hold the Bootloader button down till 
# the readout has started then you can release it
# baudrates: 115200 460800 921600

firmwareName=MS3000_Firmware_0.9.1.bin

./esptool/esptool.py --port $1 --baud 921600 read_flash 0x00000 0x400000 $firmwareName 

#./esptool.py --baud 460800 read_flash 0x00000 0x3FFFFF $dumpname // 460800
