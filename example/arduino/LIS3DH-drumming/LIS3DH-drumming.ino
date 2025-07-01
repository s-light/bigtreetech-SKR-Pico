
// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <Wire.h>

#include <Adafruit_MPR121.h>
Adafruit_MPR121 cap = Adafruit_MPR121();

#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH(&Wire);

// Smoothing factor for the low-pass filter
const float alpha = 0.4;

// Smoothing factor for the baseline update
const float baselineAlpha = 0.008;

// Baseline value
float baseline = 0.0;

// Filtered value of the accelerometer reading
float filteredValue = 0.0;

// Output value in the range 0 to 100
int outputValue = 0;

sensors_event_t event;

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
    // lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER);
    // lis.setDataRate(LIS3DH_DATARATE_50_HZ);

    Serial.flush();
    printConfig();
    delay(5000);

    lis.getEvent(&event);
  baseline = event.acceleration.z;
}

void printConfig() {
    Serial.print("Range = ");
    Serial.print(2 << lis.getRange());
    Serial.println("G");

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
    lis.getEvent(&event);

    float rawValue = event.acceleration.z;

    // Apply low-pass filter
    filteredValue = alpha * rawValue + (1.0 - alpha) * filteredValue;

    // Update the baseline continuously
    baseline = baselineAlpha * rawValue + (1.0 - baselineAlpha) * baseline;

    // Calculate the absolute value from the baseline
    float absoluteValue = abs(filteredValue - baseline);

    // Scale the absolute value to the range 0 to 100
    outputValue = map(absoluteValue, 0, 16, 0, 10);

    Serial.print(rawValue);
    Serial.print(", ");
    Serial.print(filteredValue);
    Serial.print(", ");
    Serial.print(baseline);
    Serial.print(", ");
    Serial.print(absoluteValue);
    Serial.print(", ");
    Serial.print(outputValue);

    Serial.println();

    delay(5);
    if (Serial.available()) {
        Serial.flush();
        printConfig();
        delay(5000);
    }
}