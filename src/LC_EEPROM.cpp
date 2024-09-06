#include "LC_EEPROM.h"

//using namespace LC_EEPROM;

LC_EEPROM::LC_EEPROM(){}

// ---------------------------------------------- Internal EEPROM -----------------------------------------------------
uint8_t LC_EEPROM::intReadByte(const uint32_t& addr) {
    if ((addr >= 0) && (addr < EEPROM.length())) return EEPROM.read(addr);
    return 0xFF;
}

void LC_EEPROM::intWriteByte(const uint32_t& addr, const uint8_t& wByte) {
    if ((addr >= 0) && (addr < EEPROM.length())) EEPROM.update(addr, wByte);
}

uint16_t LC_EEPROM::intReadInt(const uint32_t& addr) {
    if ((addr >= 0) && ((addr + 1) < EEPROM.length())) return (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
    return 0xFFFF;
}

void LC_EEPROM::intWriteInt(const uint32_t& addr, const uint16_t& wInt) {
    if ((addr >= 0) && ((addr + 1) < EEPROM.length())) {
        uint8_t tByte = (wInt >> 8); EEPROM.update(addr, tByte);
        tByte = ((wInt << 8) >> 8); EEPROM.update(addr + 1, tByte);
    }
}

uint32_t LC_EEPROM::intReadLong(const uint32_t& addr) {
    if ((addr >= 0) && ((addr + 3) < EEPROM.length())) return ((uint32_t)EEPROM.read(addr) << 24) | ((uint32_t)EEPROM.read(addr + 1) << 16) | (EEPROM.read(addr + 2) << 8) | EEPROM.read(addr + 3);
    return 0xFFFFFFFF;
}

void LC_EEPROM::intWriteLong(const uint32_t& addr, const uint32_t& wDouble) {
    if ((addr >= 0) && ((addr + 3) < EEPROM.length())) {
        uint8_t tByte = (wDouble >> 24); EEPROM.update(addr, tByte);
        tByte = (uint8_t)((wDouble << 8) >> 24); EEPROM.update(addr + 1, tByte);
        tByte = (uint8_t)((wDouble << 16) >> 24); EEPROM.update(addr + 2, tByte);
        tByte = (uint8_t)((wDouble << 24) >> 24); EEPROM.update(addr + 3, tByte);
    }
}

String LC_EEPROM::intReadStr(const uint32_t& addr, const uint16_t& quan) {
    String res = "";
    if ((addr >= 0) && ((addr + quan) < EEPROM.length()))
        for (uint16_t i = 0; i < quan; i++)
            res += char(EEPROM.read(addr + i));
    return res;
}

void LC_EEPROM::intWriteStr(const uint32_t& addr, const String& sendStr)
{
    if ((addr >= 0) && ((addr + sendStr.length()) < EEPROM.length()))
        for (uint16_t i = 0; i < sendStr.length(); i++)
            EEPROM.update(addr + i, sendStr[i]);
}

void LC_EEPROM::intFillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt) {
    for (uint32_t i = 0; i < cnt; i++) intWriteByte(addr + i, bt);
}

void LC_EEPROM::intReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst) {
    memset(dst, defVal, szDst);                 
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = intReadByte(addr + i);
        if (bt != 0xFF) dst[i] = bt;  
        else break;
    }
}
void LC_EEPROM::intWriteBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc) {
    intFillBlock(addr, szSrc, defVal);
    for (uint8_t i = 0; i < szSrc; i++)
        if (((defVal == 0xFF) && (src[i] == INT8_MIN)) || ((defVal == 0x00) && (src[i] == 0x00))) break;
        else intWriteByte(addr + i, src[i]);
}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EEPROM::intShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan = 32) {
    Serial.println("Internal EEPROM memory...");

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
            String tmpStr = LC_EEPROM::_preFix(String(LC_EEPROM::intReadByte((j * quan) + i), HEX), 2, '0') + ' ';
            tmpStr.toUpperCase();
            Serial.print(tmpStr);
        }
        Serial.println();
    }
}

String LC_EEPROM::_preFix(String str, uint8_t quan, char chr = '0') {
    String result = str;
    for (uint8_t i = 0; i < quan - str.length(); i++) result = chr + result;
    return result;
}

LC_EEPROM::~LC_EEPROM(){}