/*
 Name:    TestEEPROM.ino
 Created: 25/09/2024 17:41:04 
 Author:  Leo
*/

#include <LC_EEPROM.h>
#define ADDR 0x0400

LC_EEPROM eeprom;
LC_EXT_EEPROM ex_eeprom(_24LC512);   

void printBuffChar(char* src, const uint8_t& sz) {
  Serial.print("[");
  for (uint8_t n = 0; n < sz; n++) Serial.print(src[n]);  
  Serial.println("]");
}

void setup() {
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  
  Serial.println();  
  Serial.println("------------------------------------------------------");
  Serial.println("                 Test EEPROM internal");
  Serial.println("------------------------------------------------------");
  Serial.println(" Type   | Write  | Read   | Update | Delete | Result");
  Serial.println("------------------------------------------------------");
  Serial.print(" Byte   | ");

  bool    resO = true;                              // Result of test
  uint8_t res  = 0;                                 // Result of operation

  // Test Byte
  uint8_t newB = 12;                                // Default data for read/write test
  uint8_t updB = 34;                                // Another data for update test
  uint8_t defB = 0;                                 // Save default value  
  uint8_t getB = 0;                                 // Get Byte Value

  eeprom.intRead(ADDR, defB);                       // Save default value  
  
  // Write
  res = eeprom.intWrite(ADDR, newB);                // Write default data
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");

  // Read
  res = eeprom.intRead(ADDR, getB);                 // Read Data by Address
  Serial.print((getB == newB) ? "pass " : "error"); // If value not equil wrote value early, it's error
  if (getB != newB) resO = false;
  Serial.print("  | "); 
  
  // Update
  res = eeprom.intWrite(ADDR, updB);                // Update Data by Address
  Serial.print((res == 0) ? "pass " : "error" );    // If value not equil wrote value early, it's error
  if (res > 0) resO = false;
  Serial.print("  | ");  
  
  // Delete
  res = eeprom.intWrite(ADDR, defB);                // Return default value
  Serial.print((res == 0) ? "pass " : "error");     // If value not equil wrote value early, it's error
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Result of operation
  Serial.println(resO ? "success" : "error");
  
  Serial.println("---------------------------------------------------------");
  Serial.print(" Int    | ");
  
  // Test Integer
  uint16_t newI = 500;                              // Default data for read/write test
  uint16_t updI = 1045;                             // Another data for update test
  uint16_t defI = 0;                                // Save default value
  uint16_t getI = 0;                                // Get Data Value
  resO = true;                                      // Result of test

  eeprom.intRead(ADDR, defI);                       // Save default value
    
  // Write
  res = eeprom.intWrite(ADDR, newI);                // Write default data
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Read
  res = eeprom.intRead(ADDR, getI);                 // Read Data by Address
  Serial.print((getI == newI) ? "pass " : "error"); // If value not equil wrote value early, it's error
  if (getI != newI) resO = false;
  Serial.print("  | "); 
  
  // Update
  res = eeprom.intWrite(ADDR, updI);
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");  
  
  // Delete
  res = eeprom.intWrite(ADDR, defI);                // Return default value
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Result of operation
  Serial.println(resO ? "success" : "error");

  Serial.println("---------------------------------------------------------");
  Serial.print(" Long   | ");

  // Test Long
  uint32_t newL = 1265535;                          // Default data for read/write test
  uint32_t updL = 1754658;                          // Another data for update test
  uint32_t defL = 0;                                // Save default value
  uint32_t getL = 0;                                // Get Data Value
  resO = true;                                      // Result of test

  eeprom.intRead(ADDR, defL);                       // Save default value
  
  // Write
  res = eeprom.intWrite(ADDR, newL);                // Write default data
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Read
  res = eeprom.intRead(ADDR, getL);
  Serial.print((getL == newL) ? "pass " : "error"); // If value not equil wrote value early, it's error
  if (getL != newL) resO = false;
  Serial.print("  | "); 
  
  // Update
  res = eeprom.intWrite(ADDR, updL);
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");  
  
  // Delete
  res = eeprom.intWrite(ADDR, defL);                // Return default value
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Result of operation
  Serial.println(resO ? "success" : "error");

  Serial.println("---------------------------------------------------------");  
  Serial.print(" String | ");
  // Test String
  // Default data for read/write test 194 symbols                             
  String newS = "Testing long long long string, its the biggest testing string #1 Testing long long long string, its the biggest testing string #2 Testing long long long string, its the biggest testing string #3";
  // Another data for update test 168 symblos
  String updS = "Another datd for tersting long long very long string #1 Another datd for tersting long long very long string #2 Another datd for tersting long long very long string #3";
  String defS = "";                                 // Save default value
  String getS = "";                                 // Get Data Value
  resO = true;                                      // Result of test

  eeprom.intRead(ADDR, defS, 194);                  // Save default value

  // Write
  res = eeprom.intWrite(ADDR, newS);                // Write default data
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Read
  res = eeprom.intRead(ADDR, getS, newS.length());
  Serial.print((getS == newS) ? "pass " : "error"); // If value not equil wrote value early, it's error
  if (getS != newS) resO = false;
  Serial.print("  | "); 

  // Update
  res = eeprom.intWrite(ADDR, updS);
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");  
  
  // Delete
  res = eeprom.intWrite(ADDR, defS);                // Return default value
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Result of operation
  Serial.println(resO ? "success" : "error");
  
  Serial.println("---------------------------------------------------------");  
  Serial.print(" BlockC | ");
  // Test Block
  // Default data for read/write test 194 symbols                             
  char newBuf[] = "Testing long long long string, its the biggest testing string #1 Testing long long long string, its the biggest testing string #2 Testing long long long string, its the biggest testing string #3";
  // Another data for update test 168 symblos
  char updBuf[] = "Another datd for tersting long long very long string #1 Another datd for tersting long long very long string #2 Another datd for tersting long long very long string #3";
  char defBuf[194];
  eeprom.intRead(ADDR, defBuf, sizeof(defBuf));     // Save default value
  uint8_t  getBuf[194];                             // Get Data Value
  resO = true;                                      // Result of test

  // Write
  res = eeprom.intWrite(ADDR, newBuf, sizeof(newBuf));  // Write default data
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");

  // Read
  res = eeprom.intRead(ADDR, getBuf, sizeof(getBuf));
  Serial.print((res == 0) ? "pass " : "error");     // If value not equil wrote value early, it's error
  if (res > 0) resO = false;
  Serial.print("  | "); 

  // Update
  res = eeprom.intWrite(ADDR, updBuf, sizeof(updBuf));
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");  
  
  // Delete
  res = eeprom.intWrite(ADDR, defBuf, sizeof(defBuf)); // Return default value
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Result of operation
  Serial.println(resO ? "success" : "error");

  Serial.println("---------------------------------------------------------");
  Serial.print(" BlockB | ");
  // Test Block
  // Default data for read/write test 194 symbols                             
  uint8_t newBufB[] = { 0x0C, 0x18, 0x0F, 0x61, 0x86, 0x04, 0x5F, 0xD7, 0x2F, 0x4B, 0x5D, 0x1A, 0xFE, 0x02, 0x77, 0xAD, 0x83 }; // 17 digits
  // Another data for update test
  uint8_t updBufB[] = { 0xFC, 0xA8, 0x1F, 0x31, 0xF6, 0x74, 0x31, 0xA7, 0x24, 0x1B, 0x0D, 0xFF, 0x11, 0xD2, 0xAE, 0xA1, 0x13 }; // 17 digits
  uint8_t defBufB[64];
  eeprom.intRead(ADDR, defBufB, sizeof(defBufB));   // Save default value
  uint8_t  getBufB[64];                             // Get Data Value
  resO = true;                                      // Result of test

  // Write
  res = eeprom.intWrite(ADDR, newBufB, sizeof(newBufB)); // Write default data
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");

  // Read
  res = eeprom.intRead(ADDR, getBufB, sizeof(getBufB));
  Serial.print((res == 0) ? "pass " : "error");     // If value not equil wrote value early, it's error
  if (res > 0) resO = false;
  Serial.print("  | "); 

  // Update
  res = eeprom.intWrite(ADDR, updBufB, sizeof(updBufB));
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");  
  
  // Delete
  res = eeprom.intWrite(ADDR, defBufB, sizeof(defBufB)); // Return default value
  Serial.print((res == 0) ? "pass " : "error");     // If new value equil first read value -> write error or value was changed
  if (res > 0) resO = false;
  Serial.print("  | ");
  
  // Result of operation
  Serial.println(resO ? "success" : "error");

  Serial.println("---------------------------------------------------------");

  eeprom.intShow(ADDR, ADDR + 0xFF, 32);

//  ex_eeprom.begin(LC_EXT_EEPROM::highSpeed);      // LC_EEPROM::lowSpeed / LC_EEPROM::highSpeed / LC_EEPROM::maxSpeed
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
