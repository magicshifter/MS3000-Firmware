#!/usr/bin/env python

import sys
import serial

portName = "/dev/ttyUSB0"
baudRate = 115200

def tryOpenSerial():
    try:
        port = serial.Serial(portName, baudRate, timeout=0)
        port.setRTS(False)
        return port
    except:
        return False

def resetESP(port):
    port.setDTR(False)
    port.setRTS(True)
    time.sleep(0.05)
    port.setRTS(False)

port = tryOpenSerial()

# resetESP(port)
if (not port):
    print ("Can't open serial .. aborting")
    exit (1)

# reset 
port.setDTR(False)
port.setRTS(True)
port.setRTS(False)

refresh = False
while True:
    if port:
        s = port.read(1)
        if (len(s) > 0 and s != 0):
            code = ord(s)
            if code > 13 and code <= 255:
                try:
                    sys.stdout.write(s)
                except:
                    pass
            elif code == 10:
                sys.stdout.write('\n')
            elif code == 13:
                sys.stdout.write('\r')
