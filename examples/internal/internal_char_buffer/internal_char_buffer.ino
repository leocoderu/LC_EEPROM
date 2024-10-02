#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address for test

LC_EEPROM eeprom;                               // Create object eeprom

void setup() {
	Serial.begin(9600); delay(500);               // Delay for init EEPROM memory
	Serial.println();

  // Default data for read/write test
  char    newData[] = "Something new Data for write to EEPROM from buffer"; // New data for read/write test  
	uint8_t res = 0;                                                          // Variable for Result
	char    buff[64];                                                         // Buffer for reading default data
  char    buff2[64];                                                        // Buffer for reading data
	
  Serial.print("New value for write \t"); eeprom.outBuffer(newData, sizeof(newData));

  // Get Default Value from EEPROM
  res = eeprom.intRead(ADDR, buff, sizeof(buff));                           // Save default value
  Serial.print("Read data\t"); Serial.print(res == 0 ? "success\t" : "error\t"); eeprom.outBuffer(buff, sizeof(buff));
  
  // Write new data and return result of operation 
  res = eeprom.intWrite(ADDR, newData, sizeof(newData));
  Serial.print("Write data\t"); Serial.println(res == 0 ? "success\n" : "error\n");

  // Get value by address
  res = eeprom.intRead(ADDR, buff2, sizeof(buff2));
  Serial.print("Read data\t"); Serial.print(res == 0 ? "success\t" : "error\t"); eeprom.outBuffer(buff2, sizeof(buff2));
  
  // Return value to address
  res = eeprom.intWrite(ADDR, buff, sizeof(buff));
  Serial.print("Write data\t"); Serial.println(res == 0 ? "success\n" : "error\n");

  // Get value by address
  res = eeprom.intRead(ADDR, buff2, sizeof(buff2));
  Serial.print("Read data\t"); Serial.print(res == 0 ? "success\t" : "error\t"); eeprom.outBuffer(buff2, sizeof(buff2)); 
}

void loop() {}
