#include <LC_EEPROM.h>

#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object eeprom

void setup() {
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  eeprom.extFill(ADDR, 0xFF, 0xFF);             // Fill external memory by template value
  eeprom.intFill(ADDR, 0xFF, 0xFF);             // Fill internal memory by template value

  eeprom.extShow(ADDR, ADDR + 0xFF, 32);        // Show Map of memory
  eeprom.intShow(ADDR, ADDR + 0xFF, 32);        // Show Map of memory

}

void loop() {}
