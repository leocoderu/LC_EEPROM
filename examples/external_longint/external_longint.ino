#include <LC_EEPROM.h>
#define ADDR 0x400                              // Address byte for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model and quantity of modules eeprom

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  uint32_t n = 0xABCD0ABC;                      // Default data for read/write test
  uint8_t  res = 0;                             // Variable for Result
  uint32_t gVal = 0;                            // Variable for getting Byte
  uint32_t def = 0;                             // Save default value

  Serial.print("New value for write \t0x"); Serial.println(n, HEX);
  
  res = eeprom.extRead(ADDR, def);              // Save default value
  Serial.print("EEPROM value now \t0x"); Serial.print(def, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, n);   
  Serial.print("Result operation write: 0x"); Serial.print(n, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, gVal);     
  Serial.print("EEPROM value now \t0x"); Serial.print(gVal, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Return value to address
  res = eeprom.extWrite(ADDR, def); 
  Serial.print("Result operation write: 0x"); Serial.print(def, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, gVal);     
  Serial.print("EEPROM value now \t0x"); Serial.print(gVal, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");
}

void loop(){}
