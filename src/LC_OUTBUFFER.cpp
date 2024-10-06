#include "LC_EEPROM.h"

void LC_EEPROM::outBuffer(char* src, const uint8_t& sz, bool endZero) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        if (endZero && (src[n] == '\0')) break;
        Serial.print(src[n]);
    }
    Serial.println(" ]");
}

void LC_EEPROM::outBuffer(int8_t* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        Serial.print(src[n], DEC);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(uint8_t* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        if (src[n] < 16) Serial.print("0");
        Serial.print(src[n], HEX);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(int16_t* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        Serial.print(src[n], DEC);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(uint16_t* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        if (src[n] < 4096) Serial.print("0");
        if (src[n] < 256) Serial.print("0");
        if (src[n] < 16) Serial.print("0");
        Serial.print(src[n], HEX);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(int32_t* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        Serial.print(src[n], DEC);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(uint32_t* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        if (src[n] < 268435456) Serial.print("0");
        if (src[n] < 16777216) Serial.print("0");
        if (src[n] < 1048576) Serial.print("0");
        if (src[n] < 65536) Serial.print("0");
        if (src[n] < 4096) Serial.print("0");
        if (src[n] < 256) Serial.print("0");
        if (src[n] < 16) Serial.print("0");
        Serial.print(src[n], HEX);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(int64_t* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        _print64(src[n]);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(uint64_t* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        uint32_t high = (uint32_t)(src[n] >> 32);
        uint32_t low = (uint32_t)(src[n]);

        if (high < 268435456) Serial.print("0");
        if (high < 16777216) Serial.print("0");
        if (high < 1048576) Serial.print("0");
        if (high < 65536) Serial.print("0");
        if (high < 4096) Serial.print("0");
        if (high < 256) Serial.print("0");
        if (high < 16) Serial.print("0");

        Serial.print(high, HEX);

        if (low < 268435456) Serial.print("0");
        if (low < 16777216) Serial.print("0");
        if (low < 1048576) Serial.print("0");
        if (low < 65536) Serial.print("0");
        if (low < 4096) Serial.print("0");
        if (low < 256) Serial.print("0");
        if (low < 16) Serial.print("0");

        Serial.print(low, HEX);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(float* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        Serial.print(src[n]);
        Serial.print(" ");
    }
    Serial.println("]");
}

void LC_EEPROM::outBuffer(double* src, const uint8_t& sz) {
    Serial.print("[ ");
    for (uint8_t n = 0; n < sz; n++) {
        Serial.print(src[n]);
        Serial.print(" ");
    }
    Serial.println("]");
}