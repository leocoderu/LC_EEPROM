#include <LC_EEPROM.h>
#define ADDR 0x0400                             // Address byte for test

LC_EEPROM eeprom;

void printBuffChar(char* src, const uint8_t& sz) {
  Serial.print("[");
  for (uint8_t n = 0; n < sz; n++) Serial.print(src[n]);  
  Serial.println("]");
}

void setup() {
	Serial.begin(9600); delay(200);                                         // Delay for init EEPROM memory
	Serial.println();

	uint8_t   res = 0;                                                      // Variable for Result
	char buff[64];                                                          // Buffer for reading default data
  char buff2[64];                                                         // Buffer for reading data
	char newData[] = "Something new Data for write to EEPROM from buffer";  // Default data for read/write test	
	
	res = eeprom.intReadBlock(ADDR, buff, sizeof(buff));                    // Save default value
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? " success" : " error");
  
  
	Serial.print("New value for write \t"); printBuffChar(newData, sizeof(newData));
	Serial.print("EEPROM data now \t"); printBuffChar(buff, sizeof(buff)); Serial.println();

  //Serial.print("Size of buff: "); Serial.println(sizeof(newData));

	// Write default data and return result of operation 
	res = eeprom.intWriteBlock(ADDR, newData, sizeof(newData));
	Serial.print("Result of writing data: "); Serial.println(res == 0 ? " success" : " error");

	// Get value by address
	res = eeprom.intReadBlock(ADDR, buff2, sizeof(buff2));
	Serial.print("Result of reading data: "); Serial.println(res == 0 ? " success" : " error");
  printBuffChar(buff2, sizeof(buff2)); Serial.println();

	// Return value to address
	res = eeprom.intWriteBlock(ADDR, buff, sizeof(buff));
	Serial.print("Result of writing data: "); Serial.println(res == 0 ? " success" : " error");

	// Get value by address
  res = eeprom.intReadBlock(ADDR, buff2, sizeof(buff2));
  Serial.print("Result of reading data: "); Serial.println(res == 0 ? " success" : " error");
  printBuffChar(buff2, sizeof(buff2)); Serial.println();

  eeprom.intShow(ADDR, ADDR + 0x3F, 32);
}

void loop() {}
