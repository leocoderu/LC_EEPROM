#include "LC_EEPROM.h"

void LC_EXT_EEPROM::_getModelInfo(const eeprom_model_t& devModel) {
    switch (devModel) {
    case _24C01:
        _devCapacity = 1;       // 1kBit
        _twiFreq = 1;           // 100000
        _pSize = 8;             // 8bit
        break;
    case _24AA01:
        _devCapacity = 1;
        _twiFreq = 4; // 400000
        _pSize = 8;
        break;
    case _24LC01:
        _devCapacity = 1;
        _twiFreq = 4; // 400000
        _pSize = 8;
        break;
    case _24FC01:
        _devCapacity = 1;
        _twiFreq = 10; // 1000000
        _pSize = 8;
        break;
    case _24C02:
        _devCapacity = 2;
        _twiFreq = 1; // 100000
        _pSize = 8;
        break;
    case _24AA02:
        _devCapacity = 2;
        _twiFreq = 4; // 400000
        _pSize = 8;
        break;
    case _24LC02:
        _devCapacity = 2;
        _twiFreq = 4; // 400000
        _pSize = 8;
        break;
    case _24FC02:
        _devCapacity = 2;
        _twiFreq = 10; // 1000000
        _pSize = 8;
        break;
    case _24C04:
        _devCapacity = 4;
        _twiFreq = 1; // 100000
        _pSize = 16;
        break;
    case _24W04:
        _devCapacity = 4;
        _twiFreq = 1; // 100000
        _pSize = 16;
        break;
    case _24AA04:
        _devCapacity = 4;
        _twiFreq = 4; // 400000
        _pSize = 16;
        break;
    case _24LC04:
        _devCapacity = 4;
        _twiFreq = 4; // 400000
        _pSize = 16;
        break;
    case _24FC04:
        _devCapacity = 4;
        _twiFreq = 10; // 1000000
        _pSize = 16;
        break;
    case _24C08:
        _devCapacity = 8;
        _twiFreq = 10; // 1000000
        _pSize = 16;
        break;
    case _24AA08:
        _devCapacity = 8;
        _twiFreq = 4; // 400000
        _pSize = 16;
        break;
    case _24LC08:
        _devCapacity = 8;
        _twiFreq = 4; // 400000
        _pSize = 16;
        break;
    case _24FC08:
        _devCapacity = 8;
        _twiFreq = 10; // 1000000
        _pSize = 16;
        break;
    case _24C16:
        _devCapacity = 16;
        _twiFreq = 10; // 1000000
        _pSize = 16;
        break;
    case _24AA16:
        _devCapacity = 16;
        _twiFreq = 4; // 400000
        _pSize = 16;
        break;
    case _24LC16:
        _devCapacity = 16;
        _twiFreq = 4; // 400000
        _pSize = 16;
        break;
    case _24FC16:
        _devCapacity = 16;
        _twiFreq = 10; // 1000000
        _pSize = 16;
        break;
    case _24C32:
        _devCapacity = 32;
        _twiFreq = 4; // 400000
        _pSize = 8;
        break;
    case _24AA32:
        _devCapacity = 32;
        _twiFreq = 4; // 400000
        _pSize = 8;
        break;
    case _24LC32:
        _devCapacity = 32;
        _twiFreq = 4; // 400000
        _pSize = 32;
        break;
    case _24FC32:
        _devCapacity = 32;
        _twiFreq = 10; // 1000000
        _pSize = 8;
        break;
    case _24AA64:
        _devCapacity = 64;
        _twiFreq = 4; // 400000
        _pSize = 32;
        break;
    case _24LC64:
        _devCapacity = 64;
        _twiFreq = 4; // 400000
        _pSize = 32;
        break;
    case _24FC64:
        _devCapacity = 64;
        _twiFreq = 10; // 1000000
        _pSize = 32;
        break;
    case _24AA128:
        _devCapacity = 128;
        _twiFreq = 4; // 400000
        _pSize = 64;
        break;
    case _24LC128:
        _devCapacity = 128;
        _twiFreq = 4; // 400000
        _pSize = 64;
        break;
    case _24FC128:
        _devCapacity = 128;
        _twiFreq = 10; // 1000000
        _pSize = 64;
        break;
    case _24AA256:
        _devCapacity = 256;
        _twiFreq = 4; // 400000
        _pSize = 64;
        break;
    case _24LC256:
        _devCapacity = 256;
        _twiFreq = 4; // 400000
        _pSize = 64;
        break;
    case _24FC256:
        _devCapacity = 256;
        _twiFreq = 10; // 1000000
        _pSize = 64;
        break;
    case _24AA512:
        _devCapacity = 512;
        _twiFreq = 4; // 400000
        _pSize = 128;
        break;
    case _24LC512:
        _devCapacity = 512;
        _twiFreq = 4; // 400000
        _pSize = 128;
        break;
    case _24FC512:
        _devCapacity = 512;
        _twiFreq = 10; // 1000000
        _pSize = 128;
        break;
    case _24AA1025:
        _devCapacity = 1024;
        _twiFreq = 4; // 400000
        _pSize = 128;
        break;
    case _24LC1025:
        _devCapacity = 1024;
        _twiFreq = 4; // 400000
        _pSize = 128;
        break;
    case _24FC1025:
        _devCapacity = 1024;
        _twiFreq = 10; // 1000000
        _pSize = 128;
        break;
    default:
        break;
    }
}