#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;

void setup(){
  Serial.begin(9600); delay(200);               // Delay for init EEPROM memory
  Serial.println();

  uint16_t newV = 64117;                        // Default data for read/write test
  uint8_t  res = 0;                             // Variable for Result
  uint16_t getV = 0;                            // Variable for getting Byte
  uint16_t defV = eeprom.intReadInt(ADDR);      // Save default value

  Serial.print("New value for write \t"); Serial.println(newV, HEX);
  Serial.print("EEPROM value now \t "); Serial.println(defV, HEX);

  // Write default data and return result of operation 
  res = eeprom.intWriteInt(ADDR, newV);   
  Serial.print("Result operation write: "); Serial.print(newV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  getV = eeprom.intReadInt(ADDR);     
  Serial.print("EEPROM value now \t"); Serial.println(getV, HEX);

  // Return value to address
  res = eeprom.intWriteInt(ADDR, defV); 
  Serial.print("Result operation write: "); Serial.print(defV, HEX);  Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  getV = eeprom.intReadInt(ADDR);     
  Serial.print("EEPROM value now \t"); Serial.println(getV, HEX);
}

void loop(){}
