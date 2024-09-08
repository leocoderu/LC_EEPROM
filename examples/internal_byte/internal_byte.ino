#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;

void setup(){
  Serial.begin(9600); Serial.println();

  uint8_t n = 12;                               // Default data for read/write test
  uint8_t res = 0;                              // Variable for Result
  uint8_t gByte = 0;                            // Variable for getting Byte
  uint8_t def = eeprom.intReadByte(ADDR);       // Save default value

  Serial.print("Default Value for write: \t"); Serial.println(n);
  Serial.print("Value from address 0x"); Serial.print(ADDR, HEX); Serial.print(" is: \t"); Serial.println(def);

  // Write default data and return result of operation 
  res = eeprom.intWriteByte(ADDR, n);   
  Serial.print("Result operation write: \t"); Serial.println(res == 0 ? "success" : "error");

  // Get value by address
  gByte = eeprom.intReadByte(ADDR);     
  Serial.print("Now value by address 0x"); Serial.print(ADDR, HEX); Serial.print(" is: \t"); Serial.println(gByte);

  // Return value to address
  res = eeprom.intWriteByte(ADDR, def); 
  Serial.print("Result operation write: \t"); Serial.println(res == 0 ? "success" : "error");

  // Get value by address
  gByte = eeprom.intReadByte(ADDR);     
  Serial.print("Now value by address 0x"); Serial.print(ADDR, HEX); Serial.print(" is: \t"); Serial.println(gByte);
}

void loop(){}
