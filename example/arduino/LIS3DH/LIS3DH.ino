
// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <Wire.h>

#include <Adafruit_MPR121.h>
Adafruit_MPR121 cap = Adafruit_MPR121();

#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH(&Wire);



void setup(void) {
    delay(1000);
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    Serial.println("LIS3DH test!");

    Wire.setSDA(D20); // LASER_PWM
    Wire.setSCL(D29); // PIN_SERVOS
    Wire.begin();
    delay(1);

    if (!cap.begin(0x5A, &Wire)) {
        Serial.println("MPR121 not found, check wiring?");
        while (1)
            yield();
    }
    Serial.println("MPR121 found!");

    if (!lis.begin(0x18)) { // change this to 0x19 for alternative i2c address
        Serial.println("Couldnt start");
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

void loop() {
    lis.read(); // get X Y and Z data at once
    // Then print out the raw data
    Serial.print(lis.x);
    Serial.print(", ");
    Serial.print(lis.y);
    Serial.print(", ");
    Serial.print(lis.z);
    // Serial.print(", ")

    /* Or....get a new sensor event, normalized */
    // sensors_event_t event;
    // lis.getEvent(&event);

    // /* Display the results (acceleration is measured in m/s^2) */
    // Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
    // Serial.print(" \tY: "); Serial.print(event.acceleration.y);
    // Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
    // Serial.println(" m/s^2 ");

    Serial.println();

    // delay(200);
}
