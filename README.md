
# MagicShifter3000 Base Firmware Project
## Introduction
The MagicShifter3000 is a versatile development platform for your application ideas. 

We provide the system with a basic firmware, from us to the user - but you are welcome to run your own firmware as well, and we support this effort.  It is possible to return the MagicShifter3000 to the original firmware, or run your own app forever - the choice is yours.

The purpose of this document is to describe the basic requirements for developing
your own firmware for the MagicShifter3000, and we hope you will find what you need.

## Architecture

The MagicShifter3000 uses the ESP8266 CPU to drive the LED hardware, and provide Wifi and other services.  The ESP8266 is a very versatile processor.  

## Toolchain

It is possible to use the ESP8266 compiler, but we suggest [platformio.org](http://platformio.org/#!/get-started) for new and experienced developers alike.

(You will also need Git for your computer, and a decent text editor of course.)

## Current Versions

As of the date of this document we use these operating systems to develop MagicShifter3000 Firmware packages:

	OS Platforms:
		OSX 10.11.2 (15C50)
		Linux Ubuntu 15.10
	
	Git:	2.7.0

	PlatformIO, version 2.8.3

	Python:	2.7.10

	React: 5.x

## Getting Started Checklist

## User Manual

[Don't forget to RTFM! Here is the User Manual: https://github.com/magicshifter/MS3000/wiki/UserManual](https://github.com/magicshifter/MS3000/wiki/UserManual)



* Download the Platform.io tools from [platformio.org](http://platformio.org/#!/get-started)
* Update platformio if needed (`` platformio update && platformio upgrade ``)
* Clone the Base Firmware repository (`` git clone https://github.com/magicshifter/MS3000.git ``)
* Read All README.md's!
* Connect your MagicShifter3000 to your computer with the USB cable.
* (`` cd MS3000 && make update && make ``)
* Test that your MagicShifter 3000 survived the final reboot.

## The MagicShifter 3000 Application Platform 

The Firmware consists of a base OS, (submodule: MagicShifter 3000-Firmware), which boots then uses and serves files from the MagicShifter 3000-Assets and MagicShifter 3000-WebInterface submodules.  The WebInterface as bundled is then served by an active MagicShifter 3000, and made available over the Wifi connection. This WebInterface then interacts dynamically with modules running on the MagicShifter 3000 as 'sub-applications' - the WebInterface module servces as the front-end to the onboard MagicShifter sub-module, according to the needs of the application.  For example, when the WebInterface for Pixel Editing is run by the user, it sends the results to the Shifter for use in the MagicShake/POV mode.  Other applications can use a similar model - for example a WebInterface for Arpeggiator editing can send .midi files to the MagicShifter 3000 for MIDI processing, etc.

## Project Master and Sub-module Repositories
We have organized the project as a single, master Git repository, which references the other sub-modules to compile a completed firmware image.  This means that you can drive the process of building a complete firmware image from the 'top-most' repository: 

	https://github.com/magicshifter/MS3000 - the preferred developer front door.
	
This master repository is set up to manage and produce the build products of each sub-module, assembling the results into a functioning MagicShifter 3000.  This is what we use to produce final Firmware releases.

However, it is possible to focus only on the specific module you are interested in - for example if you only wish to work on the onboard OS component of the Firmware, then you can clone MagicShifter 3000-Firmware sub-module, and use it independently to develop new OS versions.  MagicShifter 3000-WebInterface is also usable independently from the other modules, if you wish to isolate your work to the front-end Web application served by your MagicShifter 3000.

	MagicShifter 3000-Firmware:		C/C++ sources for the OS component.  
	MagicShifter 3000-WebInterface:	React-based project sources for the PixelEditor Web application, which is served on all new MagicShifter 3000's we ship.
	MagicShifter 3000-Assets:			Static files used by the other sub-modules for functionality - i.e. fonts, admin.html etc.

## General Developer Info:
	USB		:	VID:PID of the MagicShifter3000 is:  10C4:EA60
	Tools   :   Tools/MS3000_init.sh	- use this to automatically initialize an MagicShifter 3000 in stages: OS, filesystem, etc.
	Tools   :   Tools/MS3000Tool.py		- use this to administer an MagicShifter 3000 - i.e. format, etc.   
	Tools	:	Tools/readFirmware.sh  Tools/writeFirmware.sh - for reading and re-flashing entire onboard images (faster than in stages)
	Serial	:	MagicShifter 3000 debug messages are sometimes useful/interesting.  You can see them with screen /dev/tty.SLAB_USBtoUART (Mac OSX) 	screen /dev/ttyUSB0 (Linux) or you can use platformio monitor (See Makefiles)

	esptool	:	https://github.com/themadinventor/esptool
	esptool-ck : https://github.com/igrr/esptool-ck
	These two tools are, alas, commonly-named but have overlapping features.  We use both, since both are useful.  The urge to 'write a better esptool' must thus be resisted.  :)
	
	
##Official MagicShifter 3000 Team Firmware Releases 
[You can download the official team releases here:](https://github.com/magicshifter/MS3000/releases) https://github.com/magicshifter/MS3000/releases

We encourage you to use the MagicShifter 3000 for your needs - of course, please feel free to submit a Pull Request if you come up with something that you think should be in the official releases.

## Flashing a Firmware release
	To flash the released .bin's from the MagicShifter team, you can use the esptool (see above) ..

	Linux (Example): 
	$ ./esptool.py --baud 921600 --port /dev/tty.USB0 write_flash --flash_size 4m 0x0 MagicShifter 3000_Firmware_v0.9.0.bin

	OSX (Example):
	$ ./esptool.py --baud 921600 --port /dev/tty.SLAB_USBtoUART write_flash --flash_size 4m 0x0 MagicShifter 3000_Firmware_v0.9.0.bin

## The MagicShifter3000 Filesystem

We have some file types onboard the MagicShifter3000 which are useful to us:

	.magicBitmap - contains a POV bitmap for display.  These can be loaded into the PixelEditor for editing, also.
	.magicFont - simple font files, used by the MagicShifter3000 to display menu text, e.g. in "System Values" mode.

Also, we store AP settings and user configuration information in these files:

	settings/ap.bin
	settings/server1.bin
	settings/aplist1.bin
	settings/preferredap.bin

## magicshifter.local

When you enable WiFi on the MagicShifter 3000, it advertises itself on the local network as the name "magicshifter.local".  You can often use this name to address your MagicShifter.  When you connect to the MagicShifter 3000 as a client on its hosted AP, your device will have an address in the 192.168.4.x range, and the MagicShifter 3000 will have the address 192.168.4.1.

Once you have a WiFi connection with a MagicShifter 3000, it is possible to use the PixelEditor application, as well as admin interfaces, to make changes to the way your MagicShifter 3000 functions.  The links to these local addresses, which won't work unless you're currently connected to an active MagicShifter 3000), are:


[MagicShifter 3000 PixelEditor URL](http://magicshifter.local/index.html)

[MagicShifter 3000 local admin](http://magicshifter.local/admin.html)

The User Manual contains additional info on how to access and remotely program the MagicShifter3000.  User Manual is available here:

[https://github.com/magicshifter/MS3000/wiki/UserManual](https://github.com/magicshifter/MS3000/wiki/UserManual)


