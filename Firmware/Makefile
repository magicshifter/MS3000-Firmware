#
# All paths in this Makefile require the ESP8266 toolchain be built and functional already.
#
# Git it from https://github.com/esp8266/Arduino.git
#
#                ^^ Build it .. unpack it somewhere, 
#                   find the esp8266com/esp8266 dir, 
#                   and set it to your BASEDIR as per 
#                   the examples ..
#                   
# Also needs esptool:
#
# git clone  https://github.com/themadinventor/esptool.git ../../esptool
#
# And mkspiffs:
#
# git clone https://github.com/igrr/mkspiffs.git ../mkspiffs
#
# Note: we can turn off compiling if we want to investigate this Makefile.
SHOULD_COMPILE=true

# The location of the build-directory, where all products are composed
MS3KOS_BUILD_DIR ?= /tmp/MS3KOS_BUILD

# These three basic env VARS need to be set
#ESP8266COM_BASEDIR = <location of the ESP8266com/esp8266 SDK directory>
#ESP8266COM_BIN_DIR = <location of the ESP8266 TOOLCHAIN /bin directory>
#ESP8266COM_TOOL_DIR = <location of the ESP8266 TOOLCHAIN /tool directory>
#MS3KOS_BASE_DIR = <location of the MS3KOS Project Base Directory {should be where this Makefile lives}

HOSTNAME=${shell hostname}

# assume you've followed the instructions above to clone mkspiffs into place
SPIFFS_TOOL=../mkspiffs/mkspiffs
# TODO: Tools-build stage

ifeq (${USER},wizard23)
# e.g. wizard23
ESP8266COM_BASEDIR ?= /home/wizard23/Desktop/arduino_builds/arduino-1.6.6/hardware/esp8266com/esp8266
ESPTOOL=${ESP8266COM_TOOLS_DIR}/esptool 
MS3KOS_BASE_DIR ?= /home/wizard23/Dropbox/projects2015/MagicShifter3000/git/TestFirmWare_MagicShifter3000/Firmware/MS3KOS
MS_DEVICE_PORT ?= /dev/ttyUSB0
HW_ID ?= HW_ID_MS3000_FSOX8700CQ
endif

# e.g. jascha
ifeq (${USER}@${HOSTNAME},j@scha)
ESPTOOL=${ESP8266COM_TOOLS_DIR}/esptool/esptool 
ESP8266COM_BASEDIR ?= /home/j/apps/Arduino/
MS3KOS_BASE_DIR ?= /home/j/magicshifter/ms3k/Firmware/MS3KOS
MS_DEVICE_PORT ?= /dev/ttyUSB0
HW_ID ?= HW_ID_MS3000_MMA8452Q
endif

# e.g. seclorum@w1x8-dev.local
ifeq (${USER}@${HOSTNAME},j@w1x8-dev.local)
ESP8266COM_BASEDIR ?= /Applications/Arduino/1.6.6/Arduino.app/Contents/java/hardware/esp8266com/esp8266
ESPTOOL=${ESP8266COM_TOOLS_DIR}/esptool 
MS3KOS_BASE_DIR ?= /Users/j/Documents/wizards_at_work/MagicShifter3000/Firmware/MS3KOS
MS_DEVICE_PORT ?= /dev/tty.SLAB_USBtoUART
HW_ID ?= HW_ID_RING
#HW_ID ?= HW_ID_MS3000_MMA8452Q
# HW_ID ?= HW_ID_MS3000_FSOX8700CQ
endif
# e.g. ibisum@wizard23-DevRock2
ifeq (${USER}@${HOSTNAME},ibisum@wizard23-DevRock2)
ESP8266COM_BASEDIR ?= /home/ibisum/Documents/arduino-1.6.6/hardware/esp8266com/esp8266
ESPTOOL=${ESP8266COM_TOOLS_DIR}/esptool/esptool
SPIFFS_TOOL=${ESP8266COM_TOOLS_DIR}/mkspiffs/mkspiffs
MS3KOS_BASE_DIR ?= /home/ibisum/Documents/MagicShifter3000/Firmware/MS3KOS
MS_DEVICE_PORT ?= /dev/ttyUSB0
# HW_ID ?= HW_ID_MS3000_MMA8452Q
HW_ID ?= HW_ID_RING
MS3KOS_BUILD_DIR = /home/ibisum/Documents/builds/MagicShifter3000/MS3KOS_BUILD
endif

#TODO: post-flight check the above user-provided variables


# Now we should be able to find the toolchain
ESP8266COM_BIN_DIR = ${ESP8266COM_BASEDIR}/tools/xtensa-lx106-elf/bin
ESP8266COM_TOOLS_DIR=${ESP8266COM_BASEDIR}/tools/
#TODO: pre-flight qualify the toolchain {check for version file, etc.}

# Intended outputs
MS3KOS_AR_FILE = ${MS3KOS_BUILD_DIR}/MS3KOS_core.a
MS3KOS_ELF_FILE = ${MS3KOS_BUILD_DIR}/MS3KOS.elf
MS3KOS_BIN_FILE = ${MS3KOS_BUILD_DIR}/MS3KOS.bin
MS3KOS_SPIFFS_ROOT_DIR = ${MS3KOS_BUILD_DIR}/MS3KOS.spiffs.root/
MS3KOS_SPIFFS_BIN_FILE = ${MS3KOS_BUILD_DIR}/MS3KOS.spiffs.bin
MS3KOS_SPIFFS_FILE_LIST = ../Web_Old/index.html
#../Web_Old/MagicShifter.html

# Platform Defs for the ESP8266 rig
MS3KOS_PLATFORM_DEFS = -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ \
-DF_CPU=80000000L -DARDUINO=166 \
-DARDUINO_ESP8266_ESP01 -DARDUINO_ARCH_ESP8266  -DESP8266 \
-DHW_ID=${HW_ID}

# Warning: C flags are different to CPP flags
MS3KOS_BUILD_CFLAGS = -c -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL \
-fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals \
-falign-functions=4 -MMD -std=gnu99 -ffunction-sections \
-fdata-sections

MS3KOS_BUILD_CPPFLAGS= -c -Os -g -mlongcalls -mtext-section-literals -fno-exceptions \
-fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections \
-fdata-sections 

# CPP Flags are intended to allow us to merge C/CPP components together
MS3KOS_LINK_FLAGS = -g -Os -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static \
-Teagle.flash.4m.ld -Wl,--gc-sections -Wl,-wrap,system_restart_local \
-Os -g -mlongcalls -mtext-section-literals -fno-exceptions \
-fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections \
-fdata-sections \
-Wl,-wrap,register_chipv6_phy

MS3KOS_BASELIBS = -lm -lgcc -lhal -lphy -lnet80211 \
-llwip -lwpa -lmain -lpp -lsmartconfig -lwps -lcrypto -laxtls 


# Source Include paths for the project 
MS3KOS_INCLUDES := -I${MS3KOS_BASE_DIR}/ \
-I${MS3KOS_BASE_DIR}/Libs/Base64

# Source Include paths for the ESP8266 SDK components
ESP8266COM_INCLUDES := -I${ESP8266COM_BASEDIR}/cores/esp8266 \
-I${ESP8266COM_BASEDIR}/cores/esp8266/spiffs/ \
-I${ESP8266COM_BASEDIR}/tools/sdk/include \
-I${ESP8266COM_BASEDIR}/variants/generic \
-I${ESP8266COM_BASEDIR}/libraries/Wire \
-I${ESP8266COM_BASEDIR}/libraries/ESP8266WiFi/src \
-I${ESP8266COM_BASEDIR}/libraries/ESP8266WebServer/src \
-I${ESP8266COM_BASEDIR}/libraries/ESP8266mDNS \
-I${ESP8266COM_BASEDIR}/libraries/EEPROM \
-I${ESP8266COM_BASEDIR}/libraries/SPI 

# Source code - external dependencies, C ..
EXTERNAL_DEPS_C := ${ESP8266COM_BASEDIR}/cores/esp8266/cont_util.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_eboot_command.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_flash_utils.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_i2s.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_noniso.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_phy.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_postmortem.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_si2c.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_timer.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_wiring.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_wiring_analog.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_wiring_digital.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_wiring_pulse.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_wiring_pwm.c \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_wiring_shift.c \
${ESP8266COM_BASEDIR}/cores/esp8266/libc_replacements.c \
${ESP8266COM_BASEDIR}/cores/esp8266/spiffs/spiffs_cache.c \
${ESP8266COM_BASEDIR}/cores/esp8266/spiffs/spiffs_check.c \
${ESP8266COM_BASEDIR}/cores/esp8266/spiffs/spiffs_gc.c \
${ESP8266COM_BASEDIR}/cores/esp8266/spiffs/spiffs_hydrogen.c \
${ESP8266COM_BASEDIR}/cores/esp8266/spiffs/spiffs_nucleus.c

# Source code - external dependencies, C++ ..
EXTERNAL_DEPS_CPP := ${MS3KOS_BASE_DIR}/MS3KOS.cpp \
${MS3KOS_BASE_DIR}/Libs/Base64/Base64.cpp \
${MS3KOS_BASE_DIR}/MidiShifter/miby.cpp \
${ESP8266COM_BASEDIR}/libraries/Wire/Wire.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266WiFi/src/ESP8266WiFi.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266WiFi/src/WiFiClient.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266WiFi/src/WiFiClientSecure.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266WiFi/src/WiFiServer.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266WiFi/src/WiFiUdp.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266WebServer/src/ESP8266WebServer.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266WebServer/src/Parsing.cpp \
${ESP8266COM_BASEDIR}/libraries/ESP8266mDNS/ESP8266mDNS.cpp \
${ESP8266COM_BASEDIR}/libraries/EEPROM/EEPROM.cpp \
${ESP8266COM_BASEDIR}/libraries/SPI/SPI.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/abi.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/core_esp8266_main.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/debug.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/Esp.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/FS.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/HardwareSerial.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/IPAddress.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/pgmspace.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/Print.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/spiffs_api.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/spiffs_hal.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/Stream.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/Tone.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/Updater.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/WMath.cpp \
${ESP8266COM_BASEDIR}/cores/esp8266/WString.cpp

# Source code - external dependencies, Assembly header for our core ..
EXTERNAL_DEPS_S := ${ESP8266COM_BASEDIR}/cores/esp8266/cont.S 


##
## Warning: from this point on, automation is assumed
##

# Combine the  Deps to one
EXTERNAL_DEPS := ${EXTERNAL_DEPS_C} 
EXTERNAL_DEPS += ${EXTERNAL_DEPS_CPP}
EXTERNAL_DEPS += ${EXTERNAL_DEPS_S}

EXTERNAL_DEPS_C_OBJS := ${patsubst %.c,%.c.o, ${EXTERNAL_DEPS_C}}
EXTERNAL_DEPS_CPP_OBJS := ${patsubst %.cpp,%.cpp.o, ${EXTERNAL_DEPS_CPP}}
EXTERNAL_DEPS_S_OBJS := ${patsubst %.S,%.S.o, ${EXTERNAL_DEPS_S}}

EXTERNAL_DEPS_OBJS := ${EXTERNAL_DEPS_C_OBJS}
EXTERNAL_DEPS_OBJS += ${EXTERNAL_DEPS_CPP_OBJS}
EXTERNAL_DEPS_OBJS += ${EXTERNAL_DEPS_S_OBJS}


# Now we know the OBJS list, we will also set up the temporary build-dir - 
# note that this is also hanging off MS3KOS_BUILD_DIR
MS3KOS_BUILD_DIR_TREE_LIST := ${foreach file, ${EXTERNAL_DEPS_OBJS}, ${dir ${file}}}

# ifeq (${SHOULD_COMPILE},false)
${info USER@HOSTNAME = ${USER}@${HOSTNAME} }
${info ESP8266COM_BASEDIR = ${ESP8266COM_BASEDIR} }
${info MS3KOS_BUILD_DIR = ${MS3KOS_BUILD_DIR} }
${info MS3KOS_BASE_DIR = ${MS3KOS_BASE_DIR} }
${info EXTERNAL_DEPS = ${EXTERNAL_DEPS} }
${info EXTERNAL_DEPS_C_OBJS = ${EXTERNAL_DEPS_C_OBJS} }
${info EXTERNAL_DEPS_OBJS = ${EXTERNAL_DEPS_OBJS} }
${info MS3KOS_BUILD_DIR_TREE_LIST = ${MS3KOS_BUILD_DIR_TREE_LIST} }
# endif

.PHONY:  clean 

all:		${MS3KOS_BUILD_DIR} ${MS3KOS_BIN_FILE} ${MS3KOS_SPIFFS_BIN_FILE}

# Note that these are the targets for each type of file we have in the project - .S, .c, .cpp, &etc.
# Output is computed to our BUILD_DIR before the main target.
%.c.o:	C_OUT1 = ${subst ${ESP8266COM_BASEDIR}, ${MS3KOS_BUILD_DIR}, $@}
%.c.o:	C_OUT = ${subst ${MS3KOS_BASE_DIR}, ${MS3KOS_BUILD_DIR}/ms3kos_base, ${C_OUT1}}
%.c.o:	C_IN = ${patsubst %.c.o,%.c, $@}
%.c.o:
	@echo "\nMS3KOS: Compile C:" ${C_IN} " to: " ${C_OUT}
ifeq (${SHOULD_COMPILE},true)
	${ESP8266COM_BIN_DIR}/xtensa-lx106-elf-gcc  \
	${MS3KOS_BUILD_CFLAGS} \
	${MS3KOS_PLATFORM_DEFS} \
	${ESP8266COM_INCLUDES} \
	${MS3KOS_INCLUDES} \
	-L${ESP8266COM_BASEDIR}/tools/sdk/lib \
	-L${ESP8266COM_BASEDIR}/tools/sdk/ld \
	-o ${C_OUT} ${C_IN} 
	@echo "\nMS3KOS: Add ${C_OUT} to ${MS3KOS_AR_FILE}"
	${ESP8266COM_BIN_DIR}/xtensa-lx106-elf-ar cru ${MS3KOS_AR_FILE} ${C_OUT}
endif

%.cpp.o:	CPP_OUT1 = ${subst ${ESP8266COM_BASEDIR}, ${MS3KOS_BUILD_DIR}, $@}
%.cpp.o:	CPP_OUT = ${subst ${MS3KOS_BASE_DIR}, ${MS3KOS_BUILD_DIR}/ms3kos_base, ${CPP_OUT1}}
%.cpp.o:	CPP_IN = ${patsubst %.cpp.o,%.cpp, $@}
%.cpp.o:
	@echo "\nMS3KOS: Compile CPP:" ${CPP_IN} " to: " ${CPP_OUT}
ifeq (${SHOULD_COMPILE},true)
	${ESP8266COM_BIN_DIR}/xtensa-lx106-elf-g++  \
	${MS3KOS_BUILD_CPPFLAGS} \
	${MS3KOS_PLATFORM_DEFS} \
	${ESP8266COM_INCLUDES} \
	${MS3KOS_INCLUDES} \
	-L${ESP8266COM_BASEDIR}/tools/sdk/lib \
	-L${ESP8266COM_BASEDIR}/tools/sdk/ld \
	-o ${CPP_OUT} ${CPP_IN}
	@echo "\nMS3KOS: Add ${CPP_OUT} to ${MS3KOS_AR_FILE}"
	${ESP8266COM_BIN_DIR}/xtensa-lx106-elf-ar cru ${MS3KOS_AR_FILE} ${CPP_OUT}
endif

%.S.o:	S_OUT1 = ${subst ${ESP8266COM_BASEDIR}, ${MS3KOS_BUILD_DIR}, $@}
%.S.o:	S_OUT = ${subst ${MS3KOS_BASE_DIR}, ${MS3KOS_BUILD_DIR}/ms3kos_base, ${S_OUT1}}
%.S.o:	S_IN = ${patsubst %.S.o,%.S, $@}
%.S.o:
	@echo "\nMS3KOS: Assemble S:" ${S_IN} " to: " ${S_OUT}
ifeq (${SHOULD_COMPILE},true)
	${ESP8266COM_BIN_DIR}/xtensa-lx106-elf-gcc  \
	-c -g -x assembler-with-cpp -MMD \
	${MS3KOS_PLATFORM_DEFS} \
	${ESP8266COM_INCLUDES} \
	-L${ESP8266COM_BASEDIR}/tools/sdk/lib \
	-L${ESP8266COM_BASEDIR}/tools/sdk/ld \
	${S_IN} \
	-o ${S_OUT}
	@echo "\nMS3KOS: Add ${S_OUT} to ${MS3KOS_AR_FILE}"
	${ESP8266COM_BIN_DIR}/xtensa-lx106-elf-ar cru ${MS3KOS_AR_FILE} ${S_OUT}
endif

# Produce the ELF file
${MS3KOS_ELF_FILE}: ${EXTERNAL_DEPS_OBJS}
	@echo "\nMS3KOS: Create ${MS3KOS_ELF_FILE} "
ifeq (${SHOULD_COMPILE},true)
	${ESP8266COM_BIN_DIR}/xtensa-lx106-elf-gcc  \
	${MS3KOS_LINK_FLAGS} \
	-L${ESP8266COM_BASEDIR}/tools/sdk/lib \
	-L${ESP8266COM_BASEDIR}/tools/sdk/ld \
	-o $@ \
	-Wl,--start-group "${MS3KOS_BUILD_DIR}/ms3kos_base/MS3KOS.cpp.o" \
	${MS3KOS_AR_FILE} \
	-lm -lgcc -lhal -lphy -lnet80211 \
	-llwip -lwpa -lmain -lpp -lsmartconfig -lwps -lcrypto -laxtls \
	-Wl,--end-group  \
	"-L${MS3KOS_BUILD_DIR}"
endif

# Produce a BIN file
${MS3KOS_BIN_FILE}: ${MS3KOS_ELF_FILE}
	@echo "\nMS3KOS: Create ${MS3KOS_BIN_FILE}"
ifeq (${SHOULD_COMPILE},true)
	${ESPTOOL} \
	-eo ${ESP8266COM_BASEDIR}/bootloaders/eboot/eboot.elf \
	-bo ${MS3KOS_BIN_FILE} -bm dio -bf 40 -bz 4M -bs \
	.text -bp 4096 -ec -eo ${MS3KOS_ELF_FILE} -bs \
	.irom0.text -bs .text -bs .data -bs .rodata -bc -ec
endif


# Burn the SPIFFS filesystem into the device - note the use of bc for calculating proper offset
${MS3KOS_SPIFFS_BIN_FILE}:	${MS3KOS_SPIFFS_FILE_LIST}
	@echo "\nMS3KOS: Build SPIFFS file ${MS3KOS_SPIFFS_FILE}"
ifeq (${SHOULD_COMPILE},true)
	(mkdir -p ${MS3KOS_SPIFFS_ROOT_DIR} && \
		(for spiffsMember in ${MS3KOS_SPIFFS_FILE_LIST}; do cp -rfv $$spiffsMember ${MS3KOS_SPIFFS_ROOT_DIR} ; done) && \
		${SPIFFS_TOOL} -c ${MS3KOS_SPIFFS_ROOT_DIR} -p 256 -b 8192 -s $(shell echo "ibase=16;3FB000-100000" | bc) ${MS3KOS_SPIFFS_BIN_FILE})
endif


# Build directory is set up to contain the external deps and the project local libs independently.
# Note the use of ms3kos_base/ to affect this offset.
${MS3KOS_BUILD_DIR}:	NEW_TREE1 := ${subst ${ESP8266COM_BASEDIR}, ${MS3KOS_BUILD_DIR}, ${MS3KOS_BUILD_DIR_TREE_LIST}}
${MS3KOS_BUILD_DIR}:	NEW_TREE := ${subst ${MS3KOS_BASE_DIR}, ${MS3KOS_BUILD_DIR}/ms3kos_base, ${NEW_TREE1}}
${MS3KOS_BUILD_DIR}:
ifeq (${SHOULD_COMPILE},true)
	(mkdir -p ${MS3KOS_BUILD_DIR} && cd ${MS3KOS_BUILD_DIR} && \
		(for newOutDir in ${sort ${NEW_TREE}}; do mkdir -p $$newOutDir;  done))
else
		(for newOutDir in ${sort ${NEW_TREE}}; do echo "WOULD mkdir " $$newOutDir ".. ";  done)

endif

# Flash the firmware 
flash_firmware:	${MS3KOS_BIN_FILE} ${MS3KOS_SPIFFS_BIN_FILE}
	echo "\nMS3KOS: Flash ${MS3KOS_BIN_FILE}"
	python ../../esptool/esptool.py -p ${MS_DEVICE_PORT} write_flash 0x00 ${MS3KOS_BIN_FILE}
	#${ESPTOOL} -cd ck -cb 921600 -cp ${MS_DEVICE_PORT} -ca 0x000000 -cf ${MS3KOS_BIN_FILE}

# Flash both the firmware and the filesystem
flash_all:	
	echo "\nMS3KOS: Flash ${MS3KOS_BIN_FILE}"
	python ../../esptool/esptool.py -p ${MS_DEVICE_PORT} write_flash 0x00 ${MS3KOS_BIN_FILE}
	#${ESPTOOL} -cd ck -cb 921600 -cp ${MS_DEVICE_PORT} -ca 0x000000 -cf ${MS3KOS_BIN_FILE}
	echo "\nMS3KOS: Flash SPIFFS file ${MS3KOS_SPIFFS_FILE}"
	python ../../esptool/esptool.py -p ${MS_DEVICE_PORT} write_flash 0x100000 ${MS3KOS_SPIFFS_BIN_FILE}
	#${ESPTOOL} -cd ck -cb 921600 -cp ${MS_DEVICE_PORT} -ca 0x100000 -cf ${MS3KOS_SPIFFS_BIN_FILE}

# only the filesystem
flash_fs:
	echo "\nMS3KOS: Flash SPIFFS file ${MS3KOS_SPIFFS_FILE}"
	python ../../esptool/esptool.py -p ${MS_DEVICE_PORT} write_flash 0x100000 ${MS3KOS_SPIFFS_BIN_FILE}
	#${ESPTOOL} -cd ck -cb 921600 -cp ${MS_DEVICE_PORT} -ca 0x100000 -cf ${MS3KOS_SPIFFS_BIN_FILE}


spiffs:		${MS3KOS_BUILD_DIR} ${MS3KOS_SPIFFS_BIN_FILE}
	python ../../esptool/esptool.py -p ${MS_DEVICE_PORT} -b 921600 write_flash 0x100000 ${MS3KOS_SPIFFS_BIN_FILE} -ff 40m -fm dio 

clean:
	rm -rf ${MS3KOS_BUILD_DIR} ${MS3KOS_ELF_FILE} ${MS3KOS_BIN_FILE} ${MS3KOS_SPIFFS_ROOT_DIR}

locals:
#${info USER@HOSTNAME = ${USER}@${HOSTNAME} }
${info ESP8266COM_BASEDIR = ${ESP8266COM_BASEDIR} }
${info MS3KOS_BUILD_DIR = ${MS3KOS_BUILD_DIR} }
${info MS3KOS_BASE_DIR = ${MS3KOS_BASE_DIR} }
#${info MS3KOS_BUILD_DIR_TREE_LIST = ${MS3KOS_BUILD_DIR_TREE_LIST} }
