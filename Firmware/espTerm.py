import serial
import curses

def main(stdscr):
    # do not wait for input when calling getch
    stdscr.nodelay(1)


    port = serial.Serial("/dev/ttyUSB0", 115200, timeout=0)

    refresh = False
    while True:
        s = port.read(1)
        if (len(s) > 0):
            stdscr.clear()
            stdscr.addstr("serial: ")
            stdscr.addstr(str(ord(s)))
            stdscr.addstr(" : ")
            refresh = True

        # get keyboard input, returns -1 if none available
        c = stdscr.getch()
        if c != -1:
            # print numeric value
            stdscr.clear()
            stdscr.addstr("keyboard: ")
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


        if refresh:
            stdscr.refresh()
            # return curser to start position
            stdscr.move(0, 0)

if __name__ == '__main__':
    curses.wrapper(main)
