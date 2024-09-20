#include <LC_EEPROM.h>
#define ADDR 0x400                              // Address byte for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model and quantity of modules eeprom
  
void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  uint8_t n = 0x0A;                             // Default data for read/write test
  uint8_t res = 0;                              // Variable for Result
  uint8_t gByte = 0;                            // Variable for getting Byte
  uint8_t def = eeprom.extReadByte(ADDR);       // Save default value

  Serial.print("New value for write \t0x"); Serial.println(n, HEX);
  Serial.print("EEPROM value now \t0x"); Serial.println(def, HEX);

  // Write default data and return result of operation 
  res = eeprom.extWriteByte(ADDR, n);   
  Serial.print("Result operation write: 0x"); Serial.print(n, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  gByte = eeprom.extReadByte(ADDR);     
  Serial.print("EEPROM value now \t0x"); Serial.println(gByte, HEX);

  // Return value to address
  res = eeprom.extWriteByte(ADDR, def); 
  Serial.print("Result operation write: 0x"); Serial.print(def, HEX);  Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  gByte = eeprom.extReadByte(ADDR);     
  Serial.print("EEPROM value now \t0x"); Serial.println(gByte, HEX);
}

void loop(){}
