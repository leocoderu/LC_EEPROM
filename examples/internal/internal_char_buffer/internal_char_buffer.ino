#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EEPROM eeprom;                               // Create object eeprom

template <typename T>
void printInfo(uint8_t res, T* newData, uint8_t sz, bool ez){
  if (res == 0) { Serial.print("success\t"); eeprom.outBuffer(newData, sz, ez); } 
           else { Serial.print("error\t"); Serial.println(res); }
}

void setup() {
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  // Default data for read/write test
  char    newData[] = "Something new Data for write to EEPROM from buffer"; // New data for read/write test  
  uint8_t res = 0;                                                          // Variable for Result
  char    buff[128];                                                        // Buffer for reading default data
  char    buff2[128];                                                       // Buffer for reading data

  bool    endZero = false;
 
  Serial.print("New value for write \t"); eeprom.outBuffer(newData, (sizeof(newData) / sizeof(newData[0])), endZero);

  // Get Default Value from EEPROM
  res = eeprom.intRead(ADDR, buff, (sizeof(buff) / sizeof(buff[0])), endZero);           // Save default value
  Serial.print("Read data\t"); printInfo(res, buff, (sizeof(buff) / sizeof(buff[0])), endZero);
  
  // Write default data and return result of operation 
  res = eeprom.intWrite(ADDR, newData, (sizeof(newData) / sizeof(newData[0])), endZero);
  Serial.print("Write data\t"); printInfo(res, newData, (sizeof(newData) / sizeof(newData[0])), endZero);

  // Get value by address
  res = eeprom.intRead(ADDR, buff2, (sizeof(buff2) / sizeof(buff2[0])), endZero);
  Serial.print("Read data\t"); printInfo(res, buff2, (sizeof(buff2) / sizeof(buff2[0])), endZero);
  
  // Return value to address
  res = eeprom.intWrite(ADDR, buff, (sizeof(buff) / sizeof(buff[0])), endZero);
  Serial.print("Write data\t"); printInfo(res, buff, (sizeof(buff) / sizeof(buff[0])), endZero);

  // Get value by address
  res = eeprom.intRead(ADDR, buff2, (sizeof(buff2) / sizeof(buff2[0])), endZero);
  Serial.print("Read data\t"); printInfo(res, buff2, (sizeof(buff2) / sizeof(buff2[0])), endZero);
}

void loop() {}
