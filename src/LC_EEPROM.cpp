#include "LC_EEPROM.h"

LC_EEPROM::LC_EEPROM(){}

uint32_t LC_EEPROM::intTotalCapacity() { return EEPROM.length(); }

void LC_EEPROM::info() {
    Serial.print("Internal memory EEPROM range: [ 0x0000 - 0x"); Serial.print(_preFix(String(EEPROM.length() - 1, HEX), 4, '0')); Serial.println(" ]"); 
}

// -------------------------------------------- PUBLIC FUNCTIONS -------------------------------------------------------
// Read - unsigned int 8 bit, [ 0 - 255 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint8_t& dst) {
    if (addr >= EEPROM.length()) return 1;
    dst = EEPROM.read(addr);
    return 0;
}

// Read - signed int 8 bit, [ -128 - +127 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int8_t& dst) {
    if (addr >= EEPROM.length()) return 1;
    dst = EEPROM.read(addr) - 128;
    return 0;
}

// Read - char 8 bit, [ 'A' / 'B' / another symbol ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, char& dst) {
    if (addr >= EEPROM.length()) return 1;
    dst = EEPROM.read(addr) - 128;
    return 0;
}

// Read - unsigned int 16 bit, [ 0 - 65535 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint16_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= EEPROM.length()) return 1;
    dst = 0x0000;
    for (uint8_t n = 0; n < sz; n++) 
        dst |= (uint16_t)(EEPROM.read(addr + n)) << (8 * (sz - 1 - n));    
    return 0;
}

// Read - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int16_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= EEPROM.length()) return 1;
    uint16_t v = 0;
    intRead(addr, v);
    dst = (int16_t)(v - 32768);
    return 0;
}

// Read - unsigned int 32 bit, [ 0 - 4294967295 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint32_t& dst) { 
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= EEPROM.length()) return 1;
    dst = 0x00000000;
    for (uint8_t n = 0; n < sz; n++)
        dst |= (uint32_t)(EEPROM.read(addr + n)) << (8 * (sz - 1 - n));
    return 0;
}

// Read - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int32_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= EEPROM.length()) return 1;
    uint32_t v = 0;
    if (intRead(addr, v) != 0) return 2;
    dst = (int32_t)(v - 2147483648);
    return 0;
}

// Read - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint64_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= EEPROM.length()) return 1;
    dst = 0x0000000000000000;
    for (uint8_t n = 0; n < sz; n++)
        dst |= (uint64_t)(EEPROM.read(addr + n)) << (8 * (sz - 1 - n));
    return 0;
}

// Read - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int64_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= EEPROM.length()) return 1;
    uint64_t v = 0;
    if (intRead(addr, v) != 0) return 2;
    dst = (int64_t)(v - 9223372036854775808UL);
    return 0;
}

// Read - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, float& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= EEPROM.length()) return 1;
    uint32_t v = 0;
    if (intRead(addr, v) != 0) return 2;
    dst = *(float*)&v;
    return 0;
}

// Read - double 64 bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, double& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable
    if ((addr + sz - 1) >= EEPROM.length()) return 1;

    if (sz == 4) {                                    // For AVR
        uint32_t v = 0;
        if (intRead(addr, v) != 0) return 2;
        dst = *(double*)&v;
    };
    if (sz == 8) {                                    // For ESP and another 32bit controllers
        uint64_t v = 0;
        if (intRead(addr, v) != 0) return 2;
        dst = *(double*)&v;
    }
    return 0;
}

// Read - Bit 1 bit, [ 0 / 1 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, const uint8_t& nBit, bool& state) {
    if ((nBit > 7) || (addr >= EEPROM.length())) return 1;  // If Address, out if memory, return error code 1
    state = bitRead(EEPROM.read(addr), nBit);               // Get Bit in Byte by address
    return 0;
}

// Read - String
uint8_t LC_EEPROM::intRead(const uint32_t& addr, String& dst, const uint8_t& szDst) {    
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
    if ((addr + szDst - 1) >= EEPROM.length()) return 1;        // If we try read block outside, return error
    memset(dst, 0x00, szDst);                                   // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        int8_t v = 0;
        if (intRead(addr + i, v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - char, [ -128 - +127 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, char* dst, const uint8_t& szDst) {
    if ((addr + szDst - 1) >= EEPROM.length()) return 1;        // If we try read block outside, return error
    memset(dst, 0x00, szDst);                                   // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++){
        char v = '\0';
        if (intRead(addr + i, v) != 0) return 2;
        if (v == 0x00) break;                                   // Read characters up to the zero character like end of the line
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - unsigned int 16 bit, [ 0 - 65535 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint16_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                // Get size element of array

    if ((addr + (szDst * sz) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);                              // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++){
        uint16_t v = 0;
        if (intRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int16_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                // Get size element of array

    if ((addr + (szDst * sz) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst * sz);
    for (uint8_t i = 0; i < szDst; i++){
        int16_t v = 0;
        if (intRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - unsigned int 32 bit, [ 0 - 4294967295 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint32_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                // Get size element of array

    if ((addr + (szDst * sz) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);
    for (uint8_t i = 0; i < szDst; i++){
        uint32_t v = 0;
        if (intRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int32_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                // Get size element of array

    if ((addr + (szDst * sz) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst * sz);
    for (uint8_t i = 0; i < szDst; i++){
        int32_t v = 0;
        if (intRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, uint64_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                // Get size element of array

    if ((addr + (szDst * sz) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);
    for (uint8_t i = 0; i < szDst; i++){
        uint64_t v = 0;
        if (intRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, int64_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                // Get size element of array

    if ((addr + (szDst * sz) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst * sz);
    for (uint8_t i = 0; i < szDst; i++){
        int64_t v = 0;
        if (intRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, float* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                // Get size element of array

    if ((addr + (szDst * sz) - 1) >= EEPROM.length()) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);
    for (uint8_t i = 0; i < szDst; i++) {
        float v = 0;
        if (intRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - double 64 bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EEPROM::intRead(const uint32_t& addr, double* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                    // Get size of double variable, it may be 4 / 8

    if ((addr + (szDst * sz) - 1) >= EEPROM.length()) return 1;     // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);
    for (uint8_t i = 0; i < szDst; i++) {
        double v = 0;
        if (intRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }    
    return 0;
}

// ---------------------------------------------  Write Functions  ----------------------------------------------------
// Write - unsigned int 8bit, [ 0 - 255 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint8_t& src) {    
    if (addr >= EEPROM.length()) return 1;
    uint8_t z = 0;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        EEPROM.update(addr, src);
       
        z = 0;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - signed int 8bit, [ -128 - +127 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int8_t& src) {
    if (addr >= EEPROM.length()) return 1;
    
    int8_t z = 0;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        EEPROM.update(addr, (uint8_t)(src + 128));
        
        z = 0;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - char 8bit, [ 'A' / 'B' / another symbol ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const char& src) {
    if (addr >= EEPROM.length()) return 1;

    char z = '\0';
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        EEPROM.update(addr, (uint8_t)(src + 128));

        z = '\0';
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - unsigned int 16bit, [ 0 - 65535 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint16_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= EEPROM.length()) return 1;

    uint16_t z = 0x0000;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        for (uint8_t n = 0; n < sz; n++)
            EEPROM.update(addr + n, (uint8_t)(src >> (8 * (sz - 1 - n))));
        
        z = 0x0000;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - signed int 16bit, [ -32768 - +32767 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int16_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= EEPROM.length()) return 1;
    
    int16_t z = 0x0000;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        uint16_t v = (uint16_t)(src + 32768);
        if (intWrite(addr, v) != 0) return 2;

        z = 0x0000;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - unsigned int 32bit, [ 0 - 4294967295 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint32_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= EEPROM.length()) return 1;

    uint32_t z = 0x00000000;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        for (uint8_t n = 0; n < sz; n++)
            EEPROM.update(addr + n, (uint8_t)(src >> (8 * (sz - 1 - n))));

        z = 0x00000000;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - signed int 32bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int32_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= EEPROM.length()) return 1;

    int32_t z = 0x00000000;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        uint32_t v = (uint32_t)(src + 2147483648);
        if (intWrite(addr, v) != 0) return 2;

        z = 0x00000000;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - unsigned int 64bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint64_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= EEPROM.length()) return 1;

    uint64_t z = 0x0000000000000000;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        for (uint8_t n = 0; n < sz; n++)
            EEPROM.update(addr + n, (uint8_t)(src >> (8 * (sz - 1 - n))));
        
        z = 0x0000000000000000;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - signed int 64bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int64_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= EEPROM.length()) return 1;

    int64_t z = 0x0000000000000000;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        uint64_t v = (uint64_t)(src + 9223372036854775808UL);
        if (intWrite(addr, v) != 0) return 2;

        z = 0x0000000000000000;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - float 32bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const float& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= EEPROM.length()) return 1;

    float z = 0.0;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        uint32_t v = *(unsigned long*)&src;
        if (intWrite(addr, v) != 0) return 2;

        z = 0.0;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - double 64bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const double& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= EEPROM.length()) return 1;

    double z = 0.0;
    if (intRead(addr, z) != 0) return 2;
    if (z != src) {
        if (sz == 4) {
            uint32_t v = *(unsigned long*)&src;
            if (intWrite(addr, v) != 0) return 2;
        };
        if (sz == 8) {
            uint64_t v = *(uint64_t*)&src;
            if (intWrite(addr, v) != 0) return 2;
        };
        z = 0.0;
        if (intRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - Bit 1 bit, [ 0 / 1 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint8_t& nBit, const bool& state) {
    if ((nBit > 7) || (addr >= EEPROM.length())) return 1;  // If Address, out if memory, return error code 1

    bool z = false;
    if (intRead(addr, nBit, z) != 0) return 2;              // Get Info of Bit by address
    if (z != state) {
        uint8_t src = EEPROM.read(addr);                        // Read The byte by address
        bitWrite(src, nBit, state);                             // Set Bit in Byte
        EEPROM.update(addr, src);                               // Update The byte

        z = false;
        if (intRead(addr, nBit, z) != 0) return 2;              // Get Info of Bit by address
        if (z != state) return 2;                               // Compare info
    }
    return 0;
}

// Write - String
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const String& src) {
    if ((addr + src.length() - 1) >= EEPROM.length()) return 1;

    String z = "";
    if (intRead(addr, z, src.length()) != 0) return 2;
    if (z != src) {
        for (uint8_t i = 0; i < src.length(); i++) EEPROM.update(addr + i, (uint8_t)src[i]);

        z = "";
        if (intRead(addr, z, src.length()) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write Buffer - unsigned int 8 bit, [ 0 - 255 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint8_t* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;

    uint8_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc, 0xFF) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + i, src[i]) != 0) return 2;
        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - signed int 8 bit, [ -128 - +127 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int8_t* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;

    int8_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc, 0x00) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + i, (uint8_t)(src[i] + 128)) != 0) return 2;
        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - char 8 bit, [ -128 - +127 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const char* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= EEPROM.length()) return 1;

    char buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc, 0x00) != 0) return 2;                          // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++) {
            if (src[i] == 0x00) break;                                          // If write '\0' symbol, it is end of write char array
            if (intWrite(addr + i, (uint8_t)(src[i] + 128)) != 0) return 2;     // if src = -128 then res = 0, if src = +127 then res = 255        
        }
        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - unsigned int 16 bit, [ 0 - 65535 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint16_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= EEPROM.length()) return 1;

    uint16_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc * sz, 0xFF) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * sz), src[i]) != 0) return 2;
        
        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int16_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= EEPROM.length()) return 1;

    int16_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc * sz, 0x00) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * sz), (uint16_t)(src[i] + 32768)) != 0) return 2;
        
        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - unsigned int 32 bit, [ 0 - 4294967295 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint32_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= EEPROM.length()) return 1;

    uint32_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc * sz, 0xFF) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * sz), src[i]) != 0) return 2;

        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int32_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= EEPROM.length()) return 1;

    int32_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc * sz, 0x00) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * sz), (uint32_t)(src[i] + 2147483648)) != 0) return 2;

        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const uint64_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= EEPROM.length()) return 1;

    uint64_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc * sz, 0xFF) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * sz), src[i]) != 0) return 2;

        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const int64_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= EEPROM.length()) return 1;

    int64_t buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc * sz, 0x00) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++) 
            if (intWrite(addr + (i * sz), (uint64_t)(src[i] + 9223372036854775808UL)) != 0) return 2;

        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const float* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= EEPROM.length()) return 1;

    float buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc * sz, 0xFF) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * sz), src[i]) != 0) return 2;

        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - double 64 bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EEPROM::intWrite(const uint32_t& addr, const double* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src);
    if ((addr + (szSrc * sz) - 1) >= EEPROM.length()) return 1;

    double buff[szSrc] = {};
    if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (intFill(addr, szSrc * sz, 0xFF) != 0) return 2;
        for (uint8_t i = 0; i < szSrc; i++)
            if (intWrite(addr + (i * sz), src[i]) != 0) return 2;

        buff[szSrc] = {};
        if (intRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Fill block memory by default value
uint8_t LC_EEPROM::intFill(const uint32_t& addr, const uint32_t& cnt, const uint8_t& src) {
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

//uint32_t LC_EEPROM::_float_to_long(const float& f) { return *(unsigned long*)&f; }

// ------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------
