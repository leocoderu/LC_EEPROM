#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;

void printBuffByte(uint8_t* src, const uint8_t& sz) {
  Serial.print("[");
  for (uint8_t n = 0; n < sz; n++) {
    if (src[n] < 16) Serial.print("0");
    Serial.print(src[n], HEX);   
    Serial.print(" ");
  }
  Serial.println("]");
}

void setup() {
  Serial.begin(9600); delay(500);                                         // Delay for init EEPROM memory
  Serial.println();

  // Default data for read/write test
  uint8_t newData[] = { 0x0C, 0x18, 0x0F, 0x61, 0x86, 0x04, 0x5F, 0xD7, 0x2F, 0x4B, 0x5D, 0x1A };  // Default data for read/write test - 12 bytes
  uint8_t res = 0;                                                        // Variable for Result
  uint8_t buff[64];                                                       // Buffer for reading default data
  uint8_t buff2[64];                                                      // Buffer for reading data
   
  Serial.print("New value for write \t"); printBuffByte(newData, sizeof(newData));

  // Get Default Value from EEPROM
  res = eeprom.intRead(ADDR, buff, sizeof(buff));                    // Save default value
  Serial.print("EEPROM data now \t"); printBuffByte(buff, sizeof(buff));
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Write default data and return result of operation 
  res = eeprom.intWrite(ADDR, newData, sizeof(newData));
  Serial.print("Result of writing data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Get value by address
  res = eeprom.intRead(ADDR, buff2, sizeof(buff2));
  Serial.print("EEPROM data now \t"); printBuffByte(buff2, sizeof(buff2));
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");
  
  // Return value to address
  res = eeprom.intWrite(ADDR, buff, sizeof(buff));
  Serial.print("Result of writing data: "); Serial.println(res == 0 ? "success\n" : "error\n");

  // Get value by address
  res = eeprom.intRead(ADDR, buff2, sizeof(buff2));
  Serial.print("EEPROM data now \t"); printBuffByte(buff2, sizeof(buff2)); 
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? "success\n" : "error\n");
}

void loop() {}
