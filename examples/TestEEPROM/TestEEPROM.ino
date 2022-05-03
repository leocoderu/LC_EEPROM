/*
 Name:		TestEEPROM.ino
 Created:	5/2/2022 8:24:04 AM
 Author:	Leo
*/

#include <LC_EEPROM.h>

LC_EEPROM extEEPROM(LC_EEPROM::kbits_512, 1, 64);

void setup() {
  Serial.begin(9600);
  uint8_t resEEPROM = extEEPROM.begin(LC_EEPROM::highSpeed);
  
  if (resEEPROM) {
    Serial.print(F("extEEPROM.begin() failed, status = ")); Serial.println(resEEPROM);
    while (1);
  }

  // Test function for External EEPROM memory

  extEEPROM.writeByte(0x0001, 0xCC);
  Serial.print("Address of Byte 0x0001 (CC) = "); Serial.println(extEEPROM.readByte(0x0001), HEX);

  extEEPROM.writeInt(0x0003, 0xBB);
  Serial.print("Address of Int  0x0003 (BB) = "); Serial.println(extEEPROM.readInt(0x0003), HEX);

  extEEPROM.writeLong(0x0006, 0xAA);
  Serial.print("Address of Long 0x0006 (AA) = "); Serial.println(extEEPROM.readLong(0x0006), HEX);

  String test = "Test String";
  extEEPROM.writeStr(0x0064, test);

  String newStr = "";
  newStr = extEEPROM.readStr(0x0064, 11);
  Serial.print("Internal address of String  0x0064 (Test String) = "); Serial.println(newStr);

  extEEPROM.fillBlock(0x0020, 10, 0xDD);
  Serial.println("Address of Block from 0x0020 (DD DD DD DD DD DD DD DD DD DD):");
  extEEPROM.show(0x0016, 0x0032, 32);
  Serial.println();
  
  int8_t _buff[10];
  extEEPROM.readBlock(0x0020, 0x00, _buff, 10);     // Read from Block by address 0x0020 to buffer 10 bytes
  extEEPROM.writeBlock(0x0040, 0xFF, _buff, 10);    // Write Block from buffer to another addresses 0x0040
  Serial.println("Address of Block 0x0040 (DD DD DD DD DD DD DD DD DD DD):");
  extEEPROM.show(0x0020, 0x0064, 32);
  Serial.println();


  // Test function for Internal EEPROM memory

  extEEPROM.iWriteByte(0x0002, 0xCC);
  Serial.print("Internal address of Byte 0x0002 (CC) = "); Serial.println(extEEPROM.iReadByte(0x0002), HEX);

  extEEPROM.iWriteInt(0x0005, 0xBB);
  Serial.print("Internal address of Int  0x0005 (BB) = "); Serial.println(extEEPROM.iReadInt(0x0005), HEX);

  extEEPROM.iWriteLong(0x0009, 0xAA);
  Serial.print("Internal address of Long  0x0009 (AA) = "); Serial.println(extEEPROM.iReadLong(0x0009), HEX);

  String test2 = "Test String";
  extEEPROM.iWriteStr(0x0032, test2);

  String newStr2 = "";
  newStr2 = extEEPROM.iReadStr(0x0032, 11);
  Serial.print("Internal address of String  0x0032 (Test String) = "); Serial.println(newStr2);

  extEEPROM.iFillBlock(0x0050, 10, 0xDD);
  Serial.println("Internal address of Block from 0x0050 (DD DD DD DD DD DD DD DD DD DD):");
  extEEPROM.iShow(0x0045, 0x0064, 32);
  Serial.println();

  int8_t _buff2[10];
  extEEPROM.iReadBlock(0x0050, 0x00, _buff2, 10);     // Read from Interanl Block by address 0x0050 to buffer 10 bytes
  extEEPROM.iWriteBlock(0x0070, 0xFF, _buff2, 10);    // Write Block from buffer to another Internal addresses 0x0070
  Serial.println("Address of Block 0x0070 (DD DD DD DD DD DD DD DD DD DD):");
  extEEPROM.iShow(0x0045, 0x0096, 32);
  Serial.println();
  

  //extEEPROM.fillBlock(0x0000, 0x00FF, 0xFF);
  //extEEPROM.iFillBlock(0x0000, 0xFFFF, 0xFF);

  extEEPROM.show(0x0000, 0x0096, 32);   // Show part of External EEPROM memory
  extEEPROM.iShow(0x0000, 0x0096, 32);  // Show part of Internal EEPROM memory
  //extEEPROM.iShow();
}

void loop() {

}
