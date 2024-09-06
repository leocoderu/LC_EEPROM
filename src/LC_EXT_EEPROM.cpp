#include "LC_EEPROM.h"

//using namespace LC_EXT_EEPROM;

LC_EXT_EEPROM::LC_EXT_EEPROM(const eeprom_size_t & devCapacity, const uint8_t & qDevice, const uint16_t & pSize, const uint8_t & eeAddr = 0x50) {
    _devCapacity = devCapacity;
    _eepromAddr = eeAddr;
    _qDevice = qDevice;
    _pSize = pSize;
    _totalCapacity = _qDevice * _devCapacity * 1024UL / 8;
    _nAddrBytes = _devCapacity > kbits_16 ? 2 : 1;              // two address bytes needed for eeproms > 16kbits

    // determine the bitshift needed to isolate the chip select bits from
    // the address to put into the control byte
    uint16_t kb = _devCapacity;
    if (kb <= kbits_16) _csShift = 8;
    else if (kb >= kbits_512) _csShift = 16;
    else {
        kb >>= 6;
        _csShift = 12;
        while (kb >= 1) {
            ++_csShift;
            kb >>= 1;
        }
    }
}

uint8_t LC_EXT_EEPROM::begin(twiFreq_t twiFreq) {
    Wire.begin();
    Wire.setClock(twiFreq);
    Wire.beginTransmission(_eepromAddr);
    if (_nAddrBytes == 2) Wire.write(0);    // high addr byte
    Wire.write(0);                          // low addr byte
    return Wire.endTransmission();
}

uint8_t LC_EXT_EEPROM::extReadByte(const uint32_t& addr) {
    LC_EXT_EEPROM::_writeAddr(addr);
    Wire.requestFrom((uint8_t)_eepromAddr, (uint8_t)0x01);
    return (Wire.available()) ? Wire.read() : 0;
}

void LC_EXT_EEPROM::extWriteByte(const uint32_t& addr, const uint8_t& wByte) {
    if (extReadByte(addr) != wByte) _writeAddr(addr, wByte);
}

uint16_t LC_EXT_EEPROM::extReadInt(const uint32_t& addr) {
    return extReadByte(addr) << 8 | extReadByte(addr + 1);
}

void LC_EXT_EEPROM::extWriteInt(const uint32_t& addr, const uint16_t& wInt) {
    extWriteByte(addr, wInt >> 8);
    extWriteByte(addr + 1, (uint8_t)wInt);
}

uint32_t LC_EXT_EEPROM::extReadLong(const uint32_t& addr) {
    return ((uint32_t)extReadByte(addr) << 24) | ((uint32_t)extReadByte(addr + 1) << 16) | (extReadByte(addr + 2) << 8) | extReadByte(addr + 3);
}

void LC_EXT_EEPROM::extWriteLong(const uint32_t& addr, const uint32_t& wDouble) {
    uint32_t tByte = wDouble;
    for (int8_t i = sizeof(wDouble) - 1; i >= 0; i--) {
        extWriteByte(addr + i, (uint8_t)tByte);
        tByte = tByte >> 8;
    }
}

String LC_EXT_EEPROM::extReadStr(const uint32_t& addr, const uint16_t& quan) {
    String res = "";
   
    for (uint16_t i = 0; i < quan; i++)
        res += char(extReadByte(addr + i));
    return res;
}

void LC_EXT_EEPROM::extWriteStr(const uint32_t& addr, const String& sendStr)
{
    for (uint16_t i = 0; i < sendStr.length(); i++)
        extWriteByte(addr + i, sendStr[i]);
}

void LC_EXT_EEPROM::extFillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt) {
    for (uint32_t i = 0; i < cnt; i++) extWriteByte(addr + i, bt);
}

void LC_EXT_EEPROM::extReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst) {
    memset(dst, defVal, szDst);                 
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = extReadByte(addr + i);
        if (bt != 0xFF) dst[i] = bt;  
        else break;
    }
}

void LC_EXT_EEPROM::extWriteBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc) {
    extFillBlock(addr, szSrc, defVal);
    for (uint8_t i = 0; i < szSrc; i++)
        if (((defVal == 0xFF) && (src[i] == INT8_MIN)) || ((defVal == 0x00) && (src[i] == 0x00))) break;
        else extWriteByte(addr + i, src[i]);
}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EXT_EEPROM::extShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan) {
    Serial.println("External EEPROM memory...");

    // Header of table
    Serial.print(F("\n     "));
    for (uint8_t i = 0; i < quan; i++) {
        String tmpStr = LC_EEPROM::_preFix(String(i, HEX), 2, '0') + ' ';
        tmpStr.toUpperCase();
        Serial.print(tmpStr);
    }
    Serial.println();
    for (uint16_t j = (addrFrom / quan); j < (addrTo / quan + 1); j++) {
        String tSt = LC_EEPROM::_preFix(String(j * quan, HEX), 4, '0') + ' ';
        tSt.toUpperCase();
        Serial.print(tSt);
        for (uint8_t i = 0; i < quan; i++) {
            String tmpStr = LC_EEPROM::_preFix(String(LC_EXT_EEPROM::extReadByte((j * quan) + i), HEX), 2, '0') + ' ';
            tmpStr.toUpperCase();
            Serial.print(tmpStr);
        }
        Serial.println();
    }
}

void LC_EXT_EEPROM::_writeAddr(const uint32_t& addr, const uint8_t wByte = NULL) {
    Wire.beginTransmission(_eepromAddr);

    if (_nAddrBytes == 2) Wire.write(static_cast<uint8_t>(addr >> 8));    // high addr byte
    Wire.write(static_cast<uint8_t>(addr));                               // low addr byte

    if (wByte != NULL)  Wire.write(wByte);
    Wire.endTransmission();
    if (wByte != NULL) delayMicroseconds(500);
}

LC_EXT_EEPROM::~LC_EXT_EEPROM(){}