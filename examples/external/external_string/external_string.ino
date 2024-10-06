#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model eeprom

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  // Default data for read/write test
  String  newV = "Testing long long long string, its the biggest testing string #1 Testing long long long string, its the biggest testing string #2 Testing long long long string, its the biggest testing string #3";
  uint8_t res  = 0;                             // Variable for Result
  String  getV = "";                            // Variable for getting Byte
  String  defV = "";                            // Save default value

  Serial.print("New value for write \t"); Serial.println(newV);
  
  res = eeprom.extRead(ADDR, defV, 194);        // Save default value
  Serial.print("EEPROM value now \t"); Serial.println(defV);
  Serial.println(res == 0 ? "Result: success\n" : "Result: error\n");

  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, newV);   
  Serial.print("Result operation write: "); Serial.println(newV);
  Serial.println(res == 0 ? "Result: success\n" : "Result: error\n");

  // Get value by address
  res = eeprom.extRead(ADDR, getV, 194);     
  Serial.print("EEPROM value now \t"); Serial.println(getV);
  Serial.println(res == 0 ? "Result: success\n" : "Result: error\n");

  // Return value to address
  res = eeprom.extWrite(ADDR, defV); 
  Serial.print("Result operation write: "); Serial.println(defV);
  Serial.println(res == 0 ? "Result: success\n" : "Result: error\n");

  // Get value by address
  res = eeprom.extRead(ADDR, getV, 194);     
  Serial.print("EEPROM value now \t"); Serial.println(getV);
  Serial.println(res == 0 ? "Result: success\n" : "Result: error\n");

  //eeprom.intShow(ADDR, ADDR + 0xFF, 32);
}

void loop(){}
