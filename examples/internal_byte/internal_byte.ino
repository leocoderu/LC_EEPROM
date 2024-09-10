#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;

void setup(){
  Serial.begin(9600); delay(200);               // Delay for init EEPROM memory
  Serial.println("begins....");

  uint8_t n = 12;                               // Default data for read/write test
  uint8_t res = 0;                              // Variable for Result
  uint8_t gByte = 0;                            // Variable for getting Byte
  uint8_t def = eeprom.intReadByte(ADDR);       // Save default value

  Serial.print("New value for write \t0x"); Serial.println(n, HEX);
  Serial.print("EEPROM value now \t0x"); Serial.println(def, HEX);

  // Write default data and return result of operation 
  res = eeprom.intWriteByte(ADDR, n);   
  Serial.print("Result operation write: \t"); Serial.print(n, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  gByte = eeprom.intReadByte(ADDR);     
  Serial.print("EEPROM value now \t"); Serial.println(gByte, HEX);

  // Return value to address
  res = eeprom.intWriteByte(ADDR, def); 
  Serial.print("Result operation write: \t"); Serial.print(def, HEX);  Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  gByte = eeprom.intReadByte(ADDR);     
  Serial.print("EEPROM value now \t"); Serial.println(gByte, HEX);
}

void loop(){}
