#include <Adafruit_MPR121.h>

#ifndef _BV
    #define _BV(bit) (1 << (bit))
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();
// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

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
// ##########################################

void capUpdate() {
    // debugoutput can be streamd via USBSer1
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
