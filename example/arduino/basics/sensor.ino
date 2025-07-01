#include <Wire.h>

#include <Adafruit_MPR121.h>

#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

#ifndef _BV
    #define _BV(bit) (1 << (bit))
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();
// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

Adafruit_LIS3DH lis = Adafruit_LIS3DH(&Wire);

void capSetup() {
    if (!cap.begin(0x5A, &Wire)) {
        Serial.println("MPR121 not found, check wiring?");
        while (1)
            yield();
    }
    Serial.println("MPR121 found!");
    delay(100);
    cap.setAutoconfig(true);
}

void accelSetup() {
    if (!lis.begin(0x18)) { // change this to 0x19 for alternative i2c address
        Serial.println("LIS3DH acceleration sensor not found.");
        while (1)
            yield();
    }
    Serial.println("LIS3DH found!");

    // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

    Serial.print("Range = ");
    Serial.print(2 << lis.getRange());
    Serial.println("G");

    // lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER);
    Serial.print("Performance mode set to: ");
    switch (lis.getPerformanceMode()) {
    case LIS3DH_MODE_NORMAL:
        Serial.println("Normal 10bit");
        break;
    case LIS3DH_MODE_LOW_POWER:
        Serial.println("Low Power 8bit");
        break;
    case LIS3DH_MODE_HIGH_RESOLUTION:
        Serial.println("High Resolution 12bit");
        break;
    }

    // lis.setDataRate(LIS3DH_DATARATE_50_HZ);
    Serial.print("Data rate set to: ");
    switch (lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ:
        Serial.println("1 Hz");
        break;
    case LIS3DH_DATARATE_10_HZ:
        Serial.println("10 Hz");
        break;
    case LIS3DH_DATARATE_25_HZ:
        Serial.println("25 Hz");
        break;
    case LIS3DH_DATARATE_50_HZ:
        Serial.println("50 Hz");
        break;
    case LIS3DH_DATARATE_100_HZ:
        Serial.println("100 Hz");
        break;
    case LIS3DH_DATARATE_200_HZ:
        Serial.println("200 Hz");
        break;
    case LIS3DH_DATARATE_400_HZ:
        Serial.println("400 Hz");
        break;

    case LIS3DH_DATARATE_POWERDOWN:
        Serial.println("Powered Down");
        break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ:
        Serial.println("5 Khz Low Power");
        break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ:
        Serial.println("1.6 Khz Low Power");
        break;
    }
}

// ##########################################

void capUpdate() {
    // Get the currently touched pads
    currtouched = cap.touched();
    for (uint8_t i = 0; i < 12; i++) {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
            Serial.print(i);
            Serial.println(" touched");
        }
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
            Serial.print(i);
            Serial.println(" released");
        }
    }
    // reset our state
    lasttouched = currtouched;
}

void accelUpdate() {
    EVERY_N_MILLISECONDS(500) {
        lis.read(); // get X Y and Z data at once
        // Then print out the raw data
        Serial.print("X:  ");
        Serial.print(lis.x);
        Serial.print("  \tY:  ");
        Serial.print(lis.y);
        Serial.print("  \tZ:  ");
        Serial.print(lis.z);

        /* Or....get a new sensor event, normalized */
        sensors_event_t event;
        lis.getEvent(&event);

        /* Display the results (acceleration is measured in m/s^2) */
        Serial.print("\t\tX: ");
        Serial.print(event.acceleration.x);
        Serial.print(" \tY: ");
        Serial.print(event.acceleration.y);
        Serial.print(" \tZ: ");
        Serial.print(event.acceleration.z);
        Serial.println(" m/s^2 ");

        Serial.println();
    }
}

// ##########################################

void sensorSetup() {
    Wire.setSDA(D20); // LASER_PWM
    Wire.setSCL(D29); // PIN_SERVOS
    Wire.begin();
    capSetup();
    accelSetup();
}

void sensorUpdate() {
    capUpdate();
    accelUpdate();
}
