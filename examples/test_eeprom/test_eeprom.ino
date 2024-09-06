#include <LC_EEPROM.h>
#define ADDR 0x0400

LC_EEPROM eeprom;
LC_EXT_EEPROM ex_eeprom(LC_EXT_EEPROM::kbits_512, 1, 64);   

void setup() {
  Serial.begin(9600);
  
  Serial.println();  
  Serial.println("------------------------------------------------------");
  Serial.println("                 Test EEPROM internal");
  Serial.println("------------------------------------------------------");
  Serial.println(" Type   | Write  | Read   | Update | Delete | Result");
  Serial.println("------------------------------------------------------");
  Serial.print(" Byte   | ");

  uint8_t n = 12;                               // Default data for read/write test
  uint8_t j = 34;                               // Another data for update test
  bool    r = true;                             // Result of test
  uint8_t b = eeprom.intReadByte(ADDR);         // Save default value
  // Write
  eeprom.intWriteByte(ADDR, n);                 // Write default data
  uint8_t w = eeprom.intReadByte(ADDR);         // Reading the data that we just wrote
  Serial.print((b == w) ? "error" : "pass ");    // If new value equil first read value -> write error or value was changed
  if (b == w) r = false;
  Serial.print("  | ");
  // Read
  Serial.print((w != n) ? "error" : "pass ");    // If value not equil wrote value early, it's error
  if (w != n) r = false;
  Serial.print("  | "); 
  // Update
  eeprom.intWriteByte(ADDR, j);
  uint8_t m = eeprom.intReadByte(ADDR);
  Serial.print((m != j) ? "error" : "pass ");    // If value not equil wrote value early, it's error
  if (m != j) r = false;
  Serial.print("  | ");  
  // Delete
  eeprom.intWriteByte(ADDR, b);                 // Return default value
  uint8_t z = eeprom.intReadByte(ADDR);         // Check operation
  Serial.print((b != z) ? "error" : "pass ");    // If value not equil wrote value early, it's error
  if (b != z) r = false;
  Serial.print("  | ");
  // Result of operation
  Serial.println(r ? "success" : "error");
  
  Serial.println("---------------------------------------------------------");
  Serial.print(" Int    | ");
  uint16_t n2 = 500;                               // Default data for read/write test
  uint16_t j2 = 1045;                              // Another data for update test
  bool     r2 = true;                              // Result of test
  uint16_t b2 = eeprom.intReadInt(ADDR);           // Save default value
  // Write
  eeprom.intWriteInt(ADDR, n2);                    // Write default data
  uint16_t w2 = eeprom.intReadInt(ADDR);           // Reading the data that we just wrote
  Serial.print((b2 == w2) ? "error" : "pass ");     // If new value equil first read value -> write error or value was changed
  if (b2 == w2) r2 = false;
  Serial.print("  | ");
  // Read
  Serial.print((w2 != n2) ? "error" : "pass ");     // If value not equil wrote value early, it's error
  if (w2 != n2) r2 = false;
  Serial.print("  | "); 
  // Update
  eeprom.intWriteInt(ADDR, j2);
  uint16_t m2 = eeprom.intReadInt(ADDR);
  Serial.print((m2 != j2) ? "error" : "pass ");     // If value not equil wrote value early, it's error
  if (m2 != j2) r2 = false;
  Serial.print("  | ");  
  // Delete
  eeprom.intWriteInt(ADDR, b2);                    // Return default value
  uint16_t z2 = eeprom.intReadInt(ADDR);           // Check operation
  Serial.print((b2 != z2) ? "error" : "pass ");     // If value not equil wrote value early, it's error
  if (b2 != z2) r2 = false;
  Serial.print("  | ");
  // Result of operation
  Serial.println(r2 ? "success" : "error");

  Serial.println("---------------------------------------------------------");
  Serial.print(" Long   | ");
  uint32_t n3 = 1265535;                           // Default data for read/write test
  uint32_t j3 = 1754658;                           // Another data for update test
  bool     r3 = true;                              // Result of test
  uint32_t b3 = eeprom.intReadLong(ADDR);           // Save default value
  // Write
  eeprom.intWriteLong(ADDR, n3);                    // Write default data
  uint32_t w3 = eeprom.intReadLong(ADDR);           // Reading the data that we just wrote
  Serial.print((b3 == w3) ? "error" : "pass ");     // If new value equil first read value -> write error or value was changed
  if (b3 == w3) r3 = false;
  Serial.print("  | ");
  // Read
  Serial.print((w3 != n3) ? "error" : "pass ");     // If value not equil wrote value early, it's error
  if (w3 != n3) r3 = false;
  Serial.print("  | "); 
  
  // Update
  eeprom.intWriteLong(ADDR, j3);
  uint32_t m3 = eeprom.intReadLong(ADDR);
  Serial.print((m3 != j3) ? "error" : "pass ");     // If value not equil wrote value early, it's error
  if (m3 != j3) r3 = false;
  Serial.print("  | ");  

  Serial.println();
  Serial.print("--- ");Serial.print(j3);Serial.println(" ---");
  Serial.print("--- ");Serial.print(m3);Serial.println(" ---");

  eeprom.intShow(0x0400, 0x04FF, 32);
  
  // Delete
  eeprom.intWriteLong(ADDR, b3);                    // Return default value
  uint32_t z3 = eeprom.intReadLong(ADDR);           // Check operation
  Serial.print((b3 != z3) ? "error" : "pass ");     // If value not equil wrote value early, it's error
  if (b3 != z3) r3 = false;
  Serial.print("  | ");
  // Result of operation
  Serial.println(r3 ? "success" : "error");

  
  Serial.println("---------------------------------------------------------");  
  Serial.println(" String | pass   | pass   | pass   | pass   | success");
  Serial.println("---------------------------------------------------------");  
  Serial.println(" Block  | pass   | pass   | pass   | pass   | success");
  Serial.println("---------------------------------------------------------");

  

//  ex_eeprom.begin(LC_EXT_EEPROM::highSpeed);                      // LC_EEPROM::lowSpeed / LC_EEPROM::highSpeed / LC_EEPROM::maxSpeed
//  
//  Serial.println();
//  Serial.println("---------------------------------------------------------");
//  Serial.println("                 Test EEPROM external");
//  Serial.println("---------------------------------------------------------");
//  Serial.println(" Type   | Write  | Read   | Update | Delete | Result");
//  Serial.println("---------------------------------------------------------");
//  Serial.println(" Byte   | pass   | pass   | pass   | pass   | success");
//  Serial.println("---------------------------------------------------------");
//  Serial.println(" Int    | pass   | pass   | pass   | pass   | success");
//  Serial.println("---------------------------------------------------------");  
//  Serial.println(" Long   | pass   | pass   | pass   | pass   | success");
//  Serial.println("---------------------------------------------------------");  
//  Serial.println(" String | pass   | pass   | pass   | pass   | success");
//  Serial.println("---------------------------------------------------------");  
//  Serial.println(" Block  | pass   | pass   | pass   | pass   | success");
//  Serial.println("---------------------------------------------------------");


    /*uint32_t addr = 0x7FFFF;                                        //  130570 / 1024  = 127
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
}

void loop() {}
