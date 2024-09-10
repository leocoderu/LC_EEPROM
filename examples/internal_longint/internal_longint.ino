#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;

void setup(){
  Serial.begin(9600); delay(200);               // Delay for init EEPROM memory
  Serial.println("begins....");

  uint32_t  newV = 0x001AC622;                  // Default data for read/write test
  uint8_t   res = 0;                            // Variable for Result
  uint32_t  gLong = 0;                          // Variable for getting Byte
  uint32_t  defV = eeprom.intReadLong(ADDR);    // Save default value

  Serial.print("New value for write \t0x"); Serial.println(newV, HEX);
  Serial.print("EEPROM value now \t0x"); Serial.println(defV, HEX);

  // Write default data and return result of operation 
  res = eeprom.intWriteLong(ADDR, newV);   
  Serial.print("Write new value \t0x"); Serial.print(newV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  gLong = eeprom.intReadLong(ADDR);     
  Serial.print("EEPROM value now \t0x"); Serial.println(gLong, HEX);

  // Return value to address
  res = eeprom.intWriteLong(ADDR, defV); 
  Serial.print("Write previous value \t0x"); Serial.print(defV, HEX); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  gLong = eeprom.intReadLong(ADDR);     
  Serial.print("EEPROM value now \t0x"); Serial.println(gLong, HEX);
}

void loop(){}
