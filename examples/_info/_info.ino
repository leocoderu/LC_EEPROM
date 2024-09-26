#include <LC_EEPROM.h>

LC_EXT_EEPROM extEeprom(_24LC512); // Create object with model eeprom

LC_EEPROM intEeprom;               // Create object eeprom

void setup(){
  Serial.begin(9600); delay(500);  // Delay for init EEPROM memory
  Serial.println();

  extEeprom.begin();               // Start working with eeprom

  intEeprom.info();
  Serial.println();
  
  extEeprom.info();
}

void loop(){}
