MS3000PORT?=/dev/ttyUSB0

run:
	platformio run 

test:
	platformio run --target upload

viaserial:
	platformio run --target upload --upload-port ${MS3000PORT} 

setup:
	platformio run

monitor:
	pio serialports monitor -b 921600 --dtr 0 --rts 0
