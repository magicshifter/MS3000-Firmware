import serial
import curses
import time


def main(stdscr):

    #curses.noecho()
    #curses.cbreak()

    # do not wait for input when calling getch
    stdscr.nodelay(1)

    # stdscr.keypad(1)

    begin_x = 20; begin_y = 40
    height = 40; width = 80
    keyboardWin = curses.newwin(height, width, begin_y, begin_x)
    keyboardWin.clear()

    port = serial.Serial("/dev/ttyUSB0", 115200, timeout=0)
    port.setDTR(False)
    port.setRTS(True)
    time.sleep(0.05)
    port.setRTS(False)

    xx = 0
    refresh = False
    while True:
        s = port.read(1)
        if (len(s) > 0):
            if (xx == 0):
                stdscr.clear()
            xx = (xx + 1) % 2550
            if ord(s) >= 32 and ord(s) <= 127:
                stdscr.addstr(s)
            else:
                stdscr.addstr('?')
            stdscr.refresh()

        # get keyboard input, returns -1 if none available
        c = stdscr.getch()
        if c != -1:
            keyboardWin.addstr(chr(c))
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
