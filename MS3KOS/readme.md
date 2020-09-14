#Magicshifter OS with platformio

###How to build?
 * [install platformio](http://platformio.org/#!/get-started) (most tricky part)
 * cd to this directory
 * `platformio run` ( compile/dry run )
 * `platformio run --target upload` (upload to MS, hold ButtonA while flashing)

## Dependencies
 * remember to read the Makefile
 * `make pio-deps` can get you started...


## Example set up for Ubuntu:

	* we get ubuntu ready:
		`apt install protobuf-compiler python-protobuf python3-pip git build-essential`

	* we install some python tools - platformio and protocol buffers:
		`pip3 install platformio python3-protobuf`

	* clone the repo:
		`git clone https://github.com/magicshifter/MS3000.git`
		`cd MS3000 && git submodule update --init --recursive`

	* set up nanopb:
		`cd MS3000-Firmware/Tools/nanopb/generator/proto && make && cd -`

	* build the MS3KOS:
		`cd MS3000-Firmware/MS3KOS/ && make`

	

