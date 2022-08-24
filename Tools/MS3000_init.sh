#!/bin/bash

CONFIG_FILE=settings_ap.bin

[[ $# > 1 ]] && SERIAL_PORT="$2" || SERIAL_PORT="/dev/ttyUSB0"

echo "using nr $1 and serialport $SERIAL_PORT"

awk  'BEGIN{ printf "%s\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "'$1'"}' > $CONFIG_FILE

python ./MS3000Tool.py init $SERIAL_PORT 

