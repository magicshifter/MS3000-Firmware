SHELL := /bin/bash

MS3000_PORT?=`pio device list --json-output | python -c 'exec("""\nimport json,sys\n\nobj=json.load(sys.stdin)\nfor y in obj:\n if "10C4:EA60" in y["hwid"].upper():\n  print y["port"]\n""")'`
MS3000_PORT?=/dev/ttyUSB0

all:	clean test

setup:
	platformio run

run:
	platformio run --upload-port ${MS3000_PORT}

test:
	platformio run --target upload --upload-port ${MS3000_PORT}

viaserial:
	platformio run --target upload --upload-port ${MS3000_PORT} 

monitor:
	pio device monitor -b 921600 --dtr 0 --rts 0 -p ${MS3000_PORT}

reset-device:
	-@echo "^T^R" | pio device monitor --quiet -b 921600 --dtr 0 --rts 1 -p ${MS3000_PORT} 
	@echo "(Inappropriate ioctl is actually appropriate for a reset.. everything is okay!)"


clean:
	# !J! Todo: BUILD_PRODUCT_DIR
	rm -rf .pioenvs
