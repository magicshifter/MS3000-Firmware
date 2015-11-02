#!/usr/bin/env python

import serial
import curses
import time

portName = "/dev/ttyUSB1"
baudRate = 57600 * 2
#baudRate = 76800 # 38400 # bootloader baudreate

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

def main(stdscr):
    port = tryOpenSerial()
    # resetESP(port)

    # reset 
    port.setDTR(False)
    port.setRTS(True)
    port.setRTS(False)

    canChangeColor = curses.can_change_color()
    #curses.noecho()
    #curses.cbreak()

    # do not wait for input when calling getch
    stdscr.nodelay(1)

    # stdscr.keypad(1)

    begin_x = 20; begin_y = 40
    height = 40; width = 80
    keyboardWin = curses.newwin(height, width, begin_y, begin_x)
    keyboardWin.clear()
    curses.init_pair(1, curses.COLOR_WHITE, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_RED, curses.COLOR_BLACK)
    keyboardWin.bkgd(' ', curses.color_pair(1))



    xx = 0
    refresh = False
    while True:
        if port:
            s = port.read(1)
            if (len(s) > 0):
                code = ord(s)
                if (xx == 0):
                    stdscr.clear()
                xx = (xx + 1) % 900
                if code > 13 and code <= 255:
                    try:
                        stdscr.addstr(s)
                    except:
                        pass
                elif code == 13:
                    stdscr.addstr("\n\r")
                else:
                    stdscr.addstr('?')
                stdscr.refresh()

        # get keyboard input, returns -1 if none available
        c = stdscr.getch()
        if c != -1:
            keyboardWin.addstr(chr(c))
            if c == 65:
                keyboardWin.addstr("\ntry open serial: ")
                port = tryOpenSerial()
                resetESP(port)
                if port:
                    keyboardWin.addstr("success\n", curses.color_pair(2))
                else:
                    keyboardWin.addstr("failed\n", curses.color_pair(3))
            if c == 114: # r
                keyboardWin.addstr("RTS <- True")
                port.setRTS(True)
            if c == 82: # R
                 keyboardWin.addstr("RTS <- False")
                 port.setRTS(False)
            if c == 100: # d
                keyboardWin.addstr("DTR <- True")
                port.setDTR(True)
            if c == 68: # D
                 keyboardWin.addstr("DTR <- False")
                 port.setDTR(False)
            keyboardWin.refresh()

if __name__ == '__main__':
    curses.wrapper(main)
