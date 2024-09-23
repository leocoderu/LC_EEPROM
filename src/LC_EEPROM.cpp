#include "LC_EEPROM.h"

//using namespace LC_EEPROM;

LC_EEPROM::LC_EEPROM(){}

// ---------------------------------------------- Internal EEPROM -----------------------------------------------------
// Returns value of byte by address
uint8_t LC_EEPROM::intReadByte(const uint32_t& addr) {
    if ((addr >= 0) && (addr < EEPROM.length())) return EEPROM.read(addr);
    return 0xFF;
}

// Returns Integer value (2 bytes) by address
uint16_t LC_EEPROM::intReadInt(const uint32_t& addr) {
    if ((addr >= 0) && ((addr + 1) < EEPROM.length())) return (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
    return 0xFFFF;
}

// Returns Longint value (4 bytes) by address
uint32_t LC_EEPROM::intReadLong(const uint32_t& addr) {
    if ((addr >= 0) && ((addr + 3) < EEPROM.length())) {
        //Serial.print("Read 0x"); Serial.print(addr + 0, HEX); Serial.print(" - "); Serial.print(EEPROM.read(addr + 0), HEX); Serial.print(" - "); Serial.println(((uint32_t)EEPROM.read(addr + 0) << 24), HEX);
        //Serial.print("Read 0x"); Serial.print(addr + 1, HEX); Serial.print(" - "); Serial.print(EEPROM.read(addr + 1), HEX); Serial.print(" - "); Serial.println(((uint32_t)EEPROM.read(addr+1) << 16), HEX);
        //Serial.print("Read 0x"); Serial.print(addr + 2, HEX); Serial.print(" - "); Serial.print(EEPROM.read(addr + 2), HEX); Serial.print(" - "); Serial.println(((uint32_t)EEPROM.read(addr+2) << 8),  HEX);
        //Serial.print("Read 0x"); Serial.print(addr + 3, HEX); Serial.print(" - "); Serial.print(EEPROM.read(addr + 3), HEX); Serial.print(" - "); Serial.println((uint32_t)EEPROM.read(addr+3), HEX);
        return ((uint32_t)EEPROM.read(addr) << 24)
            | ((uint32_t)EEPROM.read(addr + 1) << 16)
            | ((uint32_t)EEPROM.read(addr + 2) << 8)
            | (uint32_t)EEPROM.read(addr + 3);
    }
    return 0xFFFFFFFF;
}

// Returns String value by address
String LC_EEPROM::intReadStr(const uint32_t& addr, const uint16_t& quan) {
    String res = "";
    if ((addr >= 0) && ((addr + quan) < EEPROM.length()))
        for (uint16_t i = 0; i < quan; i++)
            res += char(EEPROM.read(addr + i));
    return res;
}

// Returns Block data by address with unsigned byte array, like byte array
uint8_t LC_EEPROM::intReadBlock(const uint32_t& addr, uint8_t* dst, const uint8_t& szDst) {
    if ((addr + szDst) > EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst);                     
    for (uint8_t i = 0; i < szDst; i++) {
        //uint8_t bt = intReadByte(addr + i);
        //dst[i] = bt;
        dst[i] = intReadByte(addr + i);
    }
    return 0;
}

// Returns Block data by address with signed byte array, like char array
//uint8_t LC_EEPROM::intReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst) {
uint8_t LC_EEPROM::intReadBlock(const uint32_t& addr, char* dst, const uint8_t& szDst) {
    if ((addr + szDst) > EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst);                     
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = intReadByte(addr + i);
        if (bt == 0x00) break;
        dst[i] = bt;
    }
    return 0;
}

// Write byte by address
uint8_t LC_EEPROM::intWriteByte(const uint32_t& addr, const uint8_t& wByte) {    
    if ((addr >= 0) && (addr < EEPROM.length())) { 
        EEPROM.update(addr, wByte); 
        if (intReadByte(addr) != wByte) return 2;
        return 0;
    }
    return 1;
}

// Write Integer value by address
uint8_t LC_EEPROM::intWriteInt(const uint32_t& addr, const uint16_t& wInt) {
    if ((addr >= 0) && ((addr + 1) < EEPROM.length())) {
        uint8_t tByte = (wInt >> 8); EEPROM.update(addr, tByte);
        tByte = ((wInt << 8) >> 8); EEPROM.update(addr + 1, tByte);
        if (intReadInt(addr) != wInt) return 2;
        return 0;
    }
    return 1;
}

// Write Longint value by address
uint8_t LC_EEPROM::intWriteLong(const uint32_t& addr, const uint32_t& wLong){
    if ((addr >= 0) && ((addr + 3) < EEPROM.length())) {
        uint8_t tByte = (uint8_t)(wLong >> 24); EEPROM.update(addr, tByte);
        //Serial.print("Write 0x"); Serial.print(addr + 0, HEX); Serial.print(" - "); Serial.println(tByte, HEX);
        tByte = (uint8_t)((wLong << 8)  >> 24); EEPROM.update(addr + 1, tByte);
        //Serial.print("Write 0x"); Serial.print(addr + 1, HEX); Serial.print(" - "); Serial.println(tByte, HEX);
        tByte = (uint8_t)((wLong << 16) >> 24); EEPROM.update(addr + 2, tByte);
        //Serial.print("Write 0x"); Serial.print(addr + 2, HEX); Serial.print(" - "); Serial.println(tByte, HEX);
        tByte = (uint8_t)((wLong << 24) >> 24); EEPROM.update(addr + 3, tByte);
        //Serial.print("Write 0x"); Serial.print(addr + 3, HEX); Serial.print(" - "); Serial.println(tByte, HEX);
        if (intReadLong(addr) != wLong) return 2;
        return 0;
    }
    return 1;
}

// Write string by address
uint8_t LC_EEPROM::intWriteStr(const uint32_t& addr, const String& sendStr){
    if ((addr >= 0) && ((addr + sendStr.length()) < EEPROM.length())) {
        for (uint16_t i = 0; i < sendStr.length(); i++) EEPROM.update(addr + i, sendStr[i]);
        if (intReadStr(addr, sendStr.length()) != sendStr) return 2;
        return 0;
    }
    return 1;
}

// Fill block memory by default value
uint8_t LC_EEPROM::intFillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt) {
    if ((addr >= 0) && ((addr + cnt) < EEPROM.length())) {
        for (uint32_t i = 0; i < cnt; i++) intWriteByte(addr + i, bt);
        //if (intReadStr(addr, sendStr.length()) != sendStr) return 2; TODO: Check complete operation
        return 0;
    }
    return 1;
}

// Write Block with BYTE data by address
uint8_t LC_EEPROM::intWriteBlock(const uint32_t& addr, const uint8_t* src, const uint8_t& szSrc) {
    if ((addr >= 0) && ((addr + szSrc) < EEPROM.length())) {
        intFillBlock(addr, szSrc, 0xFF);
        
        for (uint8_t i = 0; i < szSrc; i++) intWriteByte(addr + i, src[i]);        
        
        uint8_t buff[szSrc];
        intReadBlock(addr, buff, sizeof(buff));        
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

        return 0;
    }
    return 1;
}

//void printB(char* src, const uint8_t& sz) {
//    Serial.print("[");
//    for (uint8_t n = 0; n < sz; n++) Serial.print(src[n]);
//    Serial.println("]");
//}

// Write Block with CHAR data by address
uint8_t LC_EEPROM::intWriteBlock(const uint32_t& addr, const char* src, const uint8_t& szSrc) {
    if ((addr >= 0) && ((addr + szSrc) < EEPROM.length())) {
        intFillBlock(addr, szSrc, 0x00);
        for (uint8_t i = 0; i < szSrc; i++) {
            if (src[i] == 0x00) break;
            intWriteByte(addr + i, src[i]);
        }
        char buff[szSrc];
        intReadBlock(addr, buff, sizeof(buff));
        //printB(buff, sizeof(buff));
        //printB(src, szSrc);
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

        //if (strcmp(buff, src) != 0) return 2;
        return 0;
    }
    return 1;
}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EEPROM::intShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan) {
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

LC_EEPROM::~LC_EEPROM() {}

// ------------------------------------------ PROTECTED FUNCTIONS ------------------------------------------------------
String LC_EEPROM::_preFix(String str, uint8_t quan, char chr = '0') {
    String result = str;
    for (uint8_t i = 0; i < quan - str.length(); i++) result = chr + result;
    return result;
}

// ------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------
//bool LC_EEPROM::_cmpBuffers(const char* src, const uint8_t& szSrc, const char* dst, const uint8_t& szDst) {
//    if (szSrc != szDst) return false;
//    for (uint8_t i = 0; i < szSrc; i++)
//        if (src[i] != dst[i]) return false;
//    return true;
//}