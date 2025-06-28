// SPDX - FileCopyrightText : 2025 Stefan Krüger s-light.eu
// SPDX - License - Identifier : CC0
// written by Stefan Krüger for Arduino-Pico
// license: https://creativecommons.org/publicdomain/zero/1.0/

// loosely inspired by these sketches..
// Rui Santos
// https://randomnerdtutorials.com/raspberry-pi-pico-i2c-scanner-arduino/
// TWI_I2C_Scanner.ino
// I2CScanner.pde
// 2009, Tod E. Kurt, http://todbot.com/blog/
// http://todbot.com/blog/2009/11/29/i2cscanner-pde-arduino-as-i2c-bus-scanner/comment-page-1/
// http://arduino.cc/forum/index.php/topic,95067.msg714094.html#msg714094
// https://getsatisfaction.com/thingm/topics/cant_get_blinkm_communicator_to_compile_and_upload

#include "Wire.h"

// for endTransmission return codes:
// https://docs.arduino.cc/language-reference/en/functions/communication/wire/endTransmission/#returns
const uint8_t WIRE_TRANSMISSION_SUCCESS = 0;
const uint8_t WIRE_TRANSMISSION_DATATOLONG = 1;
const uint8_t WIRE_TRANSMISSION_NACKONADDRESS = 2;
const uint8_t WIRE_TRANSMISSION_NACKONDATA = 3;
const uint8_t WIRE_TRANSMISSION_OTHERERROR = 4;
const uint8_t WIRE_TRANSMISSION_TIMEOUT = 5;

void printHEX(uint8_t value) {
    Serial.print("0x");
    if (value < 16) {
        Serial.print("0");
    }
    Serial.print(value, HEX);
}

// Scan the I2C bus
int scanI2CBus(TwoWire &wireX) {

    int deviceCount;
    Serial.println("Scanning...");
    deviceCount = 0;
    for (uint8_t address = 1; address < 127; address++) {
        uint8_t error;
        wireX.beginTransmission(address);
        error = wireX.endTransmission();
        switch (error) {
        case WIRE_TRANSMISSION_SUCCESS: {
            Serial.print("I2C device found at address ");
            printHEX(address);
            Serial.println();
            deviceCount++;
        } break;
        case WIRE_TRANSMISSION_DATATOLONG: {
            Serial.print("error: Data to Long.");
        } break;
        case WIRE_TRANSMISSION_NACKONADDRESS: {
            // Serial.print("error: NACK on address transmission");
        } break;
        case WIRE_TRANSMISSION_NACKONDATA: {
            Serial.print("error: NACK on data transmission");
        } break;
        case WIRE_TRANSMISSION_OTHERERROR: {
            // Serial.print("error: unknown.");
        } break;
        case WIRE_TRANSMISSION_TIMEOUT: {
            // Serial.print("error: timeout");
        } break;
        }
    }
    if (deviceCount == 0) {
        Serial.println("No I2C devices found");
    } else {
        Serial.println("done");
    }
    Serial.println("------------------------------------------\n");
    return deviceCount;
}

void setup() {
    delay(1000);
    while (!Serial);
    Serial.begin(115200);

    Serial.println();
    Serial.println("Wire-I2C-Scanner.ino");
    Serial.println();

    Serial.println("setup `Wire`");
    Wire.begin();

    Serial.println("setup `Wire1`");
    Wire1.begin();

    Serial.println("\nI2CScanner ready!");
    delay(2000);
}

void loop() {

    Serial.print("Wire0 ");
    scanI2CBus(Wire);
    Serial.print("Wire1 ");
    scanI2CBus(Wire1);

    delay(3000);
}
