List of hackish changes to make in the platformio installed esp stuff

--------------------------
CHANGE1:
~/.platformio/packages/framework-arduinoespressif/cores/esp8266/Arduino.h in line 250:
--------------------------

// HACK wizard23
#undef min
#undef max
#undef _min
#undef _max
// END HACK

// #ifndef _GLIBCXX_VECTOR
// // arduino is not compatible with std::vector
// #define min(a,b) ((a)<(b)?(a):(b))
// #define max(a,b) ((a)>(b)?(a):(b))
// #endif

#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))

--------------------------
CHANGE2:
~/.platformio/packages/framework-arduinoespressif/cores/esp8266/spiffs_api.cpp in line 66:
--------------------------

DirImplPtr SPIFFSImpl::openDir(const char* path)
{
//    if (!isSpiffsFilenameValid(path)) {
//        DEBUGV("SPIFFSImpl::openDir: invalid path=`%s` \r\n", path);
//        return DirImplPtr();
//    }
    spiffs_DIR dir;
    spiffs_DIR* result = SPIFFS_opendir(&_fs, path, &dir);
    if (!result) {
        DEBUGV("SPIFFSImpl::openDir: path=`%s` err=%d\r\n", path, _fs.err_code);
        return DirImplPtr();
    }
    return std::make_shared<SPIFFSDirImpl>(path, this, dir);
}

