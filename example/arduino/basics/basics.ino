
void setup() {
    Serial.begin(115200);
    while (!Serial)
        yield();
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
