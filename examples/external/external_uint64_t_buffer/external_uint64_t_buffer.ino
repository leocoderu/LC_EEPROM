#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model eeprom

template <typename T>
void printInfo(uint8_t res, T* newData, uint8_t sz){
  if (res == 0) { Serial.print("success\t"); eeprom.outBuffer(newData, sz); } 
           else { Serial.print("error\t"); Serial.println(res); }
}

void setup() {
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  // Default data for read/write test
  uint64_t  newData[] = {1152921504606846975, 4311233561, 0, 31232325}; // New data for read/write test  
  uint8_t   res = 0;                                                    // Variable for Result
  uint64_t  buff[8];                                                    // Buffer for reading default data
  uint64_t  buff2[8];                                                   // Buffer for reading data
 
  Serial.print("New value for write \t"); eeprom.outBuffer(newData, (sizeof(newData) / sizeof(newData[0])));

  // Get Default Value from EEPROM
  res = eeprom.extRead(ADDR, buff, (sizeof(buff) / sizeof(buff[0])));   // Save default value
  Serial.print("Read data\t"); printInfo(res, buff, (sizeof(buff) / sizeof(buff[0])));
  
  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, newData, (sizeof(newData) / sizeof(newData[0])));
  Serial.print("Write data\t"); printInfo(res, newData, (sizeof(newData) / sizeof(newData[0])));

  // Get value by address
  res = eeprom.extRead(ADDR, buff2, (sizeof(buff2) / sizeof(buff2[0])));
  Serial.print("Read data\t"); printInfo(res, buff2, (sizeof(buff2) / sizeof(buff2[0])));
  
  // Return value to address
  res = eeprom.extWrite(ADDR, buff, (sizeof(buff) / sizeof(buff[0])));
  Serial.print("Write data\t"); printInfo(res, buff, (sizeof(buff) / sizeof(buff[0])));

  // Get value by address
  res = eeprom.extRead(ADDR, buff2, (sizeof(buff2) / sizeof(buff2[0])));
  Serial.print("Read data\t"); printInfo(res, buff2, (sizeof(buff2) / sizeof(buff2[0])));
}

void loop() {}