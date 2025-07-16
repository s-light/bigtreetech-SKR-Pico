

#include "FastAccelStepper.h"
#include <TMC2209.h>

const int DELAY = 200;
const int32_t VELOCITY = 200000;
const int32_t VELOCITY_JUMP = 100000;
// current values may need to be reduced to prevent overheating depending on
// specific motor and power supply voltage
const uint8_t RUN_CURRENT_PERCENT = 5;

// Instantiate TMC2209
TMC2209 driverX;
uint32_t velocityX = 0;

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void motorSetup() {
    Serial2.begin(115200);
    driverX.setup(Serial2, 115200, TMC2209::SERIAL_ADDRESS_0);
    driverX.setHardwareEnablePin(PIN_XEN);
    delay(DELAY);
    
    driverX.setRunCurrent(RUN_CURRENT_PERCENT);

    // use motor datasheet value: 0,67A
    // and onboard 0.11R resistor
    // driverX.setRMSCurrent(670, 0.11f);
    // driverX.setRMSCurrent(1, 0.11f);
    // currently thsi does not have any effect - why?

    // driverX.useInternalSenseResistors();
    //  https://github.com/janelia-arduino/TMC2209?tab=readme-ov-file#standstill-mode
    driverX.setStandstillMode(TMC2209::FREEWHEELING);

    driverX.enableAutomaticCurrentScaling();
    driverX.enableCoolStep();

    // if you want to use the `PIN_XSTOP` (DIAG PIN)  you need to set
    // `setCoolStepDurationThreshold` to enable output on it..
    // https://github.com/janelia-arduino/TMC2209/issues/5#issuecomment-1067109432

    driverX.enable();

    // moveToHome();

    // for (uint8_t i = 0; i < 10; i++) {
    //     Serial.println("jump:");
    //     jump();
    //     delay(1000);
    // }

    // Serial.println("FastAccelStepper engine init..");
    // engine.init();
    // stepper = engine.stepperConnectToPin(PIN_XSTP);
    // if (stepper) {
    //     stepper->setDirectionPin(PIN_XDIR);

    //     // we rely on the driver for standby...
    //     // stepper->setEnablePin(enablePinStepper);
    //     stepper->setAutoEnable(false);
    //     // If auto enable/disable need delays, just add (one or both):
    //     // stepper->setDelayToEnable(50);
    //     // stepper->setDelayToDisable(1000);

    //     stepper->setSpeedInUs(1000); // the parameter is us/step !!!
    //     stepper->setAcceleration(100);
    //     stepper->move(1000);
    // }
}

void moveToHome() {
    Serial.println("moveToHome..");

    Serial.println("setStallGuardThreshold 2");
    driverX.setStallGuardThreshold(1);

    uint32_t stallGuard = driverX.getStallGuardResult();
    Serial.print("  stall_guard_result = ");
    Serial.println(stallGuard);

    Serial.println("  enableInverseMotorDirection");
    driverX.enableInverseMotorDirection();

    // Serial.println("  moveAtVelocity 5000");
    // driverX.moveAtVelocity(10000);
    // Serial.println("  wait 1s");
    // for (uint8_t i = 0; i < 10; i++) {
    //     delay(100);
    //     stallGuard = driverX.getStallGuardResult();
    //     Serial.print("  stall_guard_result = ");
    //     Serial.println(stallGuard);
    //     Serial.flush();
    // }
    // driverX.moveAtVelocity(0);
    // delay(1000);

    Serial.println("  homeing..");
    bool homeFound = false;
    bool timeOut = false;
    uint32_t startTime = millis();
    
    driverX.moveAtVelocity(10000);

    while ((!homeFound) || (timeOut)) {
        stallGuard = driverX.getStallGuardResult();
        if (stallGuard <= 1) {
            homeFound = true;
        }
        if ((millis() - startTime) > 1000) {
            timeOut = true;
        }
        delay(1);
    }
    driverX.moveAtVelocity(0);

    if (timeOut) {
        Serial.println("    searching for home position: Time Out!");
        Serial.print("      last reading: ");
        Serial.print(stallGuard);
        Serial.println();
        while (1) {
            delay(1);
        }
    }

    if (homeFound) {
        Serial.println("    home found.");
        Serial.print("      last reading: ");
        Serial.print(stallGuard);
        Serial.println();
        delay(1000);
    }

    driverX.disableInverseMotorDirection();

    Serial.println("moveToHome done.");
    Serial.println("******************************************");
    Serial.flush();
}

void jump() {
    driverX.moveAtVelocity(VELOCITY_JUMP);
    delay(200);
    driverX.moveAtVelocity(0);
    driverX.enableInverseMotorDirection();
    driverX.moveAtVelocity(VELOCITY_JUMP);
    delay(200);
    driverX.moveAtVelocity(0);
    driverX.disableInverseMotorDirection();
}

void motorUpdate() {
    EVERY_N_MILLISECONDS(2000) {
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
    }

    // EVERY_N_MILLISECONDS(500) {

    // }

    if (millis() > (60 * 1000)) {
        setXVelocity(0);
    }
    // if (millis() > (70 * 1000)) {
    //     driverX.disable();
    // }
}

void setXVelocity(uint32_t velocity) {
    velocityX = velocity;
    driverX.moveAtVelocity(velocity);
}
