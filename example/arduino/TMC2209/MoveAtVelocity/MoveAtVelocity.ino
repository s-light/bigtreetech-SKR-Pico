#include <TMC2209.h>

// This example will not work on Arduino boards without HardwareSerial ports,
// such as the Uno, Nano, and Mini.
//
// See this reference for more details:
// https://www.arduino.cc/reference/en/language/functions/communication/serial/


const int DELAY = 200;
const int32_t VELOCITY = 200000;
// current values may need to be reduced to prevent overheating depending on
// specific motor and power supply voltage
const uint8_t RUN_CURRENT_PERCENT = 11;

// Instantiate TMC2209
TMC2209 driverX;
uint32_t velocityX = 0;

void setup() {
    Serial.begin(115200);

    driverX.setup(Serial2, 115200, TMC2209::SERIAL_ADDRESS_0);
    driverX.setHardwareEnablePin(PIN_XEN);
    
    delay(DELAY);
    driverX.setRunCurrent(RUN_CURRENT_PERCENT);
    driverX.setRMSCurrent(670, 0.11);
    //driverX.useInternalSenseResistors();
    // https://github.com/janelia-arduino/TMC2209?tab=readme-ov-file#standstill-mode
    driverX.setStandstillMode(TMC2209::FREEWHEELING);

    driverX.enableAutomaticCurrentScaling();
    driverX.enableCoolStep();

    driverX.enable();
    setXVelocity(VELOCITY);

    Serial.print("Moving at velocity ");
    Serial.println(VELOCITY);

    uint32_t interstep_duration = driverX.getInterstepDuration();
    Serial.print("which is equal to an interstep_duration of ");
    Serial.println(interstep_duration);
}

void loop() {
    if (not driverX.isSetupAndCommunicating()) {
        Serial.println("Stepper driver not setup and communicating!");
        return;
    }

    bool hardware_disabled = driverX.hardwareDisabled();
    TMC2209::Settings settings = driverX.getSettings();
    TMC2209::Status status = driverX.getStatus();

    if (hardware_disabled) {
        Serial.println("Stepper driver is hardware disabled!");
    } else if (not settings.software_enabled) {
        Serial.println("Stepper driver is software disabled!");
    } else if ((not status.standstill)) {
        //
    } else {
        if (velocityX > 0) {
            Serial.println("Not moving, something is wrong!");
        }
    }

    Serial.print("stall_guard_result = ");
    Serial.println(driverX.getStallGuardResult());

    //Serial.println();

    if (millis() > (60 * 1000)) {
        setXVelocity(0);
    }
    if (millis() > (70 * 1000)) {
        driverX.disable();
    }
    delay(DELAY);
}

void setXVelocity(uint32_t velocity) {
  velocityX = velocity;
  driverX.moveAtVelocity(velocity);
}
