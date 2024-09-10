#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;

void setup(){
  Serial.begin(9600); delay(200);               // Delay for init EEPROM memory
  Serial.println();

  //eeprom.intFillBlock(ADDR, 0xFF, 0xFF);      // Clear memory
  
  // Default data for read/write test
  String  newV = "Testing long long long string, its the biggest testing string #1 Testing long long long string, its the biggest testing string #2 Testing long long long string, its the biggest testing string #3";
  uint8_t res = 0;                              // Variable for Result
  String  getV  = "";                           // Variable for getting Byte
  String  defV = eeprom.intReadStr(ADDR, 194);  // Save default value

  Serial.print("New value for write \t"); Serial.println(newV);
  Serial.print("EEPROM value now \t"); Serial.println(defV);

  // Write default data and return result of operation 
  res = eeprom.intWriteStr(ADDR, newV);   
  Serial.print("Write new value "); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  getV = eeprom.intReadStr(ADDR, 194);     
  Serial.print("EEPROM value now \t"); Serial.println(getV);

  // Return value to address
  res = eeprom.intWriteStr(ADDR, defV); 
  Serial.print("Write previous value ");Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  getV = eeprom.intReadStr(ADDR, 194);     
  Serial.print("EEPROM value now \t"); Serial.println(getV);

  eeprom.intShow(ADDR, ADDR + 0xFF, 32);
}

void loop(){}
