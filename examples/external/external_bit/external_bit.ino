#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model eeprom

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  uint8_t nBit = 3;                             // Number bit for change
  bool    state = false;                        // State of bit in byte
   
  uint8_t res  = 0;                             // Variable for Result
  bool    getV = false;                         // Getting bit in Byte
  bool    defV = false;                         // Save default value of state

  uint8_t bt = 0;                               // Byte where we are will be change bit
  res = eeprom.extRead(ADDR, bt);             
  Serial.print("EEPROM value now \t"); Serial.print(bt, BIN); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  //Serial.print("Change number bit in byte\t"); Serial.println(nBit);

  res = eeprom.extRead(ADDR, nBit, defV);       // Save default value
  Serial.print("Save default value bit"); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, nBit, state);   
  Serial.print("Save new state: "); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  res = eeprom.extRead(ADDR, bt);             
  Serial.print("EEPROM value now \t"); Serial.print(bt, BIN); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, nBit, getV);     
  Serial.print("Now state is "); Serial.print(getV ? "true" : "false"); Serial.println(res == 0 ? "\tsuccess" : "\terror");  

  // Return value to address
  res = eeprom.extWrite(ADDR, nBit, defV); 
  Serial.print("Result operation write: "); Serial.println(res == 0 ? "success" : "error");

  res = eeprom.extRead(ADDR, bt);             
  Serial.print("EEPROM value now \t"); Serial.print(bt, BIN); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, nBit, getV);     
  Serial.print("Result operation read"); Serial.println(res == 0 ? "\tsuccess" : "\terror");
}

void loop(){}