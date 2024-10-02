#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EEPROM eeprom;                               // Create object eeprom

void setup() {
  Serial.begin(9600); delay(500);                                           // Delay for init EEPROM memory
  Serial.println();

  // Default data for read/write test
  int8_t newData[] = { -45, 0, 37, -95, -67, 4, 93, 21, -4, 23, -111, 15 }; // Default data for read/write test - 12 bytes
  uint8_t res = 0;                                                          // Variable for Result
  int8_t buff[32];                                                          // Buffer for reading default data
  int8_t buff2[32];                                                         // Buffer for reading data
   
  Serial.print("New value for write \t"); eeprom.outBuffer(newData, sizeof(newData));

  // Get Default Value from EEPROM
  res = eeprom.intRead(ADDR, buff, sizeof(buff));                         // Save default value
  Serial.print("Read data\t"); Serial.print(res == 0 ? "success\t" : "error\t"); eeprom.outBuffer(buff, sizeof(buff));
  
  // Write default data and return result of operation 
  res = eeprom.intWrite(ADDR, newData, sizeof(newData));
  Serial.print("Write data\t"); Serial.print(res == 0 ? "success\t" : "error\t");eeprom.outBuffer(newData, sizeof(newData));

  eeprom.intShow(ADDR, ADDR + 0xFF, 32);

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
