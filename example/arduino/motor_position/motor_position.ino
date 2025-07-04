#include <Adafruit_TinyUSB.h>
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(1);
    }
    
    Serial.println("bigtreetech SKR-Pico motor position example.");
    Serial.println("`motor_position.ino`");

    ledSetup();
    motorSetup();
}

void loop() {
    ledUpdate();
    motorUpdate();
    // delay(DELAY);
}
