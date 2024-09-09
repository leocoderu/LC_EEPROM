#include "LC_EEPROM.h"

//using namespace LC_EXT_EEPROM;

//LC_EXT_EEPROM::LC_EXT_EEPROM(const eeprom_size_t& devCapacity, const uint8_t& qDevice){; //, const uint8_t& pSize);
//    _devCapacity = devCapacity;
//    //_eepromAddr = eeAddr;
//    _qDevice = qDevice;
//    //_pSize = pSize;
//    _totalCapacity = _qDevice * _devCapacity * 1024UL / 8;
//    //_nAddrBytes = _devCapacity > kbits_16 ? 2 : 1;              // two address bytes needed for eeproms > 16kbits
//
//    // determine the bitshift needed to isolate the chip select bits from
//    // the address to put into the control byte
//   /* uint16_t kb = _devCapacity;
//    if (kb <= kbits_16) _csShift = 8;
//    else if (kb >= kbits_512) _csShift = 16;
//    else {
//        kb >>= 6;
//        _csShift = 12;
//        while (kb >= 1) {
//            ++_csShift;
//            kb >>= 1;
//        }
//    }*/
//}

LC_EXT_EEPROM::LC_EXT_EEPROM(const eeprom_model_t& devModel, const uint8_t& qDevice)
{
    _qDevice = qDevice;
 
    switch (devModel)
    {
    case _24C01:
        _devCapacity = 1;
        _twiFreq = 1; // 100000
        _pSize = 8;
        break;
    case _24AA01:
        _devCapacity = 1;
        _twiFreq = 4; // 400000
        _pSize = 8;
        break;
    case _24LC01:
        _devCapacity = 1;
        _twiFreq = 4; // 400000
        _pSize = 8;
        break;
    case _24FC01:
        _devCapacity = 1;
        _twiFreq = 10; // 1000000
        _pSize = 8;
        break;
    case _24C02:
        break;
    case _24AA02:
        break;
    case _24LC02:
        break;
    case _24FC02:
        break;
    case _24C04:
        break;
    case _24W04:
        break;
    case _24AA04:
        break;
    case _24LC04:
        break;
    case _24FC04:
        break;
    case _24C08:
        break;
    case _24AA08:
        break;
    case _24LC08:
        break;
    case _24FC08:
        break;
    case _24C16:
        break;
    case _24AA16:
        break;
    case _24LC16:
        break;
    case _24FC16:
        break;
    case _24C32:
        break;
    case _24AA32:
        break;
    case _24LC32:
        break;
    case _24FC32:
        break;
    case _24AA64:
        break;
    case _24LC64:
        break;
    case _24FC64:
        break;
    case _24AA128:
        break;
    case _24LC128:
        break;
    case _24FC128:
        break;
    case _24AA256:
        break;
    case _24LC256:
        break;
    case _24FC256:
        break;
    case _24AA512:
        _devCapacity = 512;
        _twiFreq = 4; // 400000
        _pSize = 128;
        break;
    case _24LC512:
        _devCapacity = 512;
        _twiFreq = 4; // 400000
        _pSize = 128;
        break;
    case _24FC512:
        _devCapacity = 512;
        _twiFreq = 10; // 1000000
        _pSize = 128;
        break;
    case _24AA1025:
        break;
    case _24LC1025:
        break;
    case _24FC1025:
        break;
    default:
        break;
    }

    _totalCapacity = _qDevice * _devCapacity * 1024UL / 8;
}

uint8_t LC_EXT_EEPROM::begin() {
    Wire.begin();
    Wire.setClock(_twiFreq * 100000);
    Wire.beginTransmission(_eepromAddr);
    _sendAddr(0x0000);
    return Wire.endTransmission();
}

// Returns total Capacity of memory
uint32_t LC_EXT_EEPROM::getTotalCapacity() {
    return _totalCapacity;
};

// Returns control byte of module by address, 0x50 / 0x51 /0x52 / ... /0x57
uint8_t LC_EXT_EEPROM::getCtrlByte(const uint32_t& addr) {
    return _eepromAddr | (uint8_t)((addr / 1024.0) / (_devCapacity / 8.0));
};

// Returns value of byte by address
uint8_t LC_EXT_EEPROM::extReadByte(const uint32_t& addr) {
    if (addr > _totalCapacity) return 0;

    uint8_t ctrlByte = getCtrlByte(addr);
    Wire.beginTransmission(ctrlByte);                       // Control byte 0x50 .... 0x57
    LC_EXT_EEPROM::_sendAddr(addr);
    Wire.endTransmission();

    Wire.requestFrom((uint8_t)ctrlByte, (uint8_t)1);        // In Wire library present two functions, (uint8_t) for disable attentions
    return (Wire.available()) ? Wire.read() : 0;

}

// Returns Integer value (2 bytes) by address
uint16_t LC_EXT_EEPROM::extReadInt(const uint32_t& addr) {
    if (addr + 1 > _totalCapacity) return 0;
    return extReadByte(addr) << 8 | extReadByte(addr + 1);
}

// Returns Longint value (4 bytes) by address
uint32_t LC_EXT_EEPROM::extReadLong(const uint32_t& addr) {
    if (addr + 3 > _totalCapacity) return 0;
    return ((uint32_t)extReadByte(addr) << 24) | ((uint32_t)extReadByte(addr + 1) << 16) | ((uint32_t)extReadByte(addr + 2) << 8) | (uint32_t)extReadByte(addr + 3);
}

// Returns String value by address
String LC_EXT_EEPROM::extReadStr(const uint32_t& addr, const uint16_t& quan) {
    if ((addr + quan) > _totalCapacity) return "";

    String res = "";
    for (uint16_t i = 0; i < quan; i++)
        res += char(extReadByte(addr + i));
    return res;
}

// Returns Block data by address with unsigned byte array
uint8_t LC_EXT_EEPROM::extReadBlock(const uint32_t& addr, const uint8_t& defVal, uint8_t* dst, const uint8_t& szDst) {
    if ((addr + szDst) > _totalCapacity) return 1; // If we try read block outside, return error
    memset(dst, defVal, szDst);                 // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = extReadByte(addr + i);
        dst[i] = bt;
    }
    return 0;
}

// Returns Block data by address with signed byte array
uint8_t LC_EXT_EEPROM::extReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst) {
    if ((addr + szDst) > _totalCapacity) return 1; // If we try read block outside, return error
    memset(dst, defVal, szDst);                 // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = extReadByte(addr + i);
        if (bt == 0xFF) break; // TODO: needs to change 0xFF or not
        dst[i] = bt;
    }   // perhaps value 0xFF need to change by defVal, but if read block to char array needs the end by 0
    return 0;
}

// Write byte by address
uint8_t LC_EXT_EEPROM::extWriteByte(const uint32_t& addr, const uint8_t& wByte) {
    if (addr > _totalCapacity) return 1;
    if (extReadByte(addr) != wByte) {
        uint8_t wData[1] = { wByte };
        _write(addr, wData, 1);
    }
    return 0;
}

// Write Integer value by address
uint8_t LC_EXT_EEPROM::extWriteInt(const uint32_t& addr, const uint16_t& wInt) {
    if ((addr + 1) > _totalCapacity) return 1;
    if (extReadInt(addr) != wInt) {
        uint8_t wData[2] = { wInt >> 8, (uint8_t)wInt };
        _write(addr, wData, 2);
    }
}

// Write Longint value by address
uint8_t LC_EXT_EEPROM::extWriteLong(const uint32_t& addr, const uint32_t& wLong) {
    if ((addr + 3) > _totalCapacity) return 1;
    if (extReadLong(addr) != wLong) {
        uint8_t wData[4] = { (uint8_t)(wLong >> 24), (uint8_t)(wLong >> 16), (uint8_t)(wLong >> 8), (uint8_t)wLong };
        _write(addr, wData, 4);
    }
    return 0;
}

// Write string by address
uint8_t LC_EXT_EEPROM::extWriteStr(const uint32_t& addr, const String& sendStr){   // TODO: ATTATION! First of all you need to check sendStr.length(), probably it returns address of variable but not length of strings content
    if ((addr + sendStr.length()) > _totalCapacity) return 1;
    if (extReadStr(addr, sendStr.length()) != sendStr) {
        uint8_t wData[sendStr.length() + 1];                // Maybe need set last element of array, like 0x00, lot end of char array
        sendStr.toCharArray(wData, sizeof(wData));
        _write(addr, wData, sendStr.length());
    }
    return 0;
}

// Fill block memory by default value
uint8_t LC_EXT_EEPROM::extFillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt) {
    if ((addr + cnt) > _totalCapacity) return 1;
    for (uint32_t i = 0; i < cnt; i++) extWriteByte(addr + i, bt);
    return 0;
}

// Write Block data by address
uint8_t LC_EXT_EEPROM::extWriteBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc) {
    if ((addr + szSrc) > _totalCapacity) return 1;
    extFillBlock(addr, szSrc, defVal);      // Clear valu EEPROM by default value
    for (uint8_t i = 0; i < szSrc; i++)
        if (((defVal == 0xFF) && (src[i] == INT8_MIN)) || ((defVal == 0x00) && (src[i] == 0x00))) break;
        else extWriteByte(addr + i, src[i]);
    return 0;
}

//// Write Block data by address
//void LC_EXT_EEPROM::extWriteBlock(const uint32_t& addr, const int8_t& defVal, const int8_t* src, const uint8_t& szSrc) {
//    extFillBlock(addr, szSrc, defVal);
//    for (uint8_t i = 0; i < szSrc; i++)
//        if (((defVal == 0xFF) && (src[i] == INT8_MIN)) || ((defVal == 0x00) && (src[i] == 0x00))) break;
//        else extWriteByte(addr + i, src[i]);
//}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EXT_EEPROM::extShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan) {
    Serial.print("External EEPROM memory...");

    // Header of table
    Serial.print(F("\nI2C  Address  "));
    for (uint8_t i = 0; i < quan; i++) {
        String tmpStr = LC_EEPROM::_preFix(String(i, HEX), 2, '0') + ' ';
        tmpStr.toUpperCase();
        Serial.print(tmpStr);
    }
    Serial.println();
    for (uint32_t j = (addrFrom / quan); j < (addrTo / quan + 1); j++) {
        String cByte = F("0x");
        cByte += String(getCtrlByte(j * quan), HEX) + ' ';
        String tSt = LC_EEPROM::_preFix(String(j * quan, HEX), 8, '0') + ' ';
        tSt.toUpperCase();
        tSt = cByte + tSt;
        Serial.print(tSt);

        uint8_t res[quan];
        String tmpStr = "";
        for (uint8_t i = 0; i < quan; i++)
            tmpStr += LC_EEPROM::_preFix(String(extReadByte((j * quan) + i), HEX), 2, '0') + ' ';
        tmpStr.toUpperCase();
        Serial.println(tmpStr);
    }
}

LC_EXT_EEPROM::~LC_EXT_EEPROM() {}

// ------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------

void LC_EXT_EEPROM::_sendAddr(const uint32_t& addr) {
    uint8_t nAddrBytes = _devCapacity > kbits_16 ? 2 : 1;               // For module with 1024bit & 2048 may be 3 bytes for adress
    if (nAddrBytes == 2) Wire.write(static_cast<uint8_t>(addr >> 8));   // write high address byte
    Wire.write(static_cast<uint8_t>(addr));                             // write low  address byte
}

uint8_t LC_EXT_EEPROM::_write(uint32_t addr, uint8_t* wData, uint16_t qBytes) {
    if (addr + qBytes > _totalCapacity) return EEPROM_ADDR_ERR;

    uint8_t txState = 0;
    while (qBytes > 0) {
        uint16_t nPage = _pSize - (addr & (_pSize - 1));                // Part of page size for write data from address
        uint16_t nWrite = (qBytes < nPage) ? qBytes : nPage;            // Quantity of Bytes that you cat write to page
        //uint8_t  ctrlByte = _eepromAddr | (uint8_t)((addr / 1024.0) / (_devCapacity / 8.0));
        uint8_t  ctrlByte = getCtrlByte(addr);

        Wire.beginTransmission(ctrlByte);
        LC_EXT_EEPROM::_sendAddr(addr);
        Wire.write(wData, nWrite);
        txState = Wire.endTransmission();
        if (txState != 0) return txState;

        // wait up to 5ms for the write to complete, Wait 5ms max!!!
        for (uint8_t i = 0; i < 10; i++) {
            delayMicroseconds(500);
            Wire.beginTransmission(ctrlByte);
            LC_EXT_EEPROM::_sendAddr(0);
            txState = Wire.endTransmission();
            if (txState == 0) break;
        }
        if (txState != 0) return txState;

        addr += nWrite;         // increment the EEPROM address
        wData += nWrite;         // increment the input data pointer
        qBytes -= nWrite;         // decrement the number of bytes left to write
    };
    return txState;
}



/*template <typename T>
T LC_EEPROM::read(const uint32_t& addr, typeof T){
    uint8_t getVal[sizeof(T)];
    uint8_t res = LC_EEPROM::_read(addr, getVal, sizeof(T));
    if (res != 0) return 0;

    //T value = 0;
    for(uint8_t i = 0; i < sizeof(T); i++)
        value = value << 8 | getVal[i];

    return value;
}*/

/*uint8_t LC_EEPROM::readByte(const uint32_t& addr) {
    uint8_t _getVal[1];
    uint8_t _res = LC_EEPROM::_read(addr, _getVal, sizeof(_getVal));

    uint32_t _result = 0;
    for(uint8_t i = 0; i < sizeof(uint8_t); i++)
        _result += (_getVal[i] << 8);

    return (_res == 0) ? _getVal[0] : 0;
}*/

/*uint8_t LC_EEPROM::_read(uint32_t addr, uint8_t* values, uint8_t qBytes = 1){
    if (addr + qBytes > _totalCapacity) return EEPROM_ADDR_ERR;

    uint8_t rState = 0;
    do {
        uint8_t ctrlByte = _eepromAddr | (uint8_t)((addr / 1024.0) / (_devCapacity / 8.0));
        Wire.beginTransmission(ctrlByte);                                      // Упавляющий байт 0x50 .... 0x57
        _sendAddr(addr);
        rState = Wire.endTransmission();
        if (rState != 0) return rState;

        uint16_t nPage = _pSize - ( addr & (_pSize - 1) );
        uint8_t nRead = qBytes < nPage ? qBytes : nPage;
        nRead = BUFFER_LENGTH < nRead ? BUFFER_LENGTH : nRead;
        Wire.requestFrom(ctrlByte, nRead);
        for (uint8_t i = 0; i < nRead; i++)
            if (Wire.available()) values[i] = Wire.read();
                else break;

        addr += nRead;          // increment the EEPROM address
        values += nRead;        // increment the input data pointer
        qBytes -= nRead;        // decrement the number of bytes left to write

    } while (qBytes > 0);

    return 0;
};*/




//���������� ���������� ����� ����� &= ~((1<<8)|(1<<9)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15)));

/*char* readPCharIntEEPROM(const uint16_t &addr, const uint16_t &quan){
  uint8_t i = 0;
  while((i < quan) && (EEPROM.read(addr+i) != 0)){
    if (((addr + i) >= 0) && ((addr + i) < EEPROM.length())) {
      buff[i] = char(EEPROM.read(addr + i));
      i++;
    } else break;
  }
  buff[i] = '\0';
  return buff;
}*/

/*void writePCharIntEEPROM(const uint16_t &addr, const uint8_t * sendStr){
  if ((addr >= 0) && ((addr + strlen(sendStr)) < EEPROM.length()))
    for(uint8_t i = 0; i < strlen(sendStr); i++)
      EEPROM.update(addr + i, sendStr[i]);
}*/