#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model eeprom

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  int16_t newV = -1345;                         // Default data for read/write test
  uint8_t res  = 0;                             // Variable for Result
  int16_t getV = 0;                             // Variable for getting Byte
  int16_t defV = 0;                             // Save default value

  Serial.print("New value for write \t"); Serial.println(newV);

  res = eeprom.extRead(ADDR, defV);             // Save default value
  Serial.print("EEPROM value now \t"); Serial.print(defV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, newV);   
  Serial.print("Result operation write: "); Serial.print(newV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, getV);     
  Serial.print("EEPROM value now \t"); Serial.print(getV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Return value to address
  res = eeprom.extWrite(ADDR, defV); 
  Serial.print("Result operation write: "); Serial.print(defV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, getV);     
  Serial.print("EEPROM value now \t"); Serial.print(getV); Serial.println(res == 0 ? "\tsuccess" : "\terror");
}

void loop(){}
