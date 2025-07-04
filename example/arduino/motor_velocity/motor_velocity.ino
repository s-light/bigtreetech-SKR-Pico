
void setup() {
    Serial.begin(115200);
    while (!Serial)
        yield();
    Serial.println(
        "bigtreetech SKR-Pico motor velocity example. `motor_velocity.ino`");
    ledSetup();
    motorSetup();
}


void loop() {
    ledUpdate();
    motorUpdate();
    // delay(DELAY);
}
