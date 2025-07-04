
// #include <Adafruit_LIS3DH.h>
// #include <Adafruit_Sensor.h>


// Adafruit_LIS3DH lis = Adafruit_LIS3DH(&Wire);


// void accelSetup() {
//     if (!lis.begin(0x18)) { // change this to 0x19 for alternative i2c address
//         Serial.println("LIS3DH acceleration sensor not found.");
//         while (1)
//             yield();
//     }
//     Serial.println("LIS3DH found!");

//     // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

//     Serial.print("Range = ");
//     Serial.print(2 << lis.getRange());
//     Serial.println("G");

//     // lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER);
//     Serial.print("Performance mode set to: ");
//     switch (lis.getPerformanceMode()) {
//     case LIS3DH_MODE_NORMAL:
//         Serial.println("Normal 10bit");
//         break;
//     case LIS3DH_MODE_LOW_POWER:
//         Serial.println("Low Power 8bit");
//         break;
//     case LIS3DH_MODE_HIGH_RESOLUTION:
//         Serial.println("High Resolution 12bit");
//         break;
//     }

//     // lis.setDataRate(LIS3DH_DATARATE_50_HZ);
//     Serial.print("Data rate set to: ");
//     switch (lis.getDataRate()) {
//     case LIS3DH_DATARATE_1_HZ:
//         Serial.println("1 Hz");
//         break;
//     case LIS3DH_DATARATE_10_HZ:
//         Serial.println("10 Hz");
//         break;
//     case LIS3DH_DATARATE_25_HZ:
//         Serial.println("25 Hz");
//         break;
//     case LIS3DH_DATARATE_50_HZ:
//         Serial.println("50 Hz");
//         break;
//     case LIS3DH_DATARATE_100_HZ:
//         Serial.println("100 Hz");
//         break;
//     case LIS3DH_DATARATE_200_HZ:
//         Serial.println("200 Hz");
//         break;
//     case LIS3DH_DATARATE_400_HZ:
//         Serial.println("400 Hz");
//         break;

//     case LIS3DH_DATARATE_POWERDOWN:
//         Serial.println("Powered Down");
//         break;
//     case LIS3DH_DATARATE_LOWPOWER_5KHZ:
//         Serial.println("5 Khz Low Power");
//         break;
//     case LIS3DH_DATARATE_LOWPOWER_1K6HZ:
//         Serial.println("1.6 Khz Low Power");
//         break;
//     }
// }

// ##########################################

// void accelUpdate() {
//     EVERY_N_MILLISECONDS(500) {
//         lis.read(); // get X Y and Z data at once
//         // Then print out the raw data
//         USBSer1.print("X:  ");
//         USBSer1.print(lis.x);
//         USBSer1.print("  \tY:  ");
//         USBSer1.print(lis.y);
//         USBSer1.print("  \tZ:  ");
//         USBSer1.print(lis.z);

//         /* Or....get a new sensor event, normalized */
//         sensors_event_t event;
//         lis.getEvent(&event);

//         /* Display the results (acceleration is measured in m/s^2) */
//         USBSer1.print("\t\tX: ");
//         USBSer1.print(event.acceleration.x);
//         USBSer1.print(" \tY: ");
//         USBSer1.print(event.acceleration.y);
//         USBSer1.print(" \tZ: ");
//         USBSer1.print(event.acceleration.z);
//         USBSer1.println(" m/s^2 ");

//         USBSer1.println();
//     }
// }
