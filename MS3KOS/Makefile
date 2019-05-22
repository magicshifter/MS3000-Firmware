SHELL := /bin/bash

include ../Tools/nanopb/extra/nanopb.mk

MS3000_PORT?=`pio serialports list --json-output | python -c 'exec("""\nimport json,sys\n\nobj=json.load(sys.stdin)\nfor y in obj:\n if "10C4:EA60" in y["hwid"].upper():\n  print (y["port"])\n""")'`
MS3000_PORT?=/dev/ttyUSB0
MS3000_PORT=/dev/tty.SLAB_USBtoUART

all:	clean proto build 

CFLAGS += -I$(NANOPB_DIR) -DPB_FIELD_16BIT

CSRC += $(NANOPB_DIR)/pb_encode.c
CSRC += $(NANOPB_DIR)/pb_decode.c
CSRC += $(NANOPB_DIR)/pb_common.c

PROTOC_OPTS += -I../Tools/nanopb/generator/proto

ESPOTA = ../Tools/ESP8266_ota/espota.py

pio-deps:
	@echo "NOTE: installing platform dependencies with PlatformIO"
	pio lib install 222 64 423 35 568 170 567 4271

proto:  ../../MS3000.proto 
	@echo "NANOPB_DIR = $(NANOPB_DIR)"
	@echo "CFLAGS = $(CFLAGS)"
# protoc --proto_path=src --js_out=library=whizz/ponycopter,binary:build/gen src/foo.proto src/bar/baz.proto
	@mkdir -p ./ms3000.proto.out
	$(PROTOC) $(PROTOC_OPTS) --proto_path ../../ ../../MS3000.proto  -o ../../MS3000.pb --js_out=./ms3000.proto.out --nanopb_out=-v:.
	python ../Tools/nanopb/generator/nanopb_generator.py ../../MS3000.pb
	#$(PROTOC) $(PROTOC_OPTS) --nanopb_out=src/ --proto_path ../../ ../../MS3000.proto  -o../../MS3000.pb 
	cp ../../MS3000.pb.? src/
	cp src/MS3000.pb.? .

build:
	platformio run --upload-port ${MS3000_PORT}

burn:
	platformio run --target upload --upload-port ${MS3000_PORT}

monitor:
	pio serialports monitor -b 921600 --dtr 0 --rts 0 -p ${MS3000_PORT}

ota.mode:
	@echo "set ota mode"
	curl http://proto-jv.local/mode?m=7

ota: .pioenvs/ms3000/firmware.bin
	@echo "ota:"
	python ${ESPOTA} -i proto-jv.local -p 8266 -f .pioenvs/ms3000/firmware.bin -d -r

reset-device:
	-@echo "^T^R" | pio serialports monitor --quiet -b 921600 --dtr 0 --rts 1 -p ${MS3000_PORT} 
	@echo "(Inappropriate ioctl is actually appropriate for a reset.. everything is okay!)"

pio-deps-clean:
	rm -rf .piolibdeps/

test:    proto build burn monitor
    

clean:
	# !J! Todo: BUILD_PRODUCT_DIR
	#rm -rf .pioenvs
	pio run -t clean
	rm -rf src/MS3000.pb.c src/MS3000.pb.h
