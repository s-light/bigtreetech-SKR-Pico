/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop
https://www.adafruit.com/search?q=MPR121

These sensors use I2C communicate, at least 2 pins are required
to interface

this sketch demonstrates the auto-config chip-functionality.
for more information have a look at
https://github.com/adafruit/Adafruit_MPR121/issues/39
https://github.com/adafruit/Adafruit_MPR121/pull/43


based on MPR121test
modified & extended by Carter Nelson/caternuson

MIT license, all text above must be included in any redistribution
**********************************************************/

#include <Adafruit_MPR121.h>
#include <Wire.h>

#ifndef _BV
    #define _BV(bit) (1 << (bit))
#endif

uint8_t twi_address = MPR121_I2CADDR_DEFAULT;

enum twi_state_t {
    TWI_STATE_success = 0,
    TWI_STATE_data_to_long = 1,
    TWI_STATE_rec_NACK_on_address = 2,
    TWI_STATE_rec_NACK_on_data = 3,
    TWI_STATE_other_error = 4,
    TWI_STATE_timeout = 5,
    TWI_STATE_undefined = 99,
};

void twi_state_print(twi_state_t state) {
    switch (state) {
    case TWI_STATE_success: {
        Serial.print(F("success"));
    } break;
    case TWI_STATE_data_to_long: {
        Serial.print(F("data too long to fit in transmit buffer"));
    } break;
    case TWI_STATE_rec_NACK_on_address: {
        Serial.print(F("received NACK on transmit of address"));
    } break;
    case TWI_STATE_rec_NACK_on_data: {
        Serial.print(F("received NACK on transmit of data"));
    } break;
    case TWI_STATE_other_error: {
        Serial.print(F("other error"));
    } break;
    default: {
        Serial.print(F("??"));
    }
    }
}


void writeRegister(uint8_t reg, uint8_t value) {
    twi_state_t twi_state = TWI_STATE_undefined;
    Wire.beginTransmission(twi_address);
    Wire.write(reg);
    Wire.write(value);
    twi_state = (twi_state_t)Wire.endTransmission();
    twi_state_print(twi_state);
    // if(error != 0) {
    //     twi_state_print(error);
    // }
}

uint8_t readRegister(uint8_t reg_name) {
    uint8_t result_value = 0;
    twi_state_t twi_state = TWI_STATE_undefined;
    // set register
    Wire.beginTransmission(twi_address);
    Wire.write(reg_name);
    twi_state = (twi_state_t)Wire.endTransmission();
    if (twi_state == TWI_STATE_success) {
        // read data
        Wire.requestFrom(twi_address, (uint8_t)1);
        result_value = Wire.read();
    } else {
            twi_state_print(twi_state);
    }
    return result_value;
}

void setup() {
    delay(1000);
    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println("MPR121 Test. (MPR121-simpletest.ino)");

    Serial.println("startup `Wire`");
    // PIN_WIRE_SDA == PIN_SERIAL1_TX == D0
    // PIN_WIRE_SCL == PIN_SERIAL1_RX == D1
    // Wire.setSDA(PIN_WIRE_SDA);
    // Wire.setSCL(PIN_WIRE_SCL);
    Wire.begin();
    Wire.setTimeout();
    Serial.println("done.");
    delay(100);

    Serial.println("setup MPR121..");
    writeRegister(MPR121_SOFTRESET, 0x63);
    writeRegister(MPR121_ECR, 0x0);
    
    Serial.println();
    Serial.print("read: MPR121_CONFIG2 ");
    uint8_t reg_value = readRegister(MPR121_CONFIG2);
    Serial.print("0x");
    Serial.println(reg_value, HEX);
    if (reg_value != 0x24) {
        Serial.println("reading from MPR121 register returned wrong value: ");
        Serial.println("  expected: 0x24");
        Serial.print("  read: 0x");
        Serial.print(reg_value);
        Serial.println("");
        Serial.println();
        Serial.println("please fix things and try again.");

        while(1);
    }

    for (uint8_t i = 0; i < 12; i++) {
            writeRegister(MPR121_TOUCHTH_0 + 2 * i,
                          MPR121_TOUCH_THRESHOLD_DEFAULT);
            writeRegister(MPR121_RELEASETH_0 + 2 * i,
                          MPR121_RELEASE_THRESHOLD_DEFAULT);
        }
    writeRegister(MPR121_MHDR, 0x01);
    writeRegister(MPR121_NHDR, 0x01);
    writeRegister(MPR121_NCLR, 0x0E);
    writeRegister(MPR121_FDLR, 0x00);

    writeRegister(MPR121_MHDF, 0x01);
    writeRegister(MPR121_NHDF, 0x05);
    writeRegister(MPR121_NCLF, 0x01);
    writeRegister(MPR121_FDLF, 0x00);

    writeRegister(MPR121_NHDT, 0x00);
    writeRegister(MPR121_NCLT, 0x00);
    writeRegister(MPR121_FDLT, 0x00);

    writeRegister(MPR121_DEBOUNCE, 0);
    writeRegister(MPR121_CONFIG1, 0x10); // default, 16uA charge current
    writeRegister(MPR121_CONFIG2, 0x20); // 0.5uS encoding, 1ms period
    // enable X electrodes = start MPR121
    // CL Calibration Lock: B10 = 5 bits for baseline tracking
    // ELEPROX_EN  proximity: disabled
    // ELE_EN Electrode Enable:  amount of electrodes running (12)
    byte ECR_SETTING = 0b10000000 + 12;
    writeRegister(MPR121_ECR, ECR_SETTING); // start with above ECR setting
}

void loop() {
    delay(1000);
    Serial.println(millis());   
    // // Get the currently touched pads
    // currtouched = cap.touched();

    // for (uint8_t i = 0; i < 12; i++) {
    //     // it if *is* touched and *wasnt* touched before, alert!
    //     if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
    //         Serial.print(i);
    //         Serial.println(" touched");
    //     }
    //     // if it *was* touched and now *isnt*, alert!
    //     if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
    //         Serial.print(i);
    //         Serial.println(" released");
    //     }
    // }

    // // reset our state
    // lasttouched = currtouched;
}