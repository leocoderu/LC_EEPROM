#ifndef LC_EEPROM_H
#define LC_EEPROM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <EEPROM.h>
#include <Wire.h> 

#define I2CADDR 0x50    // eeprom i2c address

// EEPROM size in kilobits.
enum eeprom_model_t {
//  Model          Size        Frequency   Page (Bytes)
    _24C00,      // 128bit      400kHz      8
    _24AA00,     // 128bit      400kHz      8
    _24LC00,     // 128bit      400kHz      8

    _24C01,      // 1kBit       100kHz      8
    _24AA01,     // 1kBit       400kHz      8
    _24LC01,     // 1kBit       400kHz      8
    _24FC01,     // 1kBit       1000kHz     8
    
    _24C02,      // 2kBit       100kHz      8
    _24AA02,     // 2kBit       400kHz      8
    _24LC02,     // 2kBit       400kHz      8
    _24FC02,     // 2kBit       1000kHz     8
    
    _24C04,      // 4kBit       100kHz      16 
    _24W04,      // 4kBit       100kHz      16 
    _24AA04,     // 4kBit       400kHz      16 
    _24LC04,     // 4kBit       400kHz      16 
    _24FC04,     // 4kBit       1000kHz     16
        
    _24C08,      // 8kBit       1000kHz     16
    _24AA08,     // 8kBit       400kHz      16
    _24LC08,     // 8kBit       400kHz      16
    _24FC08,     // 8kBit       1000kHz     16
    
    _24C16,      // 16kBit      1000kHz     16
    _24AA16,     // 16kBit      400kHz      16
    _24LC16,     // 16kBit      400kHz      16
    _24FC16,     // 16kBit      1000kHz     16
    
    _24C32,      // 32kBit      400kHz      8
    _24AA32,     // 32kBit      400kHz      8
    _24LC32,     // 32kBit      400kHz      32
    _24FC32,     // 32kBit      1000kHz     8

    _24AA64,     // 64kBit      400kHz      32
    _24LC64,     // 64kBit      400kHz      32
    _24FC64,     // 64kBit      1000kHz     32

    _24AA128,    // 128kBit     400kHz      64
    _24LC128,    // 128kBit     400kHz      64
    _24FC128,    // 128kBit     1000kHz     64

    _24AA256,    // 256kBit     400kHz      64
    _24LC256,    // 256kBit     400kHz      64
    _24FC256,    // 256kBit     1000kHz     64

    _24AA512,    // 512kBit     400kHz      128
    _24LC512,    // 512kBit     400kHz      128
    _24FC512,    // 512kBit     1000kHz     128

    _24AA1025,   // 1024kBit    400kHz      128  
    _24LC1025,   // 1024kBit    400kHz      128
    _24FC1025    // 1024kBit    1000kHz     128    
};

    class LC_EEPROM {

    public:
        // CONSTRUCTOR
        LC_EEPROM();

        uint32_t intTotalCapacity();
        void     info();
        
        // Functions for working with internal EEPROM memory
        uint8_t  intRead(const uint32_t& addr, uint8_t& dst);
        uint8_t  intRead(const uint32_t& addr, int8_t& dst);
        uint8_t  intRead(const uint32_t& addr, uint16_t& dst);
        uint8_t  intRead(const uint32_t& addr, int16_t& dst);
        uint8_t  intRead(const uint32_t& addr, uint32_t& dst);
        uint8_t  intRead(const uint32_t& addr, int32_t& dst);
        uint8_t  intRead(const uint32_t& addr, String& dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, uint8_t* dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, int8_t* dst, const uint8_t& szDst);        
        uint8_t  intRead(const uint32_t& addr, char* dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, uint16_t* dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, int16_t* dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, int* dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, uint32_t* dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, int32_t* dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, double* dst, const uint8_t& szDst);
        uint8_t  intRead(const uint32_t& addr, float* dst, const uint8_t& szDst);

        uint8_t  intWrite(const uint32_t& addr, const uint8_t& src);
        uint8_t  intWrite(const uint32_t& addr, const int8_t& src);
        uint8_t  intWrite(const uint32_t& addr, const uint16_t& src);
        uint8_t  intWrite(const uint32_t& addr, const int16_t& src);
        uint8_t  intWrite(const uint32_t& addr, const uint32_t& src);
        uint8_t  intWrite(const uint32_t& addr, const int32_t& src);
        uint8_t  intWrite(const uint32_t& addr, const String& src);
        uint8_t  intFill (const uint32_t& addr, const uint32_t& cnt, const uint8_t& src);
        uint8_t  intWrite(const uint32_t& addr, const uint8_t* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const int8_t* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const char* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const uint16_t* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const int16_t* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const int* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const uint32_t* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const int32_t* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const double* src, const uint8_t& szSrc);
        uint8_t  intWrite(const uint32_t& addr, const float* src, const uint8_t& szSrc);

        void     intShow(const uint32_t& addrFrom = 0x0000, const uint32_t& addrTo = EEPROM.length() - 1, const uint8_t& quan = 32);
        void     outBuffer(int8_t* src, const uint8_t& sz);
        void     outBuffer(uint8_t* src, const uint8_t& sz);
        void     outBuffer(char* src, const uint8_t& sz);

        ~LC_EEPROM();

    protected:
        String   _preFix(String str, uint8_t quan, char chr);
    
        template <typename T>
        bool     _cmpBuffers(const T* src, const uint8_t& szSrc, const T* dst, const uint8_t& szDst) {
            if (szSrc != szDst) return false;
            for (uint8_t i = 0; i < szSrc; i++)
                if (src[i] != dst[i]) return false;
            return true;
        }

    private:
        // Some private function will be here
    };

    class LC_EXT_EEPROM : public LC_EEPROM {

        uint32_t _totalCapacity;    // capacity of all EEPROM devices on the bus, in bytes
        uint16_t _devCapacity;      // capacity of one EEPROM device, in kbits
        uint8_t  _qDevice;          // number of devices on the bus
        uint8_t  _pSize;            // page size in bytes, because buffer Wire is 32 bytes!!!, 2-bytes address & 30-Data //TODO: Test with 64 and 128
        uint8_t  _twiFreq;          // Frequency in 10th kHz like (1, 4, 10)

    public:
        // CONSTRUCTOR
        LC_EXT_EEPROM(const eeprom_model_t& devModel);

        uint8_t  begin();
        uint32_t extTotalCapacity();
        bool     exists();
        void     info();

        // Getters & Setters
        uint16_t getCapacity();
        void     setCapacity(uint16_t capacity);
        uint8_t  getQDevices();
        void     setQDevices(uint8_t qDevices);
        uint8_t  getPageSize();
        void     setPageSize(uint8_t pSize);
        uint8_t  getFrequency();
        void     setFrequency(uint8_t freq);

        // Functions for working with external EEPROM memory
        uint8_t  extRead(const uint32_t& addr, uint8_t& dst);
        uint8_t  extRead(const uint32_t& addr, uint16_t& dst);
        uint8_t  extRead(const uint32_t& addr, uint32_t& dst);
        uint8_t  extRead(const uint32_t& addr, String& dst, const uint8_t& quan);
        uint8_t  extRead(const uint32_t& addr, uint8_t* dst, const uint8_t& szDst);
        uint8_t  extRead(const uint32_t& addr, int8_t* dst, const uint8_t& szDst);
        uint8_t  extRead(const uint32_t& addr, char* dst, const uint8_t& szDst);

        uint8_t  extWrite(const uint32_t& addr, const uint8_t& src);
        uint8_t  extWrite(const uint32_t& addr, const uint16_t& src);
        uint8_t  extWrite(const uint32_t& addr, const uint32_t& src);
        uint8_t  extWrite(const uint32_t& addr, const String& src);
        uint8_t  extFill (const uint32_t& addr, const uint32_t& cnt, const uint8_t& src);
        uint8_t  extWrite(const uint32_t& addr, const uint8_t* src, const uint8_t& szSrc);
        uint8_t  extWrite(const uint32_t& addr, const int8_t* src, const uint8_t& szSrc);
        uint8_t  extWrite(const uint32_t& addr, const char* src, const uint8_t& szSrc);

        void     extShow(const uint32_t& addrFrom = 0x0000, const uint32_t& addrTo = 0x0000, const uint8_t& quan = 32); 

        ~LC_EXT_EEPROM();

    private:
        void    _getModelInfo(const eeprom_model_t& devModel);
        uint8_t _checkQDevices();
        void    _setTotalCapacity();
        uint8_t _getCtrlByte(const uint32_t& addr);
        void    _sendAddr(const uint32_t& addr);
        uint8_t _readWire(uint32_t addr, uint8_t* wData, uint16_t qBytes);
        uint8_t _writeWire(uint32_t addr, uint8_t* wData, uint16_t qBytes);  // <- It's a new version method of _write
        uint8_t _write(uint32_t addr, uint8_t* src, uint16_t szSrc = 1);
    };


#endif	/* LC_EEPROM_H */