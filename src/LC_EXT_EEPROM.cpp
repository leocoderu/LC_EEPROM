#include "LC_EEPROM.h"

// CONSTRUCTOR
LC_EXT_EEPROM::LC_EXT_EEPROM(const eeprom_model_t& devModel) {    
    _getModelInfo(devModel);
    if (_pSize > 30) _pSize = 30; // Rewrite Page Size, becouse Wire buffer has limit in 32 byte, 2 bytes for address and 30 bytes for data.
}

// Getters and Setters
uint32_t LC_EXT_EEPROM::extTotalCapacity() { return _totalCapacity; }
bool LC_EXT_EEPROM::exists() {
    return (_checkQDevices() > 0);
}
void LC_EXT_EEPROM::info() {
    Serial.print("Internal memory EEPROM range: [ 0x0000 - 0x"); Serial.print(_preFix(String(EEPROM.length() - 1, HEX), 4, '0')); Serial.println(" ]");
    Serial.print("External memory EEPROM has "); 
    if (_qDevice > 0) {
        Serial.print(_qDevice); Serial.println(" modules.");
        Serial.print("Capacity of module is "); Serial.print(_devCapacity); Serial.println(" kBit");
        Serial.print("Frequency I2C is "); Serial.print(_twiFreq * 100); Serial.println(" kHz");
        Serial.println("# I2C \tAddress range");
        for (uint8_t i = 0; i < _qDevice; i++) {
            Serial.print(i); Serial.print(" 0x"); Serial.print(_getCtrlByte(i * (_devCapacity * 1024.0 / 8)), HEX); 
            Serial.print("\t[ 0x"); Serial.print(_preFix(String((uint32_t)(i * (_devCapacity * 1024.0 / 8)), HEX), 5, '0')); Serial.print(" - ");
            Serial.print("0x"); Serial.print(_preFix(String((uint32_t)((i+1) * (_devCapacity * 1024.0 / 8)) - 1, HEX), 5, '0')); Serial.print(" ]");
            Serial.println();
        }
    } else {
        Serial.print("not"); Serial.println(" modules.");
    }
}
// Returns total Capacity of memory in bytes
uint16_t LC_EXT_EEPROM::getCapacity() { return _devCapacity; }
void LC_EXT_EEPROM::setCapacity(uint16_t capacity) {
    this->_devCapacity = capacity;
    _setTotalCapacity();
}
uint8_t LC_EXT_EEPROM::getQDevices() { return _qDevice; }
void    LC_EXT_EEPROM::setQDevices(uint8_t qDevices) { 
    this->_qDevice = qDevices;
    _setTotalCapacity();
};
uint8_t LC_EXT_EEPROM::getPageSize() { return _pSize; }
void LC_EXT_EEPROM::setPageSize(uint8_t pSize) { this->_pSize = pSize; }
uint8_t LC_EXT_EEPROM::getFrequency() { return _twiFreq; }
void LC_EXT_EEPROM::setFrequency(uint8_t freq) { this->_twiFreq = freq; }

// BEGINS transmission
uint8_t LC_EXT_EEPROM::begin() {
    Wire.begin();
    Wire.setClock(_twiFreq * 100000);
    this->_qDevice = _checkQDevices();
    _setTotalCapacity();
    Wire.beginTransmission(I2CADDR);
    _sendAddr(0x0000);
    return Wire.endTransmission();
}

// -------------------------------------------- PUBLIC FUNCTIONS -------------------------------------------------------

// Read - unsigned int 8bit, [ 0 - 255 ]
// Return result of reading data: 0 - success, 1 - error out of memory, 2 - error of read
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, uint8_t& dst) {
    if (addr >= _totalCapacity) return 1;
    uint8_t ctrlByte = _getCtrlByte(addr);
    Wire.beginTransmission(ctrlByte);                       // Control byte 0x50 .... 0x57
    _sendAddr(addr);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)ctrlByte, (uint8_t)1);        // In Wire library present two functions, (uint8_t) for disable attentions
    if (!Wire.available()) return 2;
    dst = Wire.read(); 
    return 0;
}

// Read - signed int 8 bit, [ -128 - +127 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, int8_t& dst) {
    if (addr >= _totalCapacity) return 1;
    uint8_t v = 0;
    if (extRead(addr, v) != 0) return 2;
    dst = (int8_t)(v - 128);
    return 0;
}

// Read - char 8 bit, [ 'A' / 'B' / another symbol ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, char& dst) {
    if (addr >= _totalCapacity) return 1;
    uint8_t v = 0;
    if (extRead(addr, v) != 0) return 2;
    dst = (char)(v - 128);
    return 0;
}

// Read - unsigned int 16bit, [ 0 - 65535 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, uint16_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= _totalCapacity) return 1;
    dst = 0x0000;
    for (uint8_t n = 0; n < sz; n++) {
        uint8_t v = 0;
        if (extRead(addr + n, v) != 0) { dst = 0x0000; return 2; }
        dst |= (uint16_t)v << (8 * (sz - 1 - n));
    }
    return 0;
}

// Read - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, int16_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= _totalCapacity) return 1;
    uint16_t v = 0;
    if (extRead(addr, v) != 0) return 2;
    dst = (int16_t)(v - 32768);
    return 0;
}

// Read - unsigned int 32bit, [ 0 - 4294967295 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, uint32_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= _totalCapacity) return 1;
    dst = 0x00000000;
    for (uint8_t n = 0; n < sz; n++) {
        uint8_t  v = 0;
        if (extRead(addr + n, v) != 0) { dst = 0x00000000; return 2; }
        dst |= (uint32_t)v << (8 * (sz - 1 - n));      
    }
    return 0;
}

// Read - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, int32_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= _totalCapacity) return 1;
    uint32_t v = 0;
    if (extRead(addr, v) != 0) return 2;
    dst = (int32_t)(v - 2147483648);
    return 0;
}

// Read - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, uint64_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= _totalCapacity) return 1;
    dst = 0x0000000000000000;
    for (uint8_t n = 0; n < sz; n++) {
        uint8_t  v = 0;
        if (extRead(addr + n, v) != 0) { dst = 0x0000000000000000; return 2; }
        dst |= (uint64_t)v << (8 * (sz - 1 - n));
    }
    return 0;
}

// Read - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, int64_t& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= _totalCapacity) return 1;
    uint64_t v = 0;
    if (extRead(addr, v) != 0) return 2;
    dst = (int64_t)(v - 9223372036854775808UL);
    return 0;
}

// Read - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, float& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of variable

    if ((addr + sz - 1) >= _totalCapacity) return 1;
    uint32_t v = 0;
    if (extRead(addr, v) != 0) return 2;
    dst = *(float*)&v;
    return 0;
}

// Read - double 64 bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, double& dst) {
    uint8_t sz = sizeof(dst);                                       // Get size of double variable, it may be 4 / 8
    if ((addr + sz - 1) >= _totalCapacity) return 1;

    if (sz == 4) {
        uint32_t v = 0;
        if (extRead(addr, v) != 0) return 2;
        dst = *(double*)&v;
    };
    if (sz == 8) {
        uint64_t v = 0;
        if (extRead(addr, v) != 0) return 2;
        dst = *(double*)&v;
    };
    
    return 0;
}

// Read - Bit 1 bit, [ 0 / 1 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, const uint8_t& nBit, bool& state) {
    if ((nBit > 7) || (addr >= _totalCapacity)) return 1;       // If Address, out if memory, return error code 1
    uint8_t v = 0;  
    if (extRead(addr, v) != 0) return 2;                        // Get Byte by address
    state = bitRead(v, nBit);                                   // Get Bit in Byte by address
    return 0;
}

// Read - String
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, String& dst, const uint8_t& szDst) {
    if ((addr + szDst - 1) >= _totalCapacity) return 1;
    dst = "";
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t v = 0;
        if (extRead(addr + i, v) != 0) return 2;
        dst += char(v);
    }
    return 0;
}

// Read Buffer - unsigned int 8 bit, [ 0 - 255 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, uint8_t* dst, const uint8_t& szDst) {
    if ((addr + szDst - 1) >= _totalCapacity) return 1; // If we try read block outside, return error
    memset(dst, 0xFF, szDst);                           // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t v = 0;
        if (extRead(addr + i, v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 8 bit, [ -128 - +127 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, int8_t* dst, const uint8_t& szDst) {
    if ((addr + szDst - 1) >= _totalCapacity) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst);                           // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        int8_t v = 0;
        if (extRead(addr + i, v) != 0) return 2;
        dst[i] = v;
    }   
    return 0;
}

// Read Buffer - char, [ -128 - +127 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, char* dst, const uint8_t& szDst) {
    if ((addr + szDst - 1) >= _totalCapacity) return 1; // If we try read block outside, return error
    memset(dst, 0x00, szDst);                           // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        char v = '\0';
        if (extRead(addr + i, v) != 0) return 2;
        if (v == 0x00) break;                           // Read characters up to the zero character like end of the line
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - unsigned int 16 bit, [ 0 - 65535 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, uint16_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);

    if ((addr + (szDst * sz) - 1) >= _totalCapacity) return 1;   // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);                               // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        uint16_t v = 0;
        if (extRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, int16_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);

    if ((addr + (szDst * sz) - 1) >= _totalCapacity) return 1;   // If we try read block outside, return error
    memset(dst, 0x00, szDst * sz);                               // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        int16_t v = 0;
        if (extRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - unsigned int 32 bit, [ 0 - 4294967295 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, uint32_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);

    if ((addr + (szDst * sz) - 1) >= _totalCapacity) return 1;   // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);                               // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        uint32_t v = 0;
        if (extRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, int32_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);

    if ((addr + (szDst * sz) - 1) >= _totalCapacity) return 1;   // If we try read block outside, return error
    memset(dst, 0x00, szDst * sz);                               // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        int32_t v = 0;
        if (extRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, uint64_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);

    if ((addr + (szDst * sz) - 1) >= _totalCapacity) return 1;   // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);                               // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        uint64_t v = 0;
        if (extRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, int64_t* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);

    if ((addr + (szDst * sz) - 1) >= _totalCapacity) return 1;   // If we try read block outside, return error
    memset(dst, 0x00, szDst * sz);                               // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        int64_t v = 0;
        if (extRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// Read Buffer - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, float* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);

    if ((addr + (szDst * sz) - 1) >= _totalCapacity) return 1;   // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);                               // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        float v = 0;
        if (extRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

uint8_t LC_EXT_EEPROM::extRead(const uint32_t& addr, double* dst, const uint8_t& szDst) {
    uint8_t sz = sizeof(dst[0]);                                 // Get size of double variable, it may be 4 / 8

    if ((addr + (szDst * sz) - 1) >= _totalCapacity) return 1;   // If we try read block outside, return error
    memset(dst, 0xFF, szDst * sz);                               // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        double v = 0;
        if (extRead(addr + (i * sz), v) != 0) return 2;
        dst[i] = v;
    }
    return 0;
}

// ---------------------------------------------  Write Functions  ----------------------------------------------------

// Write - unsigned int 8bit, [ 0 - 255 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint8_t& src) {
    if (addr >= _totalCapacity) return 1;
    uint8_t z = 0;
    if (extRead(addr, z) != 0) return 2;
    if (z != src) {
        uint8_t wData[1] = { src };
        if (_write(addr, wData, 1) != 0) return 2;

        z = 0;
        if (extRead(addr, z) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write - signed int 8bit, [ -128 - +127 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const int8_t& src) {
    if (addr >= _totalCapacity) return 1;       // Check outmemory 
    
    int8_t z = 0;
    if (extRead(addr, z) != 0) return 2;        // Get data from memory
    if (z != src) {                             // Check data in memory and src 
        uint8_t v = (uint8_t)(src + 128);       // Transfer signed to unsigned data
        if (extWrite(addr, v) != 0) return 2;   // Write unsigned data to memory

        z = 0;
        if (extRead(addr, z) != 0) return 2;    // Get wrote data
        if (z != src) return 2;                 // Check wrote data and src
    }
    return 0;
}

// Write - char 8bit, [ 'A' / 'B' / another symbol ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const char& src) {
    if (addr >= _totalCapacity) return 1;       // Check outmemory 

    char z = '\0';
    if (extRead(addr, z) != 0) return 2;        // Get data from memory
    if (z != src) {                             // Check data in memory and src 
        uint8_t v = (uint8_t)(src + 128);       // Transfer char to unsigned data
        if (extWrite(addr, v) != 0) return 2;   // Write unsigned data to memory

        z = '\0';
        if (extRead(addr, z) != 0) return 2;    // Get wrote data
        if (z != src) return 2;                 // Check wrote data and src
    }
    return 0;
}

// Write - unsigned int 16bit, [ 0 - 65535 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint16_t& src) {
    uint8_t sz = sizeof(src);

    if ((addr + sz - 1) >= _totalCapacity) return 1;
    uint16_t z = 0x0000;
    if (extRead(addr, z) != 0) return 2;
    if (z != src) {
        uint8_t wData[sz] = {};
        for (uint8_t n = 0; n < sz; n++) 
            wData[n] = (uint8_t)(src >> (8 * (sz - 1 - n)));
        if (_write(addr, wData, sz) != 0) return 2;

        z = 0x0000;
        if (extRead(addr, z) != 0) return 2;            // Get wrote data
        if (z != src) return 2;                         // Check wrote data and src
    }
    return 0;
}

// Write - signed int 16bit, [ -32768 - +32767 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const int16_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= _totalCapacity) return 1;    // Check outmemory 

    int16_t z = 0x0000;
    if (extRead(addr, z) != 0) return 2;                // Get data from memory
    if (z != src) {                                     // Check data in memory and src 
        uint16_t v = (uint16_t)(src + 32768);           // Transfer signed to unsigned data
        if (extWrite(addr, v) != 0) return 2;           // Write unsigned data to memory
        
        z = 0x0000;
        if (extRead(addr, z) != 0) return 2;            // Get wrote data
        if (z != src) return 2;                         // Check wrote data and src
    }
    return 0;
}

// Write - unsigned int 32bit, [ 0 - 4294967295 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint32_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= _totalCapacity) return 1;

    uint32_t z = 0x00000000;
    if (extRead(addr, z) != 0) return 2;
    if (z != src) {
        uint8_t wData[sz] = {};
        for (uint8_t n = 0; n < sz; n++) 
            wData[n] = (uint8_t)(src >> (8 * (sz - 1 - n)));
        if (_write(addr, wData, sz) != 0) return 2;

        z = 0x00000000;
        if (extRead(addr, z) != 0) return 2;            // Get wrote data
        if (z != src) return 2;                         // Check wrote data and src
    }
    return 0;
}

// Write - signed int 32bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const int32_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= _totalCapacity) return 1;
    
    int32_t z = 0x00000000;
    if (extRead(addr, z) != 0) return 2;                // Get data from memory
    if (z != src) {                                     // Check data in memory and src 
        uint32_t v = (uint32_t)(src + 2147483648);      // Transfer signed to unsigned data
        if (extWrite(addr, v) != 0) return 2;           // Write unsigned data to memory

        z = 0x00000000;
        if (extRead(addr, z) != 0) return 2;            // Get wrote data
        if (z != src) return 2;                         // Check wrote data and src
    }
    return 0;
}

// Write - unsigned int 64bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint64_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= _totalCapacity) return 1;
    
    uint64_t z = 0x0000000000000000;
    if (extRead(addr, z) != 0) return 2;
    if (z != src) {
        uint8_t wData[sz] = {};
        for (uint8_t n = 0; n < sz; n++)
            wData[n] = (uint8_t)(src >> (8 * (sz - 1 - n)));
        if (_write(addr, wData, sz) != 0) return 2;

        z = 0x0000000000000000;
        if (extRead(addr, z) != 0) return 2;            // Get wrote data
        if (z != src) return 2;                         // Check wrote data and src
    }
    return 0;
}

// Write - signed int 64bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const int64_t& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= _totalCapacity) return 1;

    int64_t z = 0x0000000000000000;
    if (extRead(addr, z) != 0) return 2;                        // Get data from memory
    if (z != src) {                                             // Check data in memory and src 
        uint64_t v = (uint64_t)(src + 9223372036854775808UL);   // Transfer signed to unsigned data
        if (extWrite(addr, v) != 0) return 2;                   // Write unsigned data to memory

        z = 0x0000000000000000;
        if (extRead(addr, z) != 0) return 2;                    // Get wrote data
        if (z != src) return 2;                                 // Check wrote data and src
    }
    return 0;
}

// Write - float 32bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const float& src) {
    uint8_t sz = sizeof(src);

    if ((addr + sz - 1) >= _totalCapacity) return 1;

    float z = 0.0;
    if (extRead(addr, z) != 0) return 2;                        // Get data from memory
    if (z != src) {                                             // Check data in memory and src 
        uint32_t v = *(unsigned long*)&src;
        if (extWrite(addr, v) != 0) return 2;
        
        z = 0.0;
        if (extRead(addr, z) != 0) return 2;                    // Get wrote data
        if (z != src) return 2;                                 // Check wrote data and src
    }
    return 0;
}

// Write - double 64bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const double& src) {
    uint8_t sz = sizeof(src);
    if ((addr + sz - 1) >= _totalCapacity) return 1;

    double z = 0.0;
    if (extRead(addr, z) != 0) return 2;                        // Get data from memory
    if (z != src) {                                             // Check data in memory and src 
        if (sz == 4) {
            uint32_t v = *(unsigned long*)&src;
            if (extWrite(addr, v) != 0) return 2;
        };
        if (sz == 8) {
            uint64_t v = *(uint64_t*)&src;
            if (extWrite(addr, v) != 0) return 2;
        };
        z = 0.0;
        if (extRead(addr, z) != 0) return 2;                    // Get wrote data
        if (z != src) return 2;                                 // Check wrote data and src
    }

    return 0;
}

// Write - Bit 1 bit, [ 0 / 1 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint8_t& nBit, const bool& state) {
    if ((nBit > 7) || (addr >= _totalCapacity)) return 1;       // If we try read block outside, return error

    bool z = false;
    if (extRead(addr, nBit, z) != 0) return 2;                  // Get Info of Bit by address
    if (z != state) {
        uint8_t src = 0;
        if (extRead(addr, src) != 0) return 2;                  // Get Byte by address
        bitWrite(src, nBit, state);                             // Set Bit in Byte
        if (extWrite(addr, src) != 0) return 2;                 // Update The byte

        z = false;
        if (extRead(addr, nBit, z) != 0) return 2;              // Get Info of Bit by address
        if (z != state) return 2;                               // Compare info
    }
    return 0;
}

// Write - String
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const String& src) {
    if ((addr + src.length() - 1) >= _totalCapacity) return 1;

    String z = "";
    if (extRead(addr, z, src.length()) != 0) return 2;
    if (z != src) {
        //uint8_t wData[src.length() + 1];                // Maybe need set last element of array, like 0x00, lot end of char array
        uint8_t wData[src.length()];
        for (uint8_t i = 0; i < src.length(); i++) wData[i] = (uint8_t)src[i];        
        //wData[src.length()] = '\0';
        if (_write(addr, wData, src.length()) != 0) return 2;

        z = "";
        if (extRead(addr, z, src.length()) != 0) return 2;
        if (z != src) return 2;
    }
    return 0;
}

// Write Buffer - unsigned int 8 bit, [ 0 - 255 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint8_t* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= _totalCapacity) return 1;

    uint8_t buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc, 0xFF) != 0) return 2;              // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + i, src[i]) != 0) return 2;
        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - signed int 8 bit, [ -128 - +127 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const int8_t* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= _totalCapacity) return 1;

    int8_t buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc, 0x00) != 0) return 2;                      // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + i, (uint8_t)(src[i] + 128)) != 0) return 2; // if src = -128 then res = 0, if src = +127 then res = 255    
        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - char, [ -128 - +127 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const char* src, const uint8_t& szSrc) {
    if ((addr + szSrc - 1) >= _totalCapacity) return 1;
    
    char buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc, 0x00) != 0) return 2;                      // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++) {
            if (src[i] == 0x00) break;                                      // If write '\0' symbol, it is end of write char array
            if (extWrite(addr + i, (uint8_t)(src[i] + 128)) != 0) return 2; // if src = -128 then res = 0, if src = +127 then res = 255        
        }
        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - unsigned int 16 bit, [ 0 - 65535 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint16_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= _totalCapacity) return 1;

    uint16_t buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc * sz, 0xFF) != 0) return 2;                  // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + (i * sz), src[i]) != 0) return 2;
        
        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - signed int 16 bit, [ -32768 - +32767 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const int16_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= _totalCapacity) return 1;

    int16_t buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc * sz, 0x00) != 0) return 2;                          // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + (i * sz), (uint16_t)(src[i] + 32768)) != 0) return 2;   

        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - unsigned int 32 bit, [ 0 - 4294967295 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint32_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= _totalCapacity) return 1;

    uint32_t buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc * sz, 0xFF) != 0) return 2;                  // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + (i * sz), src[i]) != 0) return 2;

        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }

    return 0;
}

// Write Buffer - signed int 32 bit, [ -2147483648 - +2147483647 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const int32_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= _totalCapacity) return 1;

    int32_t buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc * sz, 0x00) != 0) return 2;                          // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + (i * sz), (uint32_t)(src[i] + 2147483648)) != 0) return 2;

        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }

    return 0;
}

// Write Buffer - unsigned int 64 bit, [ 0 - 0xFFFFFFFFFFFFFFFF ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const uint64_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= _totalCapacity) return 1;

    uint64_t buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc * sz, 0xFF) != 0) return 2;                  // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + (i * sz), src[i]) != 0) return 2;

        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - signed int 64 bit, [ -9,223,372,036,854,775,808 - +9,223,372,036,854,775,807 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const int64_t* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= _totalCapacity) return 1;

    int64_t buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc * sz, 0x00) != 0) return 2;                          // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + (i * sz), (uint64_t)(src[i] + 9223372036854775808UL)) != 0) return 2;

        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - float 32 bit, [ -3.4E+38 - +3.4E+38 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const float* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= _totalCapacity) return 1;

    float buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc * sz, 0xFF) != 0) return 2;                  // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + (i * sz), src[i]) != 0) return 2;

        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Write Buffer - double 64 bit, [ -1.7E+308 - +1.7E+308 ]
uint8_t LC_EXT_EEPROM::extWrite(const uint32_t& addr, const double* src, const uint8_t& szSrc) {
    uint8_t sz = sizeof(src[0]);
    if ((addr + (szSrc * sz) - 1) >= _totalCapacity) return 1;

    double buff[szSrc] = {};
    if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
    if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) {
        if (extFill(addr, szSrc * sz, 0xFF) != 0) return 2;                  // Clear value EEPROM by default value
        for (uint8_t i = 0; i < szSrc; i++)
            if (extWrite(addr + (i * sz), src[i]) != 0) return 2;

        buff[szSrc] = {};
        if (extRead(addr, buff, sizeof(buff)) != 0) return 2;
        if (!_cmpBuffers(src, szSrc, buff, sizeof(buff))) return 2;
    }
    return 0;
}

// Fill block memory by default value
uint8_t LC_EXT_EEPROM::extFill(const uint32_t& addr, const uint32_t& cnt, const uint8_t& src) {
    if ((addr + cnt - 1) >= _totalCapacity) return 1;
    for (uint32_t i = 0; i < cnt; i++)
        if (extWrite(addr + i, src) != 0) return 2;
    return 0;
}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EXT_EEPROM::extShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan) {
    uint32_t nAddrTo = (addrTo == 0x0000) ? _totalCapacity - 1 : addrTo;

    if (addrFrom >= nAddrTo) {
        Serial.print("Error: Incorrect memory range!");
    }
    else {
        Serial.print("External EEPROM memory...");

        // Header of table
        Serial.print(F("\nI2C  Address  "));
        for (uint8_t i = 0; i < quan; i++) {
            String tmpStr = _preFix(String(i, HEX), 2, '0') + ' ';
            tmpStr.toUpperCase();
            Serial.print(tmpStr);
        }
        Serial.println();
        for (uint32_t j = (addrFrom / quan); j < (nAddrTo / quan + 1); j++) {
            String cByte = F("0x");
            cByte += String(_getCtrlByte(j * quan), HEX) + ' ';
            String tSt = _preFix(String(j * quan, HEX), 8, '0') + ' ';
            tSt.toUpperCase();
            tSt = cByte + tSt;
            Serial.print(tSt);

            uint8_t res[quan];
            String tmpStr = "";
            for (uint8_t i = 0; i < quan; i++) {
                uint8_t bt = 0;
                extRead((j * quan) + i, bt);
                tmpStr += _preFix(String(bt, HEX), 2, '0') + ' ';
            }
            tmpStr.toUpperCase();
            Serial.println(tmpStr);
        }
    }
}

LC_EXT_EEPROM::~LC_EXT_EEPROM() {}

// ------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------

uint8_t LC_EXT_EEPROM::_checkQDevices() {
    uint8_t n = 0;
    for (uint8_t i = 0; i < 8; i++) {
        Wire.beginTransmission(I2CADDR + i);
        if (Wire.endTransmission() > 0) break;
        n++;
    }
    return n;
}

void LC_EXT_EEPROM::_setTotalCapacity() {
    _totalCapacity = _qDevice * _devCapacity * 1024UL / 8;
}

// Returns control byte of module by address, 0x50 / 0x51 /0x52 / ... /0x57
uint8_t LC_EXT_EEPROM::_getCtrlByte(const uint32_t& addr) {
    return I2CADDR | (uint8_t)((addr / 1024.0) / (_devCapacity / 8.0));
};

void LC_EXT_EEPROM::_sendAddr(const uint32_t& addr) {
    if (_devCapacity > 1024) Wire.write(static_cast<uint8_t>(addr >> 16)); // write very high address byte
    if (_devCapacity > 16)   Wire.write(static_cast<uint8_t>(addr >> 8));  // write high address byte
    Wire.write(static_cast<uint8_t>(addr));                                // write low  address byte
}

uint8_t LC_EXT_EEPROM::_readWire(uint32_t addr, uint8_t* wData, uint16_t qBytes) {
    uint8_t ctrlByte = _getCtrlByte(addr);                  // Control byte 0x50 .... 0x57
    
    Wire.beginTransmission(ctrlByte);                       
    _sendAddr(addr);
    Wire.endTransmission();

    Wire.requestFrom((uint8_t) ctrlByte, (uint16_t) qBytes); // In Wire library present two functions, (uint8_t) for disable attentions

    uint16_t cnt = 0;
    while(cnt < qBytes) {
        if (!Wire.available()) break;                       // Emergency exit from loop, if Wire is not available
        wData[cnt] = Wire.read();                           // While write data is available, we are transfer it to wData buffer
        cnt++;
    }

    return cnt; // Returns the number of bytes received, or zero if nothing is received. Zero can be perceived as an error.
}

// It's a new version of method _write
uint8_t LC_EXT_EEPROM::_writeWire(uint32_t addr, uint8_t* wData, uint16_t qBytes) {
}

uint8_t LC_EXT_EEPROM::_write(uint32_t addr, uint8_t* src, uint16_t szSrc) {
    if ((addr + szSrc - 1) >= _totalCapacity) return 1;
        
    while (szSrc > 0) {
        uint16_t nPage = _pSize - (addr & (_pSize - 1));              // Part of page size for write data from address
        uint16_t nWrite = (szSrc < nPage) ? szSrc : nPage;            // Quantity of Bytes that you cat write to page
        uint8_t  ctrlByte = _getCtrlByte(addr);

        Wire.beginTransmission(ctrlByte);
        _sendAddr(addr);
        Wire.write(src, nWrite);
        uint8_t txState = Wire.endTransmission();
        if (txState != 0) return 2;

        // wait up to 5ms for the write to complete, Wait 5ms max!!!
        for (uint8_t i = 0; i < 10; i++) {
            delayMicroseconds(500);
            Wire.beginTransmission(ctrlByte);
            _sendAddr(0);
            txState = Wire.endTransmission();
            if (txState == 0) break;
        }
        if (txState != 0) return 2;

        addr  += nWrite;         // increment the EEPROM address
        src   += nWrite;        // increment the input data pointer
        szSrc -= nWrite;       // decrement the number of bytes left to write
    };

    return 0;
}
