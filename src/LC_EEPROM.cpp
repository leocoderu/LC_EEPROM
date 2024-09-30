#include "LC_EEPROM.h"

//using namespace LC_EEPROM;

LC_EEPROM::LC_EEPROM(){}

uint32_t LC_EEPROM::intTotalCapacity() { return EEPROM.length(); }

void LC_EEPROM::info(){
    Serial.print("Internal memory EEPROM range: [ 0x0000 - 0x"); Serial.print(_preFix(String(EEPROM.length() - 1, HEX), 4, '0')); Serial.println(" ]"); 
}

void LC_EEPROM::setArchDev(const uint8_t& archDev){
    this->_archDev = archDev;
}

// -------------------------------------------- PUBLIC FUNCTIONS -------------------------------------------------------
// Read - unsigned int 8 bit, [ 0 - 255 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint8_t& dst){
    if (addr >= EEPROM.length()) return 1;
    dst = EEPROM.read(addr);
    return 0;
}

// Read - signed int 8 bit, [ -128 - +127 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int8_t& dst){
    if (addr >= EEPROM.length()) return 1;
    dst = EEPROM.read(addr) - 128;
    return 0;
}

// Read - char 8 bit, [ 'A' / 'B' / another symbol ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, char& dst){
    if (addr >= EEPROM.length()) return 1;
    dst = EEPROM.read(addr) - 128;
    return 0;
}

// Read - unsigned int 16 bit, [ 0 - 65535 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint16_t& dst){
    if ((addr + 1) >= EEPROM.length()) return 1;
    dst = (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
    return 0;
}

// Read - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int16_t& dst){
    if ((addr + 1) >= EEPROM.length()) return 1;
    dst = ((EEPROM.read(addr) << 8) | EEPROM.read(addr + 1)) - 32768;
    return 0;
}

// Read - unsigned int 32 bit, [ 0 - 4294967295 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint32_t& dst){ 
    if ((addr + 3) >= EEPROM.length()) return 1;
    dst = ((uint32_t)EEPROM.read(addr) << 24) 
        | ((uint32_t)EEPROM.read(addr + 1) << 16) 
        | ((uint32_t)EEPROM.read(addr + 2) << 8) 
        | (uint32_t)EEPROM.read(addr + 3);
    return 0;
}

// Read - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int32_t& dst){
    if ((addr + 3) >= EEPROM.length()) return 1;
    dst = (((uint32_t)EEPROM.read(addr) << 24) 
        | ((uint32_t)EEPROM.read(addr + 1) << 16) 
        | ((uint32_t)EEPROM.read(addr + 2) << 8) 
        | (uint32_t)EEPROM.read(addr + 3)) - 2147483648;
    return 0;
}

// Read - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint64_t& dst){
    if ((addr + 7) >= EEPROM.length()) return 1;
    dst = ((uint64_t)EEPROM.read(addr) << 56) 
        | ((uint64_t)EEPROM.read(addr + 1) << 48) 
        | ((uint64_t)EEPROM.read(addr + 2) << 40) 
        | ((uint64_t)EEPROM.read(addr + 3) << 32) 
        | ((uint64_t)EEPROM.read(addr + 4) << 24) 
        | ((uint64_t)EEPROM.read(addr + 5) << 16) 
        | ((uint64_t)EEPROM.read(addr + 6) << 8) 
        | (uint64_t)EEPROM.read(addr + 7);
    return 0;
}

// Read - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int64_t& dst){
    if ((addr + 7) >= EEPROM.length()) return 1;
    dst = (((uint64_t)EEPROM.read(addr) << 56)
        | ((uint64_t)EEPROM.read(addr + 1) << 48)
        | ((uint64_t)EEPROM.read(addr + 2) << 40)
        | ((uint64_t)EEPROM.read(addr + 3) << 32)
        | ((uint64_t)EEPROM.read(addr + 4) << 24)
        | ((uint64_t)EEPROM.read(addr + 5) << 16)
        | ((uint64_t)EEPROM.read(addr + 6) << 8)
        | (uint64_t)EEPROM.read(addr + 7)) - 9223372036854775808UL;
    return 0;
}

// Read - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, float& dst){
    if ((addr + 3) >= EEPROM.length()) return 1;
    uint32_t v = ((uint32_t)EEPROM.read(addr) << 24)
        | ((uint32_t)EEPROM.read(addr + 1) << 16)
        | ((uint32_t)EEPROM.read(addr + 2) << 8)
        | (uint32_t)EEPROM.read(addr + 3);
    dst = *(float*)&v;
    return 0;
}

// Read - double 64 bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, double& dst){
    if (_archDev == 0) {                                    // For AVR
        if ((addr + 3) >= EEPROM.length()) return 1;
        uint32_t v = ((uint32_t)EEPROM.read(addr) << 24)
            | ((uint32_t)EEPROM.read(addr + 1) << 16)
            | ((uint32_t)EEPROM.read(addr + 2) << 8)
            | (uint32_t)EEPROM.read(addr + 3);
        dst = *(double*)&v;
    };
    if (_archDev == 1) {                                    // For ESP and another 32bit controllers
        if ((addr + 7) >= EEPROM.length()) return 1;
        uint64_t v = ((uint64_t)EEPROM.read(addr) << 56)
            | ((uint64_t)EEPROM.read(addr + 1) << 48)
            | ((uint64_t)EEPROM.read(addr + 2) << 40)
            | ((uint64_t)EEPROM.read(addr + 3) << 32)
            | ((uint64_t)EEPROM.read(addr + 4) << 24)
            | ((uint64_t)EEPROM.read(addr + 5) << 16)
            | ((uint64_t)EEPROM.read(addr + 6) << 8)
            | (uint64_t)EEPROM.read(addr + 7);
        dst = *(double*)&v;
    }
    return 0;
}

// Read - Bit 1 bit, [ 0 / 1 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, const uint8_t& nBit, bool& state){
    if (addr >= EEPROM.length()) return 1;      // If Address, out if memory, return error code 1
    state = bitRead(EEPROM.read(addr), nBit);   // Get Bit in Byte by address
    return 0;
}

// Read - String
uint8_t LC_EEPROM::intRead(const uint32_t& addr, String& dst, const uint8_t& szDst){    
    if ((addr + szDst - 1) >= EEPROM.length()) return 1;
    dst = "";
    for (uint8_t i = 0; i < szDst; i++)
        dst += char(EEPROM.read(addr + i));
    return 0;
}

// Read Buffer - unsigned int 8 bit, [ 0 - 255 ]
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

// Read Buffer - signed int 8 bit, [ -128 - +127 ]
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

// Read Buffer - char, [ -128 - +127 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, char* dst, const uint8_t& szDst){
    if ((addr + szDst - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst);                     
    for (uint8_t i = 0; i < szDst; i++){
        uint8_t v = 0;
        if (intRead(addr + i, v) != 0) return 2;
        //if (bt == 0x00) break;
        //dst[i] = (char)(v - 128);
        dst[i] = (v - 128);
    }
    return 0;
}

// Read Buffer - unsigned int 16 bit, [ 0 - 65535 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint16_t* dst, const uint8_t& szDst){
    if ((addr + (szDst * 2) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst * 2);
    for (uint8_t i = 0; i < szDst; i++){
        uint16_t v = 0;
        if (intRead(addr + (i * 2), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int16_t* dst, const uint8_t& szDst){
    if ((addr + (szDst * 2) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst * 2);
    for (uint8_t i = 0; i < szDst; i++){
        uint16_t v = 0;
        if (intRead(addr + (i * 2), v) != 0) return 2;
        dst[i] = (v - 32768);
    }
    return 0;
}

// Read Buffer - unsigned int 32 bit, [ 0 - 4294967295 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint32_t* dst, const uint8_t& szDst){
    if ((addr + (szDst * 4) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst * 4);
    for (uint8_t i = 0; i < szDst; i++){
        uint32_t v = 0;
        if (intRead(addr + (i * 4), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int32_t* dst, const uint8_t& szDst){
    if ((addr + (szDst * 4) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst * 4);
    for (uint8_t i = 0; i < szDst; i++){
        uint32_t v = 0;
        if (intRead(addr + (i * 4), v) != 0) return 2;
        dst[i] = (v - 2147483648);
    }
    return 0;
}

// Read Buffer - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint64_t* dst, const uint8_t& szDst){
    if ((addr + (szDst * 8) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst * 8);
    for (uint8_t i = 0; i < szDst; i++){
        uint64_t v = 0;
        if (intRead(addr + (i * 8), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int64_t* dst, const uint8_t& szDst){
    if ((addr + (szDst * 8) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst * 8);
    for (uint8_t i = 0; i < szDst; i++){
        uint64_t v = 0;
        if (intRead(addr + (i * 8), v) != 0) return 2;
        dst[i] = (v - 9223372036854775808UL);
    }
    return 0;
}

// Read Buffer - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, float* dst, const uint8_t& szDst){
    if ((addr + (szDst * 4) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst * 4);
    for (uint8_t i = 0; i < szDst; i++) {
        float v = 0;
        if (intRead(addr + (i * 4), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - double 64 bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, double* dst, const uint8_t& szDst) {
    if (_archDev == 0) {                                            // For AVR
        if ((addr + (szDst * 4) - 1) >= EEPROM.length()) return 1;  // If we try read block outside, return error
        memset(dst, 0xFF, szDst * 4);
        for (uint8_t i = 0; i < szDst; i++) {
            double v = 0;
            if (intRead(addr + (i * 4), v) != 0) return 2;
            dst[i] = v;
        }
    };
    if (_archDev == 1) {                                            // For ESP and another 32bit controllers
        if ((addr + (szDst * 8) - 1) >= EEPROM.length()) return 1;  // If we try read block outside, return error
        memset(dst, 0xFF, szDst * 8);
        for (uint8_t i = 0; i < szDst; i++) {
            double v = 0;
            if (intRead(addr + (i * 8), v) != 0) return 2;
            dst[i] = v;
        }
    }
    return 0;
}

// ---------------------------------------------  Write Functions  ----------------------------------------------------
// Write - unsigned int 8bit, [ 0 - 255 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint8_t& src){    
    if (addr >= EEPROM.length()) return 1;
    EEPROM.update(addr, src); 
    uint8_t v = 0;
    if (intRead(addr, v) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Write - signed int 8bit, [ -128 - +127 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int8_t& src){
    if (addr >= EEPROM.length()) return 1;
    EEPROM.update(addr, src + 128);
    int8_t v = 0;
    if (intRead(addr, v) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Write - char 8bit, [ 'A' / 'B' / another symbol ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const char& src){
    if (addr >= EEPROM.length()) return 1;
    EEPROM.update(addr, src + 128);
    char  v = '\0';
    if (intRead(addr, v) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Write - unsigned int 16bit, [ 0 - 65535 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint16_t& src){
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

// Write - signed int 16bit, [ -32768 - +32767 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int16_t& src) {
    if ((addr + 1) >= EEPROM.length()) return 1;
    
    uint16_t v = (uint16_t)(src + 32768);
    EEPROM.update(addr, (uint8_t)(v >> 8));
    EEPROM.update(addr + 1, (uint8_t)v);

    v = 0x0000;
    if (intRead(addr, v) != 0) return 2;
    if ((v - 32768) != src) return 2;
    return 0;
}

// Write - unsigned int 32bit, [ 0 - 4294967295 ]
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

// Write - signed int 32bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int32_t& src){
    if ((addr + 3) >= EEPROM.length()) return 1;

    uint32_t v = (uint32_t)(src + 2147483648);
    EEPROM.update(addr, (uint8_t)(v >> 24));
    EEPROM.update(addr + 1, (uint8_t)(v >> 16));
    EEPROM.update(addr + 2, (uint8_t)(v >> 8));
    EEPROM.update(addr + 3, (uint8_t)v);    
    v = 0x00000000;
    if (intRead(addr, v) != 0) return 2;
    if ((v - 2147483648) != src) return 2;
    return 0;
}

// Write - unsigned int 64bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint64_t& src){
    if ((addr + 7) >= EEPROM.length()) return 1;
    EEPROM.update(addr, (uint8_t)(src >> 56));
    EEPROM.update(addr + 1, (uint8_t)(src >> 48));
    EEPROM.update(addr + 2, (uint8_t)(src >> 40));
    EEPROM.update(addr + 3, (uint8_t)(src >> 32));
    EEPROM.update(addr + 4, (uint8_t)(src >> 24));
    EEPROM.update(addr + 5, (uint8_t)(src >> 16));
    EEPROM.update(addr + 6, (uint8_t)(src >> 8));
    EEPROM.update(addr + 7, (uint8_t)src);
    uint64_t v = 0x0000000000000000;
    if (intRead(addr, v) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Write - signed int 64bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int64_t& src){
    if ((addr + 7) >= EEPROM.length()) return 1;

    uint64_t v = (uint64_t)(src + 9223372036854775808UL);
    EEPROM.update(addr, (uint8_t)(v >> 56));
    EEPROM.update(addr + 1, (uint8_t)(v >> 48));
    EEPROM.update(addr + 2, (uint8_t)(v >> 40));
    EEPROM.update(addr + 3, (uint8_t)(v >> 32));
    EEPROM.update(addr + 4, (uint8_t)(v >> 24));
    EEPROM.update(addr + 5, (uint8_t)(v >> 16));
    EEPROM.update(addr + 6, (uint8_t)(v >> 8));
    EEPROM.update(addr + 7, (uint8_t)src);

    v = 0x0000000000000000;
    if (intRead(addr, v) != 0) return 2;
    if ((v - 9223372036854775808UL) != src) return 2;
    return 0;
}

// Write - float 32bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const float& src){
    if ((addr + 3) >= EEPROM.length()) return 1;
    uint32_t v = *(unsigned long*)&src;
    EEPROM.update(addr, (uint8_t)(v >> 24));
    EEPROM.update(addr + 1, (uint8_t)(v >> 16));
    EEPROM.update(addr + 2, (uint8_t)(v >> 8));
    EEPROM.update(addr + 3, (uint8_t)v);
    
    float z = 0;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) return 2;
    return 0;
}

// Write - double 64bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const double& src){
    if (_archDev == 0) {
        if ((addr + 3) >= EEPROM.length()) return 1;
        uint32_t v = *(unsigned long*)&src;
        EEPROM.update(addr, (uint8_t)(v >> 24));
        EEPROM.update(addr + 1, (uint8_t)(v >> 16));
        EEPROM.update(addr + 2, (uint8_t)(v >> 8));
        EEPROM.update(addr + 3, (uint8_t)v);

        double z = 0;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    };
    if (_archDev == 1) {
        if ((addr + 7) >= EEPROM.length()) return 1;

        uint64_t v = *(uint64_t*)&src;
        EEPROM.update(addr, (uint8_t)(v >> 56));
        EEPROM.update(addr + 1, (uint8_t)(v >> 48));
        EEPROM.update(addr + 2, (uint8_t)(v >> 40));
        EEPROM.update(addr + 3, (uint8_t)(v >> 32));
        EEPROM.update(addr + 4, (uint8_t)(v >> 24));
        EEPROM.update(addr + 5, (uint8_t)(v >> 16));
        EEPROM.update(addr + 6, (uint8_t)(v >> 8));
        EEPROM.update(addr + 7, (uint8_t)v);
        
        double z = 0.0;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    };
    return 0;
}

// Write - Bit 1 bit, [ 0 / 1 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint8_t& nBit, const bool& state){
    if (addr >= EEPROM.length()) return 1;      // If Address, out if memory, return error code 1
    uint8_t src = EEPROM.read(addr);            // Read The byte by address
    bitWrite(src, nBit, state);                 // Set Bit in Byte
    EEPROM.update(addr, src);                   // Update The byte

    bool v = false;    
    if (intRead(addr, nBit, v) != 0) return 2;  // Get Info of Bit by address
    if (v != state) return 2;                   // Compare info
    return 0;
}

// Write - String
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const String& src){
    if ((addr + src.length() - 1) >= EEPROM.length()) return 1;
    for (uint8_t i = 0; i < src.length(); i++) EEPROM.update(addr + i, src[i]);
    String v = "";
    if (intRead(addr, v, src.length()) != 0) return 2;
    if (v != src) return 2;
    return 0;
}

// Write Buffer - unsigned int 8 bit, [ 0 - 255 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint8_t* src, const uint8_t& szSrc){
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc, 0xFF) != 0) return 2;        
    for (uint8_t i = 0; i < szSrc; i++) 
        if (intWrite(addr + i, src[i]) != 0) return 2;
    uint8_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - signed int 8 bit, [ -128 - +127 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int8_t* src, const uint8_t& szSrc){
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc, 0x00) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++) 
        if (intWrite(addr + i, (uint8_t)(src[i] + 128)) != 0) return 2;
    int8_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - char 8 bit, [ -128 - +127 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const char* src, const uint8_t& szSrc){
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc, 0x00) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++){
        //if (src[i] == 0x00) break;
        if (intWrite(addr + i, (uint8_t)(src[i] + 128)) != 0) return 2;
    }
    char buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - unsigned int 16 bit, [ 0 - 65535 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint16_t* src, const uint8_t& szSrc){
    if ((addr + (szSrc * 2) - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc * 2, 0xFF) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++)
        if (intWrite(addr + (i * 2), src[i]) != 0) return 2;
    uint16_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int16_t* src, const uint8_t& szSrc){
    if ((addr + (szSrc * 2) - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc * 2, 0x00) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++)
        if (intWrite(addr + (i * 2), (uint16_t)(src[i] + 32768)) != 0) return 2;
    int16_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - unsigned int 32 bit, [ 0 - 4294967295 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint32_t* src, const uint8_t& szSrc){
    if ((addr + (szSrc * 4) - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc * 4, 0xFF) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++)
        if (intWrite(addr + (i * 4), src[i]) != 0) return 2;
    uint32_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int32_t* src, const uint8_t& szSrc){
    if ((addr + (szSrc * 4) - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc * 4, 0x00) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++) 
        if (intWrite(addr + (i * 4), (uint32_t)(src[i] + 2147483648)) != 0) return 2;
    int32_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint64_t* src, const uint8_t& szSrc){
    if (intFill(addr, szSrc * 8, 0xFF) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++)
        if (intWrite(addr + (i * 8), src[i]) != 0) return 2;
    uint64_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int64_t* src, const uint8_t& szSrc){
    if ((addr + (szSrc * 8) - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc * 8, 0x00) != 0) return 2;
    for (uint8_t i = 0; i < szSrc; i++) {
        if (intWrite(addr + (i * 8), (uint64_t)(src[i] + 9223372036854775808UL)) != 0) return 2;
    }
    int64_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;

    return 0;
}

// Write Buffer - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const float* src, const uint8_t& szSrc){
    if ((addr + (szSrc * 4) - 1) >= EEPROM.length()) return 1;
    if (intFill(addr, szSrc * 4, 0xFF) != 0) return 2;

    for (uint8_t i = 0; i < szSrc; i++) 
        if (intWrite(addr + (i * 4), src[i]) != 0) return 2;
    
    float buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    return 0;
}

// Write Buffer - double 64 bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const double* src, const uint8_t& szSrc) {
    if (_archDev == 0) {
        if ((addr + (szSrc * 4) - 1) >= EEPROM.length()) return 1;
        if (intFill(addr, szSrc * 4, 0xFF) != 0) return 2;

        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * 4), src[i]) != 0) return 2;

        double buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    };
    if (_archDev == 1) {
        if ((addr + (szSrc * 8) - 1) >= EEPROM.length()) return 1;
        if (intFill(addr, szSrc * 8, 0xFF) != 0) return 2;

        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * 8), src[i]) != 0) return 2;

        double buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    };
    return 0;
}

// Fill block memory by default value
uint8_t LC_EEPROM::intFill(const uint32_t& addr, const uint32_t& cnt, const uint8_t& src){
    if ((addr + cnt - 1) >= EEPROM.length()) return 1;
    for (uint32_t i = 0; i < cnt; i++)
        if (intWrite(addr + i, src) != 0) return 2;
    return 0;
}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EEPROM::intShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan) {
    if (addrFrom >= addrTo) {
        Serial.print("Error: Incorrect memory range!");
    }
    else {
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

uint32_t LC_EEPROM::_float_to_long(const float& f) { return *(unsigned long*)&f; }

// ------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------
