#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EEPROM eeprom;                               // Create object eeprom

void setup() {
  Serial.begin(9600); delay(500);                                         // Delay for init EEPROM memory
  Serial.println();

  // Default data for read/write test
  uint8_t newData[] = { 0x0C, 0x18, 0x0F, 0x61, 0x86, 0x04, 0x5F, 0xD7, 0x2F, 0x4B, 0x5D, 0x1A };  // Default data for read/write test - 12 bytes
  uint8_t res = 0;                                                        // Variable for Result
  uint8_t buff[32];                                                       // Buffer for reading default data
  uint8_t buff2[32];                                                      // Buffer for reading data
   
  Serial.print("New value for write \t"); eeprom.outBuffer(newData, sizeof(newData));

  // Get Default Value from EEPROM
  res = eeprom.intRead(ADDR, buff, sizeof(buff));                         // Save default value
  Serial.print("Read data\t"); Serial.print(res == 0 ? "success\t" : "error\t"); eeprom.outBuffer(buff, sizeof(buff));
  
  // Write default data and return result of operation 
  res = eeprom.intWrite(ADDR, newData, sizeof(newData));
  Serial.print("Write data\t"); Serial.print(res == 0 ? "success\t" : "error\t");eeprom.outBuffer(newData, sizeof(newData));

  // Get value by address
  res = eeprom.intRead(ADDR, buff2, sizeof(buff2));
  Serial.print("Read data\t"); Serial.print(res == 0 ? "success\t" : "error\t"); eeprom.outBuffer(buff2, sizeof(buff2));
  
  // Return value to address
  res = eeprom.intWrite(ADDR, buff, sizeof(buff));
  Serial.print("Write data\t"); Serial.print(res == 0 ? "success\t" : "error\t");eeprom.outBuffer(buff, sizeof(buff));

  // Get value by address
  res = eeprom.intRead(ADDR, buff2, sizeof(buff2));
  Serial.print("Read data\t"); Serial.print(res == 0 ? "success\t" : "error\t"); eeprom.outBuffer(buff2, sizeof(buff2)); 
}

void loop() {}
