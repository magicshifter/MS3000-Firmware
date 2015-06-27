import serial
import curses
import time


def main(stdscr):

    #curses.noecho()
    #curses.cbreak()

    # do not wait for input when calling getch
    stdscr.nodelay(1)

    # stdscr.keypad(1)

    begin_x = 20; begin_y = 7
    height = 20; width = 60
    win = curses.newwin(height, width, begin_y, begin_x)
    win.clear()

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
                win.clear()
            xx = (xx + 1) % 250
            win.addstr(str(ord(s)))
            win.addstr("/")
            win.refresh()

        # get keyboard input, returns -1 if none available
        c = stdscr.getch()
        if c != -1:
            stdscr.clear()
            stdscr.addstr("key: ")
            stdscr.addstr(str(c))
            refresh = True;
            stdscr.addstr(" ")
            if c == 114: # r
                stdscr.addstr("RTS <- True")
                port.setRTS(True)
            if c == 82: # R
                 stdscr.addstr("RTS <- False")
                 port.setRTS(False)
            if c == 100: # d
                stdscr.addstr("DTR <- True")
                port.setDTR(True)
            if c == 68: # D
                 stdscr.addstr("DTR <- False")
                 port.setDTR(False)
            stdscr.refresh()
            stdscr.move(0, 0)

if __name__ == '__main__':
    curses.wrapper(main)
