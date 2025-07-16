#include <Adafruit_TinyUSB.h>
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    uint32_t startTime = millis();
    while (!Serial && !((millis() - startTime) > 2000)) {
        delay(1);
    }

    Serial.println("bigtreetech SKR-Pico motor position example.");
    Serial.println("`motor_position.ino`");

    Wire.begin();
    capSetup();
    ledSetup();
    motorSetup();
}

void checkInput() {
    if (Serial.available()) {
        char input = Serial.read();

        Serial.print("got '");
        Serial.print(input);
        Serial.println("'");
        switch (input) {
        case 'h': {
            moveToHome();
        } break;
        case 'j': {
            jump();
        } break;
        }
    }
}

void loop() {
    checkInput();
    capUpdate();
    ledUpdate();
    motorUpdate();
    // delay(DELAY);
}
