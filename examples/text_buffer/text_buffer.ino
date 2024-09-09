char* info = "This is Long Long String, o it is not String it's array of char!!!";

void intReadBlockChar(const uint16_t& addr, char* dst, const uint8_t& szDst) {
	memset(dst, INT8_MIN, szDst);                 // ���������� ������� INT8_MIN
	for (uint8_t i = 0; i < szDst; i++) {
		uint8_t bt = info[addr + i];
		if (bt == INT8_MIN) break;  // �������� ������ 0xFF ����� ������� defVal, �.�. ��� char ������ ���� ����� �� ������
		dst[i] = bt;
	}
}

void printBuff(char* src, const uint8_t& sz) {
	Serial.print("Buff: [");
	for (uint8_t n = 0; n < sz; n++) Serial.print(src[n]);	
	Serial.println("]");
}

void setup()
{
	Serial.begin(9600); delay(200);
	Serial.println("Hello from Arduino!");

	char buff[30];
	intReadBlockChar(0, buff, sizeof(buff));
	printBuff(buff, sizeof(buff));
}

void loop(){}
