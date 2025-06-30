
void setup() {
    Serial.begin(115200);
    while (!Serial)
        delay(0);
    Serial.println("bigtreetech SKR-Pico basic example. `basic.ino`");
    sensorSetup();
    ledSetup();
    motorSetup();
}


void loop() {
    sensorUpdate();
    ledUpdate();
    motorUpdate();
    // delay(DELAY);
}
