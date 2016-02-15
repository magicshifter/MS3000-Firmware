#!/usr/bin/env python
#coding=utf8
import serial
from time import sleep
from subprocess import call
import re
import os
import sys
import array
import time

device = None #"/dev/ttyACM0" Device needs to be found first.
baudrate = 921600 #Baudrate goes here
pathToProgrammerScript = "./magic_flash.sh"
#pathToProgrammerScript = "./magic_flash_noeeprom.sh"
#pathToProgrammerScript = "../Tester/raspberry/magic_flash_noeeprom.sh"
#pathToProgrammerScript = "../Tester/raspberry/magic_flash.sh"
#pathToProgrammerScript = "/home/pi/magicshifter/magic_flash.sh"

    
#==Response==
#Every Response has an extra string which will be sent
#Specification of occured error or success followes any Response (except MAGIC_PONG)
#=================================



def read_single_keypress():
    """Waits for a single keypress on stdin.

    This is a silly function to call if you need to do it a lot because it has
    to store stdin's current setup, setup stdin for reading single keystrokes
    then read the single keystroke then revert stdin back after reading the
    keystroke.

    Returns the character of the key that was pressed (zero on
    KeyboardInterrupt which can happen when a signal gets handled)

    """
    import termios, fcntl, sys, os
    fd = sys.stdin.fileno()
    # save old state
    flags_save = fcntl.fcntl(fd, fcntl.F_GETFL)
    attrs_save = termios.tcgetattr(fd)
    # make raw - the way to do this comes from the termios(3) man page.
    attrs = list(attrs_save) # copy the stored version to update
    # iflag
    attrs[0] &= ~(termios.IGNBRK | termios.BRKINT | termios.PARMRK 
                  | termios.ISTRIP | termios.INLCR | termios. IGNCR 
                  | termios.ICRNL | termios.IXON )
    # oflag
    attrs[1] &= ~termios.OPOST
    # cflag
    attrs[2] &= ~(termios.CSIZE | termios. PARENB)
    attrs[2] |= termios.CS8
    # lflag
    attrs[3] &= ~(termios.ECHONL | termios.ECHO | termios.ICANON
                  | termios.ISIG | termios.IEXTEN)
    termios.tcsetattr(fd, termios.TCSANOW, attrs)
    # turn off non-blocking
    fcntl.fcntl(fd, fcntl.F_SETFL, flags_save & ~os.O_NONBLOCK)
    # read a single keystroke
    try:
        ret = sys.stdin.read(1) # returns a single character
    except KeyboardInterrupt: 
        ret = 0
    finally:
        # restore old state
        termios.tcsetattr(fd, termios.TCSAFLUSH, attrs_save)
        fcntl.fcntl(fd, fcntl.F_SETFL, flags_save)
    print ret
    return ret


#===============Colors: HELPER=================
class bcolors:
    HEADER = '\033[35m'
    OKBLUE = '\033[34m'
    OKGREEN = '\033[32m'
    WARNING = '\033[33m'
    FAIL = '\033[31m'
    ENDC = '\033[0m'

    def disable(self):
        self.HEADER = ''
        self.OKBLUE = ''
        self.OKGREEN = ''
        self.WARNING = ''
        self.FAIL = ''
        self.ENDC = ''

#============Colors: USEABLE===================
def pFail(s):
	print bcolors.FAIL + s + bcolors.ENDC
	return
def pOK(s):
	print bcolors.OKGREEN + s + bcolors.ENDC
	return
#================================================
#==========================Call Programmer: USEABLE/HELPER=========================
def callProgrammer():
	try:
		fi, fo, fe= os.popen3("sudo sh " + pathToProgrammerScript)
		stderr = fe.readlines()
		stdout = fo.readlines()
		#print stderr
		#print stdout
		if len(stderr) > 0:
			match = re.match("^.*?" + "no device present" + ".*?$", stderr[len(stderr)-1]) #Last line of output of magic_flash.sh
			if(match): # If there's no device present, programming failed.
				print stderr
				return False
			match = re.match("^.*?" + "bytes used" + ".*?$", stderr[len(stderr)-1]) #Last line of output of magic_flash.sh
			if(not match): # something else is wrong!
				match = re.match("^.*?" + "dfu-programmer: failed to release interface" + ".*?$", stderr[len(stderr)-1]) #Last line of output o magic_flash.sh in the weird case of my ubunu 13.x mach8ine
				if(not match): # something else is wrong!
					print stderr
					return False
			return True
		print stderr
		return False
	except IOError as e:
		print e
		return False

def dmesgPing():
	try:
		fi, fo, fe= os.popen3("sudo dmesg")
		stderr = fe.readlines()
		stdout = fo.readlines()
		#print stderr
		#print stdout
		if len(stdout) > 0:
			for line in stdout:
				match = re.match("^.*?" + "ATm32U4DFU" + ".*?$", line) #
				if(match): 
					return True				
#			match = re.match("^.*?" + "new full speed USB device" + ".*?$", stdout[len(stdout)-1]) #
#			if(match): 
			#return True # for pc side
			return True	
		return False
	except IOError as e:
		print e
		return False

def dmesgDisconnected():
	try:
		fi, fo, fe= os.popen3("sudo dmesg")
		stderr = fe.readlines()
		stdout = fo.readlines()
		#print stderr
		#print stdout
		if len(stdout) > 0:
			for line in stdout:
				match = re.match("^.*?" + "USB disconnect" + ".*?$", line) #
				if(match): 
					return True				
#			match = re.match("^.*?" + "new full speed USB device" + ".*?$", stdout[len(stdout)-1]) #
#			if(match): 
			#return True # for pc side
			return False	
		return False
	except IOError as e:
		print e
		return False

def dmesgClear():
	try:
		fi, fo, fe= os.popen3("sudo dmesg -c")
		stderr = fe.readlines()
		stdout = fo.readlines()
		#print stderr
		#print stdout
		if len(stderr) > 0:
			match = re.match("^.*?" + "no device present" + ".*?$", stderr[len(stderr)-1]) #Last line of output of magic_flash.sh
			if(match): # If there's no device present, programming failed.
				return False
			match = re.match("^.*?" + "bytes used" + ".*?$", stderr[len(stderr)-1]) #Last line of output of magic_flash.sh
			if(not match): # something else is wrong!
				return False
			return True
		return False
	except IOError as e:
		print e
		return False

#===============================================================================



#===========================Search Magic Shifter: HELPER===============
def getDevices():
	try:
		#fi, fo, fe= os.popen3("ls /dev/tty.usbmodem*")
		#fi, fo, fe= os.popen3("ls /dev/ttyACM*")
		fi, fo, fe= os.popen3("ls /dev/ttyACM* /dev/tty.usbmodem*")
		stderr = fe.readlines()
		f = fo.readlines()
		devList = []
		for val in f:
				devList.append(val.strip())
		return devList
	except IOError as e:
		print e
		return None

def isMagicShifter(dev):
	dev = dev
	print issueCommand(cmdset[0])[0]
	try:
		if("MAGIC_PONG" == issueCommand(cmdset[0])[0]):
			return dev
		else:
			return None
	except:
		return None
#================Search Magic Shifter: USABLE===========================================
def findMagicShifter():
	devList = getDevices()
	for dev in devList:
		if(isMagicShifter(dev)):
			return dev

def findMagicShifterSimple():
	if device:
		return device
	devList = getDevices()
	if (len(devList) > 0):
		return devList[0]
	return None
#================================================================

def openPort(timeout):
	ser = serial.Serial(device, baudrate=baudrate, timeout=timeout)#timeout=None
	ser.setRTS(True)
	sleep(0.1)
	ser.setDTR(False)
	ser.setRTS(False)
#	ser.open()
	sleep(6)
	return ser

	


def issueDump(device, cmd, timeout):	
	ser = None
	try:    
		ser = serial.Serial(device, baudrate, timeout=timeout)#timeout=None
		port.setRTS(False)
		ser.write(cmd)
		response = ser.readline()
		return response
	except Exception as e:
		pFail("UNEXPECTED ERROR WHILE sending: " + cmd)
		print type(e)
		pFail(str(e))
		return None	
	finally:
		if (ser != None):
			#print "FINALY closing!"
			try: 
				ser.close()
			except Exception as e:
				print "FINALY exception!"
				pFail(e)

#==================Communication: USABLE/HELPER=================================
def issueCommand(device, cmd, timeout):	
	ser = None
	try:    
		ser = serial.Serial(device, baudrate, timeout=timeout)#timeout=None
		ser.write(cmd)
		response = ser.readline()
		response = response.split(':', 1) #Splits the response at the first ":"
		response[0] = response[0].strip()
		return response
	#except SerialTimeoutException as e:
	#	print e
	#	pFail("Error! Device not responding. Message sent: " + cmd)
	#	return None
	except Exception as e:
		pFail("UNEXPECTED ERROR WHILE sending: " + cmd)
		pFail(str(e))
		return None	
	finally:
		if (ser != None):
			#print "FINALY closing!"
			try: 
				ser.close()
			except Exception as e:
				print "FINALY exception!"
				pFail(e)


def debugConsole():	
	device = findMagicShifterSimple()
	if (device == None):
		pFail("No serial found! No DEBUG Console possible")
		return False
	ser = None
	try:    
		ser = serial.Serial(device, baudrate)#timeout=None
		print "opened: " + device + " now dumping"		
		while 1:
			data = ser.read(1)
			if data != "":
				sys.stdout.write(data)
				sys.stdout.flush()
				#print hex(ord(data))
	except Exception as e:
		pFail("UNEXPECTED ERROR while RESET!")
		pFail(str(e))
		return None	
	finally:
		if (ser != None):
			#print "FINALY closing!"
			try: 
				ser.close()
			except Exception as e:
				print "FINALY exception!"
				pFail(e)


# this only works if I press the reset button at least once manually
# TODO: find out what the reason is and use the Leonardo style reset (open and close serial at 1200 baud)
def ResetIntoBootloaderLeonardo():
	device = findMagicShifterSimple()
	if (device == None):
		pFail("No serial found! No DEBUG Console possible")
		return False
	ser = None
	try:    
		ser = serial.Serial(device, 1200)#timeout=None
		ser.close()		
	except Exception as e:
		pFail("UNEXPECTED ERROR while RESET!")
		pFail(str(e))
		return None	
	finally:
		if (ser != None):
			#print "FINALY closing!"
			try: 
				ser.close()
			except Exception as e:
				print "FINALY exception!"
				pFail(e)

def ResetIntoBootloader():
	device = findMagicShifterSimple()
	if (device == None):
		pFail("No serial found! No RESET possible")
		return False

	cmd = "MAGIC_RESET"
	ser = None
	resetHappened = False
	try:    
		ser = serial.Serial(device, baudrate, timeout=0.5)#timeout=None
		try:
			ser.write(cmd)
			ser.read() # read one byte to find out if connection still works
		except Exception as e:
			# this SHOULD happen because we try to read a byte above but the serial is disconnected because the reset happened
			resetHappened = True
		finally:
			if (resetHappened):
				pOK("RESET done")
			else:
				pFail("RESET sent but serial connection still here? Does this shifter already implement reset?")
	except Exception as e:
		pFail("UNEXPECTED ERROR WHILE sending: " + cmd)
		print type(e)
		pFail(str(e))
		return None	
	


def StartFromBootloader():
	try:
		fi, fo, fe= os.popen3("dfu-programmer atmega32u4 start")
		stderr = fe.readlines()
		stdout = fo.readlines()
		print stderr
		print stdout
	except IOError as e:
		print e
		return False
		
#=======================================================================================
def dump():
	device = findMagicShifterSimple()
	if (device == None):
		pFail("No serial found! No DUMP possible")
		return False
	response = issueDump(device, "MAGIC_DUMP", 3)
	if(response == None):
		pFail("Error! Device not responding to DUMP.")
		return False
	pOK("DUMPED: " + str(response))
	return True

def ping():
	device = findMagicShifterSimple()
	if (device == None):
		pFail("No serial found! No PING possible")
		return False
	response = issueDump(device, "MAGIC_PING", 1)
	if(response == None):
		pFail("Error! Device not responding to PING.")
		return False
	pOK("PINGED: " + str(response))
	return True

def Str2Hex(s):
	return ':'.join(x.encode('hex') for x in s)

def SendInChunks(ser, dataString, chunkSize, delay):
	l = len(dataString)
	for pos in xrange(0, l, chunkSize):
		chunk = dataString[pos:min(l, pos + chunkSize)]
		ser.write(chunk)	
		ser.flush()
		sleep(delay)

def issueUpload(device, data, filename):	
	ser = None
	try:    
		print "openimng port"
		#ser = serial.Serial(device, baudrate, timeout=5)#timeout=None
		ser = openPort(5)
		ser.write("MAGIC_UPLOAD")

		print "writeUpload"

		dataLen = len(data)

		headerString = map(ord,filename)
		headerString.insert(0, 0x30)
		headerString.insert(1, 0)
		headerString.insert(2, 0)
		headerString.insert(3, 0)

		headerString.insert(4, (dataLen>>24)&0xFF)
		headerString.insert(5, (dataLen>>16)&0xFF)
		headerString.insert(6, (dataLen>>8)&0xFF)
		headerString.insert(7, (dataLen>>0)&0xFF)


		while len(headerString) < 256:
			headerString.insert(len(headerString), 0)

		print headerString
			 
		headerString = array.array('B', headerString).tostring()
		print dataLen
		#print Str2Hex(headerString)	
		sleep(0.5)	
		ser.write(headerString)
		sleep(0.5)
		#sleep(0.5)			
		dataString = array.array('B', data).tostring()
		#print Str2Hex(dataString)	
		SendInChunks(ser, dataString, 32, 0.003) #0.003	

		response = ser.readline()
		return response
	except Exception as e:
		pFail("UNEXPECTED ERROR WHILE sending UPLOAD!")
		pFail(str(e))
		return None	
	finally:
		if (ser != None):
			try: 
				ser.close()
			except Exception as e:
				print "FINALY exception!"
				pFail(e)

def issueShow(device, sector):	
	ser = None
	try:    
		ser = serial.Serial(device, baudrate, timeout=0.5)#timeout=None
		ser.write("MAGIC_DISPLAY")
		sleep(0.5)
		headerString = array.array('B', [sector]).tostring()
		ser.write(headerString)
		response = ser.readline()
		return response
	except Exception as e:
		pFail("UNEXPECTED ERROR WHILE sending DISPLAY!")
		pFail(str(e))
		return None	
	finally:
		if (ser != None):
			try: 
				ser.close()
			except Exception as e:
				print "FINALY exception!"
				pFail(e)

def upload(fileName, sector):	
	with open(fileName, "rb") as f:
		bytes = f.read()
	device = findMagicShifterSimple()
	if (device == None):
		pFail("No serial found! No UPLOAD possible")
		return False
	response = issueUpload(device, bytes, sector)
	if(response == None):
		pFail("Error! Device not responding to UPLOAD.")
		return False
	pOK("UPLOADRET: " + str(response))
	return True

def show(sector):	
	device = findMagicShifterSimple()
	if (device == None):
		pFail("No serial found! No DISPLAY possible")
		return False
	response = issueShow(device, sector)
	if(response == None):
		pFail("Error! Device not responding to DISPLAY.")
		return False
	pOK("UPLOADRET: " + str(response))
	return True

def main():
#1.Program Shifter
	# das kommt ins shellscript :)
	#print "Presss enter to continue."
	#raw_input()

	
	print "--> Insert device, mount it with bracket, connect battery and connect USB cable. Then press ENTER to continue"
	key = read_single_keypress()

	dmesgClear()

	print "--> Press the RESET button"
	cnt = 0
	while (not dmesgPing()):
			sleep(0.1)
			cnt = cnt + 1
			if (cnt > 100):
				pFail("Reset Button was not pressed within 10 seconds. Is it broken?")
				return False
	pOK("Reset Button OK!")

	print "--> If the BLUE 'CHRG' LED is blinking and the GREEN 'FULL' LED is on press 'Y' else press 'N'"
	key = read_single_keypress()
	if (key != 'Y' and key != 'y'):
		pFail("Is the charger IC working? Or are the LEDs broken?")
		return False
	pOK("LEDs 'CHRG' and 'FULL' verified by user as OK")

	pOK("programming device...")
	if(callProgrammer()):
		pOK("bootloader OK! :)")
	else:
		pFail("USB connection does not work or Bootloader is not responding. :(")
		return False

	sleep(1)

	device = None
	for step in range(0, 80):
		sleep(0.1)
		device = findMagicShifterSimple()
		if (device != None):
			break

	if (device == None):
		pFail("Error! Device is not showing up as USB-Serial port")
		return False
	
	pOK("device found at " + device)
	sleep(1)
	
#2.Check Shifter
	response = issueCommand(device, "MAGIC_AUTO", 15)
	if(response == None):
		pFail("Error! Device not responding to AUTO TESTING.")
		return False
	elif(response[0] == "MAGIC_OK"):		
		pOK("Automated Test OK");
	elif(response[0] == "MAGIC_ERROR"):
		pFail("ERROR WHILE AUTO TESTING: " + str(response))
		return False
	else:
		pFail("UNKNOWN RESPONSE WHILE AUTO TESTING: " + str(response))
		return False

	response = issueCommand(device, "MAGIC_PING", 2)
	if(response == None):
		pFail("Error! Device not responding to PING. TCHRG not working?")
		return False
	elif(response[0] == "MAGIC_PONG"):		
		pOK("PING (TCHRG) OK :)");
	else:
		pFail("UNKNOWN RESPONSE WHILE PING: " + str(response))
		return False

	print "--> Please Press Button 1 and then Button 2"
	response = issueCommand(device, "MAGIC_MANUAL", 15)
	if(response == None):
		pFail("Error! Device not responding to MANUAL TESTING.")
		return False
	elif(response[0] == "MAGIC_OK"):		
		pOK("Buttons OK")

		print("--> Please check if the BLUE 'LEDDEBUG', YELLOW 'LEDRX'  and the RED 'LEDTX' are blinking! Press 'Y' if the 3 LEDs are blinking else press 'N'");
		key = read_single_keypress()
		if (key != 'Y' and key != 'y'):
			pFail("LEDs not working please check 'LEDDEBUG','LEDRX', 'LEDTX'")
			return False
		pOK("LEDs 'LEDDEBUG', 'LEDRX' and 'LEDTX' verified by user as OK")
		dmesgClear()
		print("--> Press the POWER button");
		cnt = 0
		while (not dmesgDisconnected()):
			sleep(0.1)
			cnt = cnt + 1
			if (cnt > 100):
				pFail("Power Button was not pressed within 10 second. Is the power button broken?")
				return False
		pOK("Power Buttons Work!")
		pOK("--> TEST SUCCESSFULL! :)")	
		return True
	elif(response[0] == "MAGIC_ERROR"):
		pFail("ERROR WHILE MANUAL TESTING: " + str(response))
		return False
	else:
		pFail("UNKNOWN RESPONSE WHILE MANUAL TESTING: " + str(response))
		return False




def initMS3000():
	start = time.time()
	delay = 1;

	ser = openPort(5)
	issueUploadMS3000(ser, "settings_ap.bin", "settings/ap.bin")
	sleep(delay)
	issueUploadMS3000(ser, "../../Assets/MS3000_defaultconfig/index.html", "index.html")
	sleep(delay)
	issueUploadMS3000(ser, "../../Assets/MS3000_defaultconfig/index.js.gz", "index.js.gz")
	sleep(delay)
	issueUploadMS3000(ser, "../../Assets/MS3000_defaultconfig/magicshifter.appcache", "magicshifter.appcache")
	sleep(delay)
	issueUploadMS3000(ser, "../../Assets/MS3000_defaultconfig/MagicShifter.html", "MagicShifter.html")
	sleep(delay)
	issueUploadMS3000(ser, "../../Assets/MS3000_defaultconfig/admin.html", "admin.html")

	imgs = ["heart.magicBitmap", "smilie.magicBitmap", "star.magicBitmap", "oneup.magicBitmap", "mario.magicBitmap", "mario.magicBitmap", "blueGhost.magicBitmap", "redGhost.magicBitmap", "BubbleBobble.magicBitmap", "invader.magicBitmap", "giraffe.magicBitmap", "cursor.magicBitmap", "nyancat.magicBitmap"]

	for img in imgs:
		sleep(delay)
		issueUploadMS3000(ser, "../../Assets/magicBitmaps/" + img, img)

        fonts = ["font10x16.magicFont", "font4x5.magicFont", "font6x8.magicFont", "font7x12.magicFont"]
	for font in fonts:
		sleep(delay)
		issueUploadMS3000(ser, "../../Assets/fonts/" + font, font)

	end = time.time()
	print "time elapsed: ", end - start

def issueUploadMS3000(ser, sourceFilename, targetFilename):	

	with open(sourceFilename, "rb") as f:
		data = f.read()

	try:    
		ser.write("MAGIC_UPLOAD")

		print "uploading ", sourceFilename, " -> ", targetFilename 

		dataLen = len(data)

		headerString = map(ord, targetFilename)
		headerString.insert(0, 0x30)
		headerString.insert(1, 0)
		headerString.insert(2, 0)
		headerString.insert(3, 0)

		headerString.insert(4, (dataLen>>24)&0xFF)
		headerString.insert(5, (dataLen>>16)&0xFF)
		headerString.insert(6, (dataLen>>8)&0xFF)
		headerString.insert(7, (dataLen>>0)&0xFF)


		while len(headerString) < 256:
			headerString.insert(len(headerString), 0)

		print headerString
			 
		headerString = array.array('B', headerString).tostring()
		print dataLen
		#print Str2Hex(headerString)	
		sleep(0.5)	
		ser.write(headerString)
		sleep(0.5)
		#sleep(0.5)			
		dataString = array.array('B', data).tostring()
		#print Str2Hex(dataString)	
		SendInChunks(ser, dataString, 32, 0.004) #0.003	

		response = ser.readline()
		return response
	except Exception as e:
		pFail("UNEXPECTED ERROR WHILE sending UPLOAD!")
		pFail(str(e))
		return None	


def formatMS3000(ser):	
	try:    
		start = time.time()
		ser.write("MAGIC_FORMAT")
		response = ser.readline()
		return response
	except Exception as e:
		pFail("UNEXPECTED ERROR WHILE sending UPLOAD!")
		pFail(str(e))
		return None	

if __name__ == '__main__':
	if (len(sys.argv) >= 2 and sys.argv[1] == "format"):
		device = sys.argv[2]
		start = time.time()
		ser = openPort(5)
		formatMS3000(ser)
		end = time.time()
		print "time elapsed: ", end - start		

	if (len(sys.argv) >= 2 and sys.argv[1] == "init"):
		device = sys.argv[2]
		initMS3000()
		
	if (len(sys.argv) >= 4 and sys.argv[1] == "up"):
		if (len(sys.argv) == 5):
			device = sys.argv[4]
		upload(sys.argv[2], sys.argv[3])
	elif (len(sys.argv) >= 3 and sys.argv[1] == "show"):
		if (len(sys.argv) == 4):
			device = sys.argv[3]
		show(int(sys.argv[2]))
	elif (len(sys.argv) >= 2):
		if (len(sys.argv) == 3):
			device = sys.argv[2]
		if (sys.argv[1] == "ping"):
			ping()
		elif (sys.argv[1] == "dump"):
			dump()
		elif (sys.argv[1] == "debug"):
			debugConsole()
		elif (sys.argv[1] == "reset"):
			ResetIntoBootloader()
		elif (sys.argv[1] == "start"):
			StartFromBootloader()
		elif (sys.argv[1] == "test"):
			main()
	else:
		print "available commands:"
		print "up <filename> <sector> [serialdevice ex. /dev/ttyACM0]"
		print "show <sector> [serialdevice]"
		print "dump [serialdevice]"
		print "ping [serialdevice]"
		print "debug [serialdevice]"
		print "test [serialdevice]"
		print "reset [serialdevice]"
		print "start"

