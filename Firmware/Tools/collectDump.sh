dumpname=firmware-0x0000.bin
buildname=./build/WiFiMagicShifterOS.cpp.bin
targetdir=./messedUp/$1

mkdir $targetdir
cp $dumpname $targetdir
cp $buildname $targetdir
