#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;                               // Create object eeprom

void _printUInt64(const uint64_t& v) {
    String res = "";

    uint64_t n = v;
    while ((n / 10) > 0) {
        uint8_t s = n % 10;
        res = s + res;
        n -= s;
        n /= 10;
    }

    uint8_t s = n % 10;
    res = s + res;

    Serial.print(res);
}

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  uint64_t newV = 345131122212245UL;            // Default data for read/write test
  uint8_t res  = 0;                             // Variable for Result
  uint64_t getV = 0;                            // Variable for getting Byte
  uint64_t defV = 0;                            // Save default value

  Serial.print("New value for write \t"); _printUInt64(newV); Serial.println();

  res = eeprom.intRead(ADDR, defV);             // Save default value
  Serial.print("EEPROM value now \t"); _printUInt64(defV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Write default data and return result of operation 
  res = eeprom.intWrite(ADDR, newV);   
  Serial.print("Result operation write: "); _printUInt64(newV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.intRead(ADDR, getV);     
  Serial.print("EEPROM value now \t"); _printUInt64(getV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Return value to address
  res = eeprom.intWrite(ADDR, defV); 
  Serial.print("Result operation write: "); _printUInt64(defV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.intRead(ADDR, getV);     
  Serial.print("EEPROM value now \t"); _printUInt64(getV); Serial.println(res == 0 ? "\tsuccess" : "\terror");
}

void loop(){}
