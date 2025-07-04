#include <Adafruit_TinyUSB.h>
#include <Wire.h>

Adafruit_USBD_CDC USBSer1;

void setup() {
    Serial.begin(115200);

    while (!Serial)
        yield();
    Serial.println("bigtreetech SKR-Pico basic example. `basic.ino`");

    // serialoutput for plotting data
    USBSer1.begin(115200);

    // Wire.setSDA(D20); // LASER_PWM
    // Wire.setSCL(D29); // PIN_SERVOS
    Wire.begin();

    capSetup();
    // accelSetup();
    ledSetup();
    motorSetup();
}

void loop() {
    capUpdate();
    // accelUpdate();
    ledUpdate();
    motorUpdate();
}
