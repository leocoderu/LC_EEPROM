#include <LC_EEPROM.h>
#define ADDR 0x400                              // Address byte for test

LC_EXT_EEPROM eeprom(_24LC512, 3);              // Create object with model and quantity of modules eeprom

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  uint16_t n = 0x0ABF;                          // Default data for read/write test
  uint8_t res = 0;                              // Variable for Result
  uint16_t gVal = 0;                            // Variable for getting Byte
  uint16_t def = eeprom.extReadInt(ADDR);       // Save default value

  Serial.print("New value for write \t0x"); Serial.println(n, HEX);
  Serial.print("EEPROM value now \t0x"); Serial.println(def, HEX);

  // Write default data and return result of operation 
  res = eeprom.extWriteInt(ADDR, n);   
  Serial.print("Result operation write: 0x"); Serial.print(n, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  gVal = eeprom.extReadInt(ADDR);     
  Serial.print("EEPROM value now \t0x"); Serial.println(gVal, HEX);

  // Return value to address
  res = eeprom.extWriteInt(ADDR, def); 
  Serial.print("Result operation write: 0x"); Serial.print(def, HEX);  Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  gVal = eeprom.extReadInt(ADDR);     
  Serial.print("EEPROM value now \t0x"); Serial.println(gVal, HEX);
}

void loop(){}
