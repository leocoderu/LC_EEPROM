#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model eeprom

void setup() {
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  // Default data for read/write test
  int8_t   newData[] = {-128, 17, 0, -65, 41, 19, -71, -93, 86, 123, 4, 127};// Default data for read/write test  
  uint8_t  res = 0;                                                          // Variable for Result
  uint8_t  uBuff[32];                                                        // Buffer for reading default data
  uint8_t  uBuff2[32];                                                       // Buffer for reading data
  Serial.print("New value for write \t"); eeprom.outBuffer(newData, sizeof(newData));

  // Get Default Value from EEPROM
  res = eeprom.extRead(ADDR, uBuff, sizeof(uBuff));                    // Save default value
  Serial.print("EEPROM data now \t"); eeprom.outBuffer(uBuff, sizeof(uBuff));
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, newData, sizeof(newData));
  Serial.print("Result of writing data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Get value by address
  res = eeprom.extRead(ADDR, uBuff2, sizeof(uBuff2));
  Serial.print("EEPROM data now \t"); eeprom.outBuffer(uBuff2, sizeof(uBuff2));
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");
  
  // Return value to address
  res = eeprom.extWrite(ADDR, uBuff, sizeof(uBuff));
  Serial.print("Result of writing data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Get value by address
  res = eeprom.extRead(ADDR, uBuff2, sizeof(uBuff2));
  Serial.print("EEPROM data now \t"); eeprom.outBuffer(uBuff2, sizeof(uBuff2)); 
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  //eeprom.extShow(ADDR, ADDR + 0xFF, 32);
}

void loop() {}