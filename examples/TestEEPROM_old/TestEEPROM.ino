/*
 Name:		TestEEPROM.ino
 Created:	28/08/2024 21:01:04 AM
 Author:	Leo
*/

#include <LC_EEPROM.h>

LC_EXT_EEPROM extEEPROM(LC_EXT_EEPROM::kbits_512, 1, 64);

void setup() {
  Serial.begin(9600);
  uint8_t resEEPROM = extEEPROM.begin(LC_EXT_EEPROM::highSpeed);
  
  if (resEEPROM) {
    Serial.print(F("extEEPROM.begin() failed, status = ")); Serial.println(resEEPROM);
    while (1);
  }

  // Test function for External EEPROM memory

  extEEPROM.extWriteByte(0x0001, 0xCC);
  Serial.print("Address of Byte 0x0001 (CC) = "); Serial.println(extEEPROM.extReadByte(0x0001), HEX);

  extEEPROM.extWriteInt(0x0003, 0xBB);
  Serial.print("Address of Int  0x0003 (BB) = "); Serial.println(extEEPROM.extReadInt(0x0003), HEX);

  extEEPROM.extWriteLong(0x0006, 0xAA);
  Serial.print("Address of Long 0x0006 (AA) = "); Serial.println(extEEPROM.extReadLong(0x0006), HEX);

  String test = "Test String";
  extEEPROM.extWriteStr(0x0064, test);

  String newStr = "";
  newStr = extEEPROM.extReadStr(0x0064, 11);
  Serial.print("Internal address of String  0x0064 (Test String) = "); Serial.println(newStr);

  extEEPROM.extFillBlock(0x0020, 10, 0xDD);
  Serial.println("Address of Block from 0x0020 (DD DD DD DD DD DD DD DD DD DD):");
  extEEPROM.extShow(0x0016, 0x0032, 32);
  Serial.println();
  
  int8_t _buff[10];
  extEEPROM.extReadBlock(0x0020, 0x00, _buff, 10);     // Read from Block by address 0x0020 to buffer 10 bytes
  extEEPROM.extWriteBlock(0x0040, 0xFF, _buff, 10);    // Write Block from buffer to another addresses 0x0040
  Serial.println("Address of Block 0x0040 (DD DD DD DD DD DD DD DD DD DD):");
  extEEPROM.extShow(0x0020, 0x0064, 32);
  Serial.println();


  // Test function for Internal EEPROM memory

  extEEPROM.intWriteByte(0x0002, 0xCC);
  Serial.print("Internal address of Byte 0x0002 (CC) = "); Serial.println(extEEPROM.intReadByte(0x0002), HEX);

  extEEPROM.intWriteInt(0x0005, 0xBB);
  Serial.print("Internal address of Int  0x0005 (BB) = "); Serial.println(extEEPROM.intReadInt(0x0005), HEX);

  extEEPROM.intWriteLong(0x0009, 0xAA);
  Serial.print("Internal address of Long  0x0009 (AA) = "); Serial.println(extEEPROM.intReadLong(0x0009), HEX);

  String test2 = "Test String";
  extEEPROM.intWriteStr(0x0032, test2);

  String newStr2 = "";
  newStr2 = extEEPROM.intReadStr(0x0032, 11);
  Serial.print("Internal address of String  0x0032 (Test String) = "); Serial.println(newStr2);

  extEEPROM.intFillBlock(0x0050, 10, 0xDD);
  Serial.println("Internal address of Block from 0x0050 (DD DD DD DD DD DD DD DD DD DD):");
  extEEPROM.intShow(0x0045, 0x0064, 32);
  Serial.println();

  int8_t _buff2[10];
  extEEPROM.intReadBlock(0x0050, 0x00, _buff2, 10);     // Read from Interanl Block by address 0x0050 to buffer 10 bytes
  extEEPROM.intWriteBlock(0x0070, 0xFF, _buff2, 10);    // Write Block from buffer to another Internal addresses 0x0070
  Serial.println("Address of Block 0x0070 (DD DD DD DD DD DD DD DD DD DD):");
  extEEPROM.intShow(0x0045, 0x0096, 32);
  Serial.println();
  

  //extEEPROM.extFillBlock(0x0000, 0x00FF, 0xFF);
  //extEEPROM.intFillBlock(0x0000, 0xFFFF, 0xFF);

  extEEPROM.extShow(0x0000, 0x0096, 32);    // Show part of External EEPROM memory
  extEEPROM.intShow(0x0000, 0x0096, 32);    // Show part of Internal EEPROM memory
  //extEEPROM.iShow();
}

void loop() {

}
