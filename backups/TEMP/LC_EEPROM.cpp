#include "LC_EEPROM.h"

//using namespace LC_EEPROM;

// Constructor of Class
LC_EEPROM::LC_EEPROM(const eeprom_size_t& devCapacity, const uint8_t& qDevice){ //, const uint8_t& pSize){
    _devCapacity = devCapacity;
    _qDevice = qDevice;
    _totalCapacity = _qDevice * _devCapacity * 1024.0 / 8;
}

// Begins work I2C wire, set set Clock frequency
uint8_t LC_EEPROM::begin(twiFreq_t twiFreq){
    Wire.begin();
    Wire.setClock(twiFreq);
    Wire.beginTransmission(_eepromAddr);
    _sendAddr(0x0000);
    return Wire.endTransmission();
}

// Returns total Capacity of memory
uint32_t LC_EEPROM::getTotalCapacity(){
    return _totalCapacity;
};

// Returns control byte of module by address, 0x50 / 0x51 /0x52 / ... /0x57
uint8_t LC_EEPROM::getCtrlByte(const uint32_t& addr){
    return _eepromAddr | (uint8_t)((addr / 1024.0) / (_devCapacity / 8.0));
};

// Returns value of byte by address
uint8_t LC_EEPROM::readByte(const uint32_t& addr){
    if (addr > _totalCapacity) return 0;

    uint8_t ctrlByte = _eepromAddr | (uint8_t)((addr / 1024.0) / (_devCapacity / 8.0));
    Wire.beginTransmission(ctrlByte);                       // Control byte 0x50 .... 0x57
    LC_EEPROM::_sendAddr(addr);
    Wire.endTransmission();

    Wire.requestFrom((uint8_t)ctrlByte, (uint8_t)1);        // In Wire library present two functions, (uint8_t) for disable attentions
    return (Wire.available()) ? Wire.read() : 0;
}

// Returns Integer value (2 bytes) by address
uint16_t LC_EEPROM::readInt(const uint32_t& addr){
    if (addr + 1 > _totalCapacity) return 0;
    return LC_EEPROM::readByte(addr) << 8 | LC_EEPROM::readByte(addr + 1);
}

// Returns Longint value (4 bytes) by address
uint32_t LC_EEPROM::readLong(const uint32_t& addr){
    if (addr + 3 > _totalCapacity) return 0;
    return ((uint32_t)LC_EEPROM::readByte(addr) << 24) | ((uint32_t)LC_EEPROM::readByte(addr + 1) << 16) | (LC_EEPROM::readByte(addr + 2) << 8) | LC_EEPROM::readByte(addr + 3);
}

// Returns String value by address
String LC_EEPROM::readStr(const uint32_t& addr, const int16_t& quan){
    String res = "";
    for(uint16_t i = 0; i < quan; i++)
        res += char(LC_EEPROM::readByte(addr + i));
    return res;
}

// Returns Block data by address
void LC_EEPROM::readBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst) {
    memset(dst, defVal, szDst);                 // Set destination array values by default values
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = LC_EEPROM::readByte(addr + i);
        if (bt == 0xFF) break; // TODO: needs to change 0xFF or not
        dst[i] = bt;
    }   // perhaps value 0xFF need to change by defVal, but if read block to char array needs the end by 0
}

// Write byte by address
void LC_EEPROM::writeByte(const uint32_t& addr, const uint8_t& wByte){
    if (LC_EEPROM::readByte(addr) != wByte) {
        uint8_t wData[1] = {wByte};
        LC_EEPROM::_write(addr, wData, 1);
    }
}

// Write Integer value by address
void LC_EEPROM::writeInt(const uint32_t& addr, const uint16_t& wInt) {
    if (LC_EEPROM::readInt(addr) != wInt) {
        uint8_t wData[2] = {wInt >> 8, (uint8_t)wInt};
        LC_EEPROM::_write(addr, wData, 2);
    }
}

// Write Longint value by address
void LC_EEPROM::writeLong(const uint32_t& addr, const uint32_t& wLong) {
    if (LC_EEPROM::readLong(addr) != wLong) {
        uint8_t wData[4] = {(uint8_t)(wLong >> 24), (uint8_t)(wLong >> 16), (uint8_t)(wLong >> 8), (uint8_t)wLong};
        LC_EEPROM::_write(addr, wData, 4);
    }
}

// Write string by address
void LC_EEPROM::writeStr(const uint32_t& addr, const String& sendStr)
{
    if (LC_EEPROM::readStr(addr, sendStr.length()) != sendStr) {
        uint8_t wData[sendStr.length() + 1];
        sendStr.toCharArray(wData, sizeof(wData));
        LC_EEPROM::_write(addr, wData, sendStr.length());
    }
}

// Fill block memory by default value
void LC_EEPROM::fillBlock(const uint32_t& addr, const uint32_t& cnt, const uint8_t& bt) {
    for (uint32_t i = 0; i < cnt; i++) LC_EEPROM::writeByte(addr + i, bt);
}

// Write Block data by address
void LC_EEPROM::writeBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc) {
    fillBlock(addr, szSrc, defVal);
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
    for (uint32_t i = 0; i < cnt; i++) iWriteByte(addr + i, bt);
}

void LC_EEPROM::iReadBlock(const uint32_t& addr, const int8_t& defVal, int8_t* dst, const uint8_t& szDst) {
    memset(dst, defVal, szDst);                 // ���������� ������� INT8_MIN
    for (uint8_t i = 0; i < szDst; i++) {
        uint8_t bt = LC_EEPROM::iReadByte(addr + i);
        if (bt == 0xFF) break;
        dst[i] = bt;
    }
}
void LC_EEPROM::iWriteBlock(const uint32_t& addr, const uint8_t& defVal, const int8_t* src, const uint8_t& szSrc) {
    iFillBlock(addr, szSrc, defVal);
    for (uint8_t i = 0; i < szSrc; i++)
        if (((defVal == 0xFF) && (src[i] == INT8_MIN)) || ((defVal == 0x00) && (src[i] == 0x00))) break;
        else LC_EEPROM::iWriteByte(addr + i, src[i]);
}

// ---------------------------------------------- Show EEPROM ---------------------------------------------------------
void LC_EEPROM::show(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan) {
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
        cByte +=  String(LC_EEPROM::getCtrlByte(j * quan), HEX) + ' ';
        String tSt = LC_EEPROM::_preFix(String(j * quan, HEX), 8, '0') + ' ';
        tSt.toUpperCase();
        tSt = cByte + tSt;
        Serial.print(tSt);

        uint8_t res[quan];
        String tmpStr = "";
        for (uint8_t i = 0; i < quan; i++)
            tmpStr += LC_EEPROM::_preFix(String(readByte((j * quan) + i), HEX), 2, '0') + ' ';
        tmpStr.toUpperCase();
        Serial.println(tmpStr);
    }
}

void LC_EEPROM::iShow(const uint32_t& addrFrom, const uint32_t& addrTo, const uint8_t& quan = 32) {
    Serial.print("Internal EEPROM memory...");

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
            String tmpStr = LC_EEPROM::_preFix(String(iReadByte((j * quan) + i), HEX), 2, '0') + ' ';
            tmpStr.toUpperCase();
            Serial.print(tmpStr);
        }
        Serial.println();
    }
}

// DESTRUCTOR
LC_EEPROM::~LC_EEPROM() {

};

// ------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------
void LC_EEPROM::_sendAddr(const uint32_t& addr){
    uint8_t nAddrBytes = _devCapacity > kbits_16 ? 2 : 1;               // For module with 1024bit & 2048 may be 3 bytes for adress
    if (nAddrBytes == 2) Wire.write(static_cast<uint8_t>(addr >> 8));   // write high address byte
    Wire.write(static_cast<uint8_t>(addr));                             // write low  address byte
}

uint8_t LC_EEPROM::_write(uint32_t addr, uint8_t* wData, uint16_t qBytes) {
    if (addr + qBytes > _totalCapacity) return EEPROM_ADDR_ERR;

    uint8_t txState = 0;
    while(qBytes > 0){
        uint16_t nPage = _pSize - (addr & (_pSize - 1));                // Part of page size for write data from address
        uint16_t nWrite = (qBytes < nPage) ? qBytes : nPage;            // Quantity of Bytes that you cat write to page
        uint8_t  ctrlByte = _eepromAddr | (uint8_t)((addr / 1024.0) / (_devCapacity / 8.0));

        Wire.beginTransmission(ctrlByte);
        LC_EEPROM::_sendAddr(addr);
        Wire.write(wData, nWrite);
        txState = Wire.endTransmission();
        if(txState != 0) return txState;

        // wait up to 5ms for the write to complete, Wait 5ms max!!!
        for (uint8_t i = 0; i < 10; i++) {
            delayMicroseconds(500);
            Wire.beginTransmission(ctrlByte);
            LC_EEPROM::_sendAddr(0);
            txState = Wire.endTransmission();
            if (txState == 0) break;
        }
        if (txState != 0) return txState;

        addr   += nWrite;         // increment the EEPROM address
        wData  += nWrite;         // increment the input data pointer
        qBytes -= nWrite;         // decrement the number of bytes left to write
    };
    return txState;
}

String LC_EEPROM::_preFix(String str, byte quan, char chr = '0') {
    String result = str;
    for (uint8_t i = 0; i < quan - str.length(); i++) result = chr + result;
    return result;
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