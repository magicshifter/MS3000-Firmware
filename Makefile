SHELL := /bin/bash

all:
	make -C MS3KOS/

piolibs:
	pio lib --global install WifiManager AppleMidi OSC

