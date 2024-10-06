#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EXT_EEPROM eeprom(_24LC512);                 // Create object with model eeprom

void _printInt64(const int64_t& v) {
    bool minus = (v / abs(v)) < 0;
    String res = "";

    int64_t n = minus ? (v * -1) : v;
    while ((n / 10) > 0) {
        uint8_t s = n % 10;
        res = s + res;
        n -= s;
        n /= 10;
    }

    uint8_t s = n % 10;
    res = s + res;

    if (minus) res = '-' + res;
    Serial.print(res);
}

void setup(){
  Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
  Serial.println();

  eeprom.begin();                               // Start working with eeprom

  int64_t newV = -131122212245;                 // Default data for read/write test
  uint8_t res  = 0;                             // Variable for Result
  int64_t getV = 0;                             // Variable for getting Byte
  int64_t defV = 0;                             // Save default value

  Serial.print("New value for write \t"); _printInt64(newV); Serial.println();

  res = eeprom.extRead(ADDR, defV);             // Save default value
  Serial.print("EEPROM value now \t"); _printInt64(defV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Write default data and return result of operation 
  res = eeprom.extWrite(ADDR, newV);   
  Serial.print("Result operation write: "); _printInt64(newV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, getV);     
  Serial.print("EEPROM value now \t"); _printInt64(getV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Return value to address
  res = eeprom.extWrite(ADDR, defV); 
  Serial.print("Result operation write: "); _printInt64(defV); Serial.println(res == 0 ? "\tsuccess" : "\terror");

  // Get value by address
  res = eeprom.extRead(ADDR, getV);     
  Serial.print("EEPROM value now \t"); _printInt64(getV); Serial.println(res == 0 ? "\tsuccess" : "\terror");
}

void loop(){}
