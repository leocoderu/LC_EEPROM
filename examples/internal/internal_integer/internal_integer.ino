#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  uint16_t newV = 0xFA75;                       // Default data for read/write test
  uint8_t  res  = 0;                            // Variable for Result
  uint16_t getV = 0;                            // Variable for getting Byte
  uint16_t defV = 0;                            // Save default value

  Serial.print("New value for write \t0x"); Serial.println(newV, HEX);

  res = eeprom.intRead(ADDR, defV);             // Save default value
  Serial.print("EEPROM value now \t0x"); Serial.print(defV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Write default data and return result of operation 
  res = eeprom.intWrite(ADDR, newV);   
  Serial.print("Result operation write: 0x"); Serial.print(newV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.intRead(ADDR, getV);     
  Serial.print("EEPROM value now \t0x"); Serial.print(getV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Return value to address
  res = eeprom.intWrite(ADDR, defV); 
  Serial.print("Result operation write: 0x"); Serial.print(defV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.intRead(ADDR, getV);     
  Serial.print("EEPROM value now \t0x"); Serial.print(getV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");
}

void loop(){}
