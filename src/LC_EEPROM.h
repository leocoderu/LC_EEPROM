#ifndef LC_EEPROM_H
#define LC_EEPROM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <EEPROM.h>
#include <Wire.h> 

class LC_EEPROM{

public:

	LC_EEPROM();

    // Function for Internal EEPROM memory
    uint8_t     intReadByte(const uint32_t& addr);
    void        intWriteByte(const uint32_t& addr, const uint8_t& wByte);
    uint16_t    intReadInt(const uint32_t& addr);
    void        intWriteInt(const uint32_t& addr, const uint16_t& wInt);
    uint32_t    intReadLong(const uint32_t& addr);
    void        intWriteLong(const uint32_t& addr, const uint32_t& wDouble);
    String      intReadStr(const uint32_t& addr, const uint16_t& quan);
    void        intWriteStr(const uint32_t& addr, const String& sendStr);
    void        intFillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt);
    void        intReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst);
    void        intWriteBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc);
    void        intShow(const uint32_t& addrFrom = 0x0000, const uint32_t& addrTo = EEPROM.length() - 1, const uint8_t& quan = 32);

	~LC_EEPROM();

protected:
    String _preFix(String str, uint8_t quan, char chr);
};


class LC_EXT_EEPROM : public LC_EEPROM {

    uint32_t _devCapacity;      // capacity of one EEPROM device, in kbits
    uint8_t  _eepromAddr;       // eeprom i2c address
    uint8_t  _qDevice;          // number of devices on the bus
    uint16_t _pSize;            // page size in bytes
    uint32_t _totalCapacity;    // capacity of all EEPROM devices on the bus, in bytes
    uint8_t _nAddrBytes;        // number of address bytes (1 or 2)
    uint8_t  _csShift;          // number of bits to shift address for chip select bits in control byte

public:
    // EEPROM size in kilobits.
    enum eeprom_size_t {
        kbits_2 = 2,
        kbits_4 = 4,
        kbits_8 = 8,
        kbits_16 = 16,
        kbits_32 = 32,
        kbits_64 = 64,
        kbits_128 = 128,
        kbits_256 = 256,
        kbits_512 = 512,
        kbits_1024 = 1024,
        kbits_2048 = 2048
    };
    // I2C clock frequency
    enum twiFreq_t {
        lowSpeed = 100000,     // 100kHz
        highSpeed = 400000     // 400kHz
    };
    // EEPROM addressing error, returned by write() or read() if upper address bound is exceeded
    static const uint8_t EEPROM_ADDR_ERR{ 9 };

    // CONSTRUCTOR
    LC_EXT_EEPROM(const eeprom_size_t& devCapacity, const uint8_t& qDevice,
        const uint16_t& pSize, const uint8_t& eeAddr = 0x50);

    uint8_t     begin(twiFreq_t twiFreq = lowSpeed);

    // Function for External EEPROM memory
    uint8_t     extReadByte(const uint32_t& addr);
    void        extWriteByte(const uint32_t& addr, const uint8_t& wByte);
    uint16_t    extReadInt(const uint32_t& addr);
    void        extWriteInt(const uint32_t& addr, const uint16_t& wInt);
    uint32_t    extReadLong(const uint32_t& addr);
    void        extWriteLong(const uint32_t& addr, const uint32_t& wDouble);
    String      extReadStr(const uint32_t& addr, const uint16_t& quan);
    void        extWriteStr(const uint32_t& addr, const String& sendStr);
    void        extFillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt);
    void        extReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst);
    void        extWriteBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc);

    void        extShow(const uint32_t& addrFrom = 0x0000, const uint32_t& addrTo = 0xFFFF, const uint8_t& quan = 32);

	~LC_EXT_EEPROM();

private:
    void _writeAddr(const uint32_t& addr, const uint8_t wByte = NULL);
};

#endif	/* LC_EEPROM_H */