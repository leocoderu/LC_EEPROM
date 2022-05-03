#include "LC_EEPROM.h"
//#include <Wire.h>

// Constructor of Class
LC_EEPROM::LC_EEPROM(const eeprom_size_t& devCapacity, const uint8_t& qDevice, const uint16_t& pSize, const uint8_t& eeAddr = 0x50){
    _devCapacity = devCapacity;
    _eepromAddr = eeAddr;
    _qDevice = qDevice;
    _pSize = pSize;
    _totalCapacity = _qDevice * _devCapacity * 1024UL / 8;
    _nAddrBytes = _devCapacity > kbits_16 ? 2 : 1;              // two address bytes needed for eeproms > 16kbits

    // determine the bitshift needed to isolate the chip select bits from
    // the address to put into the control byte
    uint16_t kb = _devCapacity;
    if ( kb <= kbits_16 ) _csShift = 8;
    else if ( kb >= kbits_512 ) _csShift = 16;
        else {
            kb >>= 6;
            _csShift = 12;
            while ( kb >= 1 ) {
                ++_csShift;
                kb >>= 1;
            }
        }
}

uint8_t LC_EEPROM::begin(twiFreq_t twiFreq){
    Wire.begin();
    Wire.setClock(twiFreq);
    Wire.beginTransmission(_eepromAddr);
    if (_nAddrBytes == 2) Wire.write(0);    // high addr byte
    Wire.write(0);                          // low addr byte
    return Wire.endTransmission();
}

uint8_t LC_EEPROM::readByte(const uint32_t& addr) {
    _writeAddr(addr);
    Wire.requestFrom((uint8_t)_eepromAddr, (uint8_t)0x01);
    return (Wire.available()) ? Wire.read() : 0;
}

void LC_EEPROM::writeByte(const uint32_t& addr, const uint8_t& wByte){
    if (LC_EEPROM::readByte(addr) != wByte) {
        Wire.beginTransmission(_eepromAddr);
        Wire.write((byte*)&addr, 2);
        Wire.write(wByte);
        Wire.endTransmission();
        delay(5);
    }
}

uint16_t LC_EEPROM::readInt(const uint32_t& addr) {
    return LC_EEPROM::readByte(addr) << 8 | LC_EEPROM::readByte(addr + 1);
}

void LC_EEPROM::writeInt(const uint32_t& addr, const uint16_t& wInt) {
    LC_EEPROM::writeByte(addr, wInt >> 8);
    LC_EEPROM::writeByte(addr + 1, (uint8_t)wInt);
}

uint32_t LC_EEPROM::readLong(const uint32_t& addr) {
    return ((uint32_t)LC_EEPROM::readByte(addr) << 24) | ((uint32_t)LC_EEPROM::readByte(addr + 1) << 16) | (LC_EEPROM::readByte(addr + 2) << 8) | LC_EEPROM::readByte(addr + 3);
}

void LC_EEPROM::writeLong(const uint32_t& addr, const uint32_t& wDouble) {
    uint32_t tByte = wDouble;
    for (int8_t i = sizeof(wDouble) - 1; i >= 0; i--) {
        LC_EEPROM::writeByte(addr + i, (uint8_t)tByte);
        tByte = tByte >> 8;
    }
}

String LC_EEPROM::readStr(const uint32_t& addr, const uint16_t& quan) {
    String res = "";
    //if ((addr >= 0) && ((addr + quan) < EEPROM.length())) <- ������� ������������ ������ ������� ������, ����������� � JC
        for (uint16_t i = 0; i < quan; i++)
            res += char(LC_EEPROM::readByte(addr + i));
    return res;
}

void LC_EEPROM::writeStr(const uint32_t& addr, const String& sendStr)
{
    //if ((addr >= 0) && ((addr + sendStr.length()) < EEPROM.length())) <- ������� ������������ ������ ������� ������, ����������� � JC
        for (uint16_t i = 0; i < sendStr.length(); i++)
            LC_EEPROM::writeByte(addr + i, sendStr[i]);
}

void LC_EEPROM::fillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt) {
    for (uint32_t i = 0; i < cnt; i++) LC_EEPROM::writeByte(addr + i, bt);    
}

void LC_EEPROM::readBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst) {
    memset(dst, defVal, szDst);                 // ���������� ������� INT8_MIN
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = LC_EEPROM::readByte(addr + i);
        if (bt != 0xFF) dst[i] = bt;  // �������� ������ 0xFF ����� ������� defVal, �.�. ��� char ����� 0 ����� �� ������
        else break;
    }
}

void LC_EEPROM::writeBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc) {
    LC_EEPROM::fillBlock(addr, szSrc, defVal);
    for (uint8_t i = 0; i < szSrc; i++)
        if (((defVal == 0xFF) && (src[i] == INT8_MIN)) || ((defVal == 0x00) && (src[i] == 0x00))) break;
        else LC_EEPROM::writeByte(addr + i, src[i]);
}


// ---------------------------------------------- Internal EEPROM -----------------------------------------------------
uint8_t LC_EEPROM::iReadByte(const uint32_t& addr) {
    if ((addr >= 0) && (addr < EEPROM.length())) return EEPROM.read(addr);
    return 0xFF;
}

void LC_EEPROM::iWriteByte(const uint32_t& addr, const uint8_t& wByte) {
    if ((addr >= 0) && (addr < EEPROM.length())) EEPROM.update(addr, wByte);
}

uint16_t LC_EEPROM::iReadInt(const uint32_t& addr) {
    if ((addr >= 0) && ((addr + 1) < EEPROM.length())) return (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
    return 0xFFFF;
}

void LC_EEPROM::iWriteInt(const uint32_t& addr, const uint16_t& wInt) {
    if ((addr >= 0) && ((addr + 1) < EEPROM.length())) {
        byte tByte = (wInt >> 8); EEPROM.update(addr, tByte);
        tByte = ((wInt << 8) >> 8); EEPROM.update(addr + 1, tByte);
    }
}

uint32_t LC_EEPROM::iReadLong(const uint32_t& addr) {
    if ((addr >= 0) && ((addr + 3) < EEPROM.length())) return ((uint32_t)EEPROM.read(addr) << 24) | ((uint32_t)EEPROM.read(addr + 1) << 16) | (EEPROM.read(addr + 2) << 8) | EEPROM.read(addr + 3);
    return 0xFFFFFFFF;
}

void LC_EEPROM::iWriteLong(const uint32_t& addr, const uint32_t& wDouble) {
    if ((addr >= 0) && ((addr + 3) < EEPROM.length())) {
        byte tByte = (wDouble >> 24); EEPROM.update(addr, tByte);
        tByte = (byte)((wDouble << 8) >> 24); EEPROM.update(addr + 1, tByte);
        tByte = (byte)((wDouble << 16) >> 24); EEPROM.update(addr + 2, tByte);
        tByte = (byte)((wDouble << 24) >> 24); EEPROM.update(addr + 3, tByte);
    }
}

String LC_EEPROM::iReadStr(const uint32_t& addr, const uint16_t& quan) {
    String res = "";
    if ((addr >= 0) && ((addr + quan) < EEPROM.length()))
        for (uint16_t i = 0; i < quan; i++)
            res += char(EEPROM.read(addr + i));
    return res;
}

void LC_EEPROM::iWriteStr(const uint32_t& addr, const String& sendStr)
{
    if ((addr >= 0) && ((addr + sendStr.length()) < EEPROM.length()))
        for (uint16_t i = 0; i < sendStr.length(); i++)
            EEPROM.update(addr + i, sendStr[i]);
}

void LC_EEPROM::iFillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt) {
    for (uint32_t i = 0; i < cnt; i++) LC_EEPROM::iWriteByte(addr + i, bt);
}

void LC_EEPROM::iReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst) {
    memset(dst, defVal, szDst);                 // ���������� ������� INT8_MIN
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = LC_EEPROM::iReadByte(addr + i);
        if (bt != 0xFF) dst[i] = bt;  // �������� ������ 0xFF ����� ������� defVal, �.�. ��� char ����� 0 ����� �� ������
        else break;
    }
}
void LC_EEPROM::iWriteBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc) {
    LC_EEPROM::iFillBlock(addr, szSrc, defVal);
    for (uint8_t i = 0; i < szSrc; i++)
        if (((defVal == 0xFF) && (src[i] == INT8_MIN)) || ((defVal == 0x00) && (src[i] == 0x00))) break;
        else LC_EEPROM::iWriteByte(addr + i, src[i]);
}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EEPROM::show(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan) {
    Serial.println("External EEPROM memory...");

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
            String tmpStr = LC_EEPROM::_preFix(String(LC_EEPROM::readByte((j * quan) + i), HEX), 2, '0') + ' ';
            tmpStr.toUpperCase();
            Serial.print(tmpStr);
        }
        Serial.println();
    }
}

void LC_EEPROM::iShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan = 32) {
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
        Serial.print(tSt);                                    // ����� ����� � ������
        for (uint8_t i = 0; i < quan; i++) {
            String tmpStr = LC_EEPROM::_preFix(String(LC_EEPROM::iReadByte((j * quan) + i), HEX), 2, '0') + ' ';
            tmpStr.toUpperCase();
            Serial.print(tmpStr);
        }
        Serial.println();
    }
}

// DESTRUCTOR
LC_EEPROM::~LC_EEPROM() {

};

// PRIVATE FUNCTIONS
void LC_EEPROM::_writeAddr(const uint32_t& addr) {
    Wire.beginTransmission(_eepromAddr);
    Wire.write((byte*)&addr, 2);
    Wire.endTransmission();
}

String LC_EEPROM::_preFix(String str, byte quan, char chr = '0') {
    String result = str;
    for (uint8_t i = 0; i < quan - str.length(); i++) result = chr + result;
    return result;
}


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