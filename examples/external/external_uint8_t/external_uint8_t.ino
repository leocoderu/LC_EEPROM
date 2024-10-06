#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model eeprom

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  uint8_t newV = 0x0C;                          // Default data for read/write test
  uint8_t res  = 0;                             // Variable for Result
  uint8_t getV = 0;                             // Variable for getting Byte
  uint8_t defV = 0;                             // Save default value

  Serial.print("New value for write \t0x"); Serial.println(newV, HEX);

  res = eeprom.extRead(ADDR, defV);             // Save default value
  Serial.print("EEPROM value now \t0x"); Serial.print(defV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, newV);   
  Serial.print("Result operation write: 0x"); Serial.print(newV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, getV);     
  Serial.print("EEPROM value now \t0x"); Serial.print(getV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Return value to address
  res = eeprom.extWrite(ADDR, defV); 
  Serial.print("Result operation write: 0x"); Serial.print(defV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, getV);     
  Serial.print("EEPROM value now \t0x"); Serial.print(getV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");
}

void loop(){}
