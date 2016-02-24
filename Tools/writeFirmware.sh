#!/bin/sh
# INFO: uploads the firmware (firmwareName) to the MS3000, uses correct resetmethods so it's hads free
# ATTN expects https://github.com/igrr/esptool-ck in the parent directory. You have to "make" this to work
# baudrates: 115200 460800 921600

if [ "$#" -ne 2 ]; then
	firmwareName=MS3000_Firmware.bin
else
	firmwareName=$2
fi	

echo "Firmware Name: " $firmwareName
./esptool-ck/esptool -vv -cd ck -cb 460800 -cp $1  -cf $firmwareName


