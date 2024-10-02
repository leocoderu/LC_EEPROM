#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model eeprom

void setup() {
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  // Default data for read/write test
  char    newData[] = "Something new Data for write to EEPROM from buffer"; // Default data for read/write test  
  uint8_t res = 0;                                                          // Variable for Result
  char    buff[64];                                                         // Buffer for reading default data
  char    buff2[64];                                                        // Buffer for reading data
  Serial.print("New value for write \t"); eeprom.outBuffer(newData, sizeof(newData));

  // Get Default Value from EEPROM
  res = eeprom.extRead(ADDR, buff, sizeof(buff));                    // Save default value
  Serial.print("EEPROM data now \t"); eeprom.outBuffer(buff, sizeof(buff));
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, newData, sizeof(newData));
  Serial.print("Result of writing data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Get value by address
  res = eeprom.extRead(ADDR, buff2, sizeof(buff2));
  Serial.print("EEPROM data now \t"); eeprom.outBuffer(buff2, sizeof(buff2));
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");
  
  // Return value to address
  res = eeprom.extWrite(ADDR, buff, sizeof(buff));
  Serial.print("Result of writing data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Get value by address
  res = eeprom.extRead(ADDR, buff2, sizeof(buff2));
  Serial.print("EEPROM data now \t"); eeprom.outBuffer(buff2, sizeof(buff2)); 
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");
}

void loop() {}
