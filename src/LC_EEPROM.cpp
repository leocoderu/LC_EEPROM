#include "LC_EEPROM.h"

//using namespace LC_EEPROM;

LC_EEPROM::LC_EEPROM(){}

uint32_t LC_EEPROM::intTotalCapacity() { return EEPROM.length(); }

void LC_EEPROM::info() {
    Serial.print("Internal memory EEPROM range: [ 0x0000 - 0x"); Serial.print(_preFix(String(EEPROM.length() - 1, HEX), 4, '0')); Serial.println(" ]"); 
}

// -------------------------------------------- PUBLIC FUNCTIONS -------------------------------------------------------
// Returns value of byte by address
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint8_t& dst) {
    if (addr >= EEPROM.length()) return 1;
    dst = EEPROM.read(addr);
    return 0;
}

// Returns Integer value (2 bytes) by address
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint16_t& dst) {
    if ((addr + 1) >= EEPROM.length()) return 1;
    dst = (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
    return 0;
}

// Returns Longint value (4 bytes) by address
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint32_t& dst) {
    if ((addr + 3) >= EEPROM.length()) return 1;
    dst = ((uint32_t)EEPROM.read(addr) << 24) | ((uint32_t)EEPROM.read(addr + 1) << 16) | ((uint32_t)EEPROM.read(addr + 2) << 8) | (uint32_t)EEPROM.read(addr + 3);
    return 0;
}

// Returns String value by address
uint8_t LC_EEPROM::intRead(const uint32_t& addr, String& dst, const uint8_t& szDst) {    
    if ((addr + szDst - 1) >= EEPROM.length()) return 1;

    dst = "";
    for (uint8_t i = 0; i < szDst; i++)
        dst += char(EEPROM.read(addr + i));
    return 0;
}

// Returns Block data by address with unsigned byte array, like byte array
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint8_t* dst, const uint8_t& szDst) {
    if ((addr + szDst - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst);                     
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t v = 0;
        if (intRead(addr + i, v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Returns Block data by address with signed byte array
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int8_t* dst, const uint8_t& szDst) {
    if ((addr + szDst - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst);
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t v = 0;
        if (intRead(addr + i, v) != 0) return 2;
        dst[i] = (v - 128);
    }
    return 0;
}

// Returns Block data by address with char array
uint8_t LC_EEPROM::intRead(const uint32_t& addr, char* dst, const uint8_t& szDst) {
    if ((addr + szDst - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst);                     
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t v = 0;
        if (intRead(addr + i, v) != 0) return 2;
        //if (bt == 0x00) break;
        //dst[i] = (char)(v - 128);
        dst[i] = (v - 128);
    }
    return 0;
}

// ---------------------------------------------  Write Functions  ----------------------------------------------------

// Write byte by address
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint8_t& src) {    
    if (addr >= EEPROM.length()) return 1;
    EEPROM.update(addr, src); 
    uint8_t v = 0;
    if (intRead(addr, v) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Write Integer value by address
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint16_t& src) {
    if ((addr + 1) >= EEPROM.length()) return 1;
    //uint8_t tByte = (src >> 8); EEPROM.update(addr, tByte);
    EEPROM.update(addr, (uint8_t)(src >> 8));
    //tByte = ((src << 8) >> 8); EEPROM.update(addr + 1, tByte);
    EEPROM.update(addr + 1, (uint8_t)src);               // TODO: Test it!!!!!
    uint16_t v = 0x0000;
    if (intRead(addr, v) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Write Longint value by address
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint32_t& src){
    if ((addr + 3) >= EEPROM.length()) return 1;
    EEPROM.update(addr, (uint8_t)(src >> 24));
    EEPROM.update(addr + 1, (uint8_t)(src >> 16));
    EEPROM.update(addr + 2, (uint8_t)(src >> 8));
    EEPROM.update(addr + 3, (uint8_t)src);
    uint32_t v = 0x00000000;
    if (intRead(addr, v) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Write String by address
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const String& src){
    if ((addr + src.length() - 1) >= EEPROM.length()) return 1;
    for (uint8_t i = 0; i < src.length(); i++) EEPROM.update(addr + i, src[i]);
    String v = "";
    if (intRead(addr, v, src.length()) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Fill block memory by default value
uint8_t LC_EEPROM::intFill(const uint32_t& addr, const uint32_t& cnt, const uint8_t& src) {
    if ((addr + cnt - 1) >= EEPROM.length()) return 1;
    for (uint32_t i = 0; i < cnt; i++) 
        if (intWrite(addr + i, src) != 0) return 2;    
    //if (intReadStr(addr, sendStr.length()) != sendStr) return 2; TODO: Check complete operation
    return 0;
}

// Write Block with BYTE data by address
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint8_t* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc, 0xFF) != 0) return 2;
        
    for (uint8_t i = 0; i < szSrc; i++) 
        if (intWrite(addr + i, src[i]) != 0) return 2;

    uint8_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Block with int8_t data by address
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int8_t* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc, 0x00) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++) {
        if (intWrite(addr + i, (uint8_t)(src[i] + 128)) != 0) return 2;
    }
    int8_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Block with char data by address
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const char* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc, 0x00) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++) {
        //if (src[i] == 0x00) break;
        if (intWrite(addr + i, (uint8_t)(src[i] + 128)) != 0) return 2;
    }
    char buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EEPROM::intShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan) {
    if (addrFrom >= addrTo) {
        Serial.print("Error: Incorrect memory range!");
        return 0;
    }
    
    Serial.println("Internal EEPROM memory...");

    // Header of table
    Serial.print(F("\n     "));
    for (uint8_t i = 0; i < quan; i++) {
        String tmpStr = _preFix(String(i, HEX), 2, '0') + ' ';
        tmpStr.toUpperCase();
        Serial.print(tmpStr);
    }
    Serial.println();
    for (uint16_t j = (addrFrom / quan); j < (addrTo / quan + 1); j++) {
        String tSt = _preFix(String(j * quan, HEX), 4, '0') + ' ';
        tSt.toUpperCase();
        Serial.print(tSt);              
        for (uint8_t i = 0; i < quan; i++) {
            uint8_t v = 0;
            intRead((j * quan) + i, v);
            String tmpStr = _preFix(String(v, HEX), 2, '0') + ' ';
            tmpStr.toUpperCase();
            Serial.print(tmpStr);
        }
        Serial.println();
    }
}

void LC_EEPROM::outBuffer(int8_t* src, const uint8_t& sz) {
    Serial.print("[");
    for (uint8_t n = 0; n < sz; n++) {
        Serial.print(src[n], DEC);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(uint8_t* src, const uint8_t& sz) {
    Serial.print("[");
    for (uint8_t n = 0; n < sz; n++) {
        if (src[n] < 16) Serial.print("0");
        Serial.print(src[n], HEX);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(char* src, const uint8_t& sz) {
    Serial.print("[");
    for (uint8_t n = 0; n < sz; n++) Serial.print(src[n]);
    Serial.println("]");
}

LC_EEPROM::~LC_EEPROM() {}

// ------------------------------------------ PROTECTED FUNCTIONS ------------------------------------------------------
String LC_EEPROM::_preFix(String str, uint8_t quan, char chr = '0') {
    String result = str;
    for (uint8_t i = 0; i < quan - str.length(); i++) result = chr + result;
    return result;
}

// ------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------
