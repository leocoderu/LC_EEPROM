#include <LC_EEPROM.h>              

LC_EXT_EEPROM extEEPROM(_24LC512, 3);   // page size was 64

void setup()
{
    Serial.begin(9600);
    //extEEPROM.begin(LC_EXT_EEPROM::highSpeed);                          // LC_EEPROM::lowSpeed / LC_EEPROM::highSpeed / LC_EEPROM::maxSpeed
    extEEPROM.begin();                          // LC_EEPROM::lowSpeed / LC_EEPROM::highSpeed / LC_EEPROM::maxSpeed

    /*uint32_t addr = 0x7FFFF;                                      //  130570 / 1024  = 127
    uint32_t _devCapacity = 512;                                    // capacity of one EEPROM device, in kbits
    const uint8_t  _eepromAddr = 0x50;                              // eeprom i2c address
    uint8_t  _qDevice = 8;                                          // number of devices on the bus
    uint8_t  _pSize = 128;                                          // page size in bytes
    uint32_t _totalCapacity = _qDevice * _devCapacity * 1024.0 / 8; // capacity of all EEPROM devices on the bus, in bytes
    uint8_t  _nAddrBytes = _devCapacity > 16 ? 2 : 1;               // number of address bytes (1 or 2)


	  uint8_t _ctrlByte1 = _eepromAddr | (uint8_t)((addr / 1024.0) / (_devCapacity / 8.0));

    Serial.print("Address: "); Serial.println(addr);
    Serial.print("Total: "); Serial.println(_totalCapacity);
    
    Serial.print("Ctrl Byte1: "); Serial.println(_ctrlByte1);*/

    uint32_t addr = 0x0000;

    //extEEPROM.extWriteByte(addr, 0xFF);
    //extEEPROM.extWriteInt(addr, 0x771F);
    //extEEPROM.extWriteLong(addr, 0x1255771F);
    String st = "Testing long long long string, its the biggest testing string #1 Testing long long long string, its the biggest testing string #2 Testing long long long string, its the biggest testing string #3"; // 194 symbols
    
    //extEEPROM.extFillBlock(0x21B00, 1888, 0xFF);
    
    //Serial.println("Start write");
    //extEEPROM.extWriteStr(addr, st);
    //Serial.println("Finish write");
    
    uint8_t  rByte = extEEPROM.extReadByte(addr);
    uint16_t rInt  = extEEPROM.extReadInt(addr);
    uint32_t rLong = extEEPROM.extReadLong(addr);
    String rStr = extEEPROM.extReadStr(addr, st.length());
    
    Serial.print("Total: "); Serial.println(extEEPROM.getTotalCapacity());
    Serial.print("Ctrl Byte: 0x"); Serial.println(extEEPROM.getCtrlByte(addr), HEX);
       
    Serial.print("rByte: ");Serial.println(rByte, HEX);
    Serial.print("rInt : ");Serial.println(rInt, HEX);
    Serial.print("rLong: ");Serial.println(rLong, HEX);
    Serial.print("rStr : ");Serial.println(rStr);   

    Serial.println("Module #1:");
    extEEPROM.extShow(0x0000, 0xFFFF, 32);
    
    Serial.println("Module #2:");
    extEEPROM.extShow(0x10000, 0x1FFFF, 32);

    Serial.println("Module #3:");
    extEEPROM.extShow(0x20000, 0x2FFFF, 32);

    //Serial.println("Module #4:");
    //extEEPROM.show(0x30000, 0x3FFFF, 32);
    
    //extEEPROM.show(0x21E00, 0x221FF, 32);
}

/*template <typename T>
T sum(T x, T y){
 return x + y; 
}*/

void loop(){}
