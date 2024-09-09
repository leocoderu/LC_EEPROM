#ifndef LEOCODE_EEPROM_H
#define LEOCODE_EEPROM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <EEPROM.h>

#include <Wire.h>

class LC_EEPROM {
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
            lowSpeed  = 100000,     // 100kHz
            highSpeed = 400000,     // 400kHz
            maxSpeed = 1000000      // 1000kHz <- Doesn't work correctly :(
        };
        // EEPROM addressing error, returned by write() or read() if upper address bound is exceeded
        static const uint8_t EEPROM_ADDR_ERR {9};

        // CONSTRUCTOR
        LC_EEPROM(const eeprom_size_t& devCapacity, const uint8_t& qDevice); //, const uint8_t& pSize);

        uint8_t begin(twiFreq_t twiFreq = highSpeed);
        
        // Function for External EEPROM memory
        //template <typename T>
        //T read(const uint32_t& addr);

        uint32_t getTotalCapacity();
        uint8_t  getCtrlByte(const uint32_t& addr);

        uint8_t  readByte(const uint32_t& addr);
        uint16_t readInt(const uint32_t& addr);
        uint32_t readLong(const uint32_t& addr);
        String   readStr(const uint32_t& addr, const int16_t& quan);
        void     readBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst);

        void writeByte(const uint32_t& addr, const uint8_t& wByte);
        void writeInt(const uint32_t& addr, const uint16_t& wInt);
        void writeLong(const uint32_t& addr, const uint32_t& wDouble);
        void writeStr(const uint32_t& addr, const String& sendStr);
        void fillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt);
        void writeBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc);

        // Function for Internal EEPROM memory
        uint8_t iReadByte(const uint32_t& addr);
        void iWriteByte(const uint32_t& addr, const uint8_t& wByte);
        uint16_t iReadInt(const uint32_t& addr);
        void iWriteInt(const uint32_t& addr, const uint16_t& wInt);
        uint32_t iReadLong(const uint32_t& addr);
        void iWriteLong(const uint32_t& addr, const uint32_t& wDouble);
        String iReadStr(const uint32_t& addr, const uint16_t& quan);
        void iWriteStr(const uint32_t& addr, const String& sendStr);
        void iFillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt);
        void iReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst);
        void iWriteBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc);

        void show(const uint32_t& addrFrom = 0x0000, const uint32_t& addrTo = 0xFFFF, const uint8_t& quan = 32);
        void iShow(const uint32_t& addrFrom = 0x0000, const uint32_t& addrTo = EEPROM.length() - 1, const uint8_t& quan = 32);

        ~LC_EEPROM();

    private:
        void    _sendAddr(const uint32_t& addr);
        uint8_t _write(uint32_t addr, uint8_t* wData, uint16_t qBytes = 1);
        String  _preFix(String str, byte quan, char chr);

        uint32_t _devCapacity;      // capacity of one EEPROM device, in kbits
        const uint8_t  _eepromAddr = 0x50;// eeprom i2c address
        uint8_t  _qDevice;          // number of devices on the bus
        const uint8_t  _pSize = 30; // page size in bytes, because buffer Wire is 32 bytes!!!, 2-bytes address & 30-Data
        uint32_t _totalCapacity;    // capacity of all EEPROM devices on the bus, in bytes

        /*static uint8_t rxBufferIndex;
        static uint8_t rxBufferLength;
        static uint8_t txBufferIndex;
        static uint8_t txBufferLength;
        void _begin();*/
};

#endif	/* LEOCODE_EEPROM_H */
