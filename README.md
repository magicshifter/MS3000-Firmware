
# MagicShifter3000 Base Firmware Project
##BASIC Documentation

The MagicShifter3000 is a versatile development platform for your application ideas. 

We provide the system with a basic firmware, from us to the user - but you are welcome to run your own firmware as well, and we support this effort.  It is possible to return the MagicShifter3000 to the original firmware, or run your own app forever - the choice is yours.

The purpose of this document is to describe the basic requirements for developing
your own firmware for the MagicShifter3000, and we hope you will find what you need.

##Architecture

The MagicShifter3000 uses the ESP8266 CPU to drive the LED hardware, and provide Wifi and other services.  The ESP8266 is a very versatile processor.  

##Toolchain

It is possible to use the ESP8266 compiler, but we suggest [platformio.org](http://platformio.org/#!/get-started) for new and experienced developers alike.

(You will also need Git for your computer, and a decent text editor of course.)

##Current Versions

As of the date of this document we use these operating systems to develop MagicShifter3000 Firmware packages:

OS Platforms:
	OSX 10.11.2 (15C50)
	Linux Ubuntu 15.10
	
Git:	2.7.0

PlatformIO, version 2.8.3

Python:	2.7.10

##Getting Started Checklist

* Download the Platform.io toolset 
* Update platformio if needed (`` platformio update && platformio upgrade ``)
* Connect your MagicShifter3000 to your computer with the USB cable.
* Clone the Base Firmware repository (`` git clone https://github.com/MagicShifter3000/whatever.git ``)

##Building a working Firmware

##Debugging Tools
	USB		:	MS3000_USB_Tool.py (!J! ??)
	Serial	:	screen /dev/tty.SLAB_USBtoUART (Mac OSX) 	screen /dev/ttyUSB0 (Linux)
	esptool	:	https://github.com/themadinventor/esptool

##Flashing an official Firmware release
	To flash the released .bin's from the MagicShifter team, you can use the esptool (see above) ..

	Linux: 
	$ ./esptool.py --baud 921600 --port /dev/tty.USB0 write_flash --flash_size 4m 0x0 firmware.bin

	OSX:
	$ ./esptool.py --baud 921600 --port /dev/tty.SLAB_USBtoUART write_flash --flash_size 4m 0x0 firmware.bin

##The MagicShifter3000 Filesystem
	.magicBitmap 
	.magicFont
	.magicText

	settings/ap.bin
	settings/server1.bin
	settings/aplist1.bin
	settings/preferredap.bin

##MagicShifter3000 Application Developer Kit - API Index

##Basic Source Code for a Module

##Factory Firmware - admin interface

http://magicshifter.local/index.html
http://magicshifter.local/admin.html
http://magicshifter.local/user.html

