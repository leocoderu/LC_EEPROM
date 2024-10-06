#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;                               // Create object eeprom

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  double  newV = -1453.12565634;                // Default data for read/write test
  uint8_t res  = 0;                             // Variable for Result
  double  getV = 0;                             // Variable for getting Byte
  double  defV = 0;                             // Save default value

  Serial.print("New value for write \t"); Serial.println(newV);

  res = eeprom.intRead(ADDR, defV);             // Save default value
  Serial.print("EEPROM value now \t"); Serial.print(defV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Write default data and return result of operation 
  res = eeprom.intWrite(ADDR, newV);   
  Serial.print("Result operation write: "); Serial.print(newV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.intRead(ADDR, getV);     
  Serial.print("EEPROM value now \t"); Serial.print(getV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Return value to address
  res = eeprom.intWrite(ADDR, defV); 
  Serial.print("Result operation write: "); Serial.print(defV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.intRead(ADDR, getV);     
  Serial.print("EEPROM value now \t"); Serial.print(getV); Serial.println(res == 0 ? "\tsuccess" : "\terror");
}

void loop(){}
