/*********************************************************
use this sketch to find the correct calibration values for your setup.

this sketch is meant to be used with a bigtreetech SKR-Pico board.

sketch license: CC0 Public Domain
**********************************************************/

#include <Adafruit_MPR121.h>
#include <Adafruit_TinyUSB.h>
#include <FastLED.h>
#include <Wire.h>
#include <slight_DebugMenu.h>

#ifndef _BV
    #define _BV(bit) (1 << (bit))
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Info
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void sketchinfo_print(Print &out) {
    out.println();
    //             "|~~~~~~~~~|~~~~~~~~~|~~~..~~~|~~~~~~~~~|~~~~~~~~~|"
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("|                       ^ ^                      |"));
    out.println(F("|                      (0,0)                     |"));
    out.println(F("|                      ( _ )                     |"));
    out.println(F("|                       \" \"                      |"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("| MPR121-adjustWithMenu.ino"));
    out.println(F("|   use this sketch to find the best values for "));
    out.println(F("|   your MPR121 setup."));
    out.println(
        F("|   the main Serial-Port is used to tweak the configuration."));
    out.println(F(
        "|   there is a second Serial-Port for plotting the current values."));
    out.println(F("|"));
    out.println(F("| This Sketch has a debug-menu:"));
    out.println(F("| send '?'+Return for help"));
    out.println(F("|"));
    out.println(F("| dream on & have fun :-)"));
    out.println(F("|"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("|"));
    // out.println(F("| Version: Nov 11 2013  20:35:04"));
    out.print(F("| version: "));
    out.print(F(__DATE__));
    out.print(F("  "));
    out.print(F(__TIME__));
    out.println();
    out.println(F("|"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println();

    // out.println(__DATE__); Nov 11 2013
    // out.println(__TIME__); 20:35:04
}

Adafruit_USBD_CDC USBSer1;

const uint8_t SENSOR_I2C_ADDRESS = 0x5A;
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

int sensormin = 8;
int sensormax = 11;

const uint16_t plotTouched = 690;

CRGB leds[NUM_NEOPIXEL];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Menu System

// slight_DebugMenu(Stream &in_ref, Print &out_ref, uint8_t input_length_new);
slight_DebugMenu myDebugMenu(Serial, Serial, 15);

boolean infoled_state = 0;
const byte infoled_pin = 9; // D9

unsigned long liveSign_TimeStamp_LastAction = 0;
const uint16_t liveSign_UpdateInterval = 1000; // ms

boolean liveSign_Serial_Enabled = 1;
boolean liveSign_LED_Enabled = 1;

void printHelp(Print &out) {
    // help
    out.println(
        F("____________________________________________________________"));
    out.println();
    out.println(F("Help for Commands:"));
    out.println();
    out.println(F("\t '?': this help"));
    out.println(F("\t 'i': sketch info"));
    out.println(F("\t 'y': toggle DebugOut livesign print"));
    out.println(F("\t 'Y': toggle DebugOut livesign LED"));
    out.println(F("\t 'x': tests"));
    out.println();
    out.println(F("\t 'a': setAutoconfig (1=on, 0=off) 'a1'"));
    out.println(F("\t 'A': restart autoconfig 'A'"));
    out.println(F("\t 'f': setThresholds(touch, release) 'f255:255'"));
    out.println();
    out.println(
        F("____________________________________________________________"));
}

// Main Menu
void handleMenu_Main(slight_DebugMenu *pInstance) {
    Print &out = pInstance->get_stream_out_ref();
    char *command = pInstance->get_command_current_pointer();
    // out.print("command: '");
    // out.print(command);
    // out.println("'");
    switch (command[0]) {
    case 'h':
    case 'H':
    case '?': {
        printHelp(out);
    } break;
    case 'i': {
        sketchinfo_print(out);
    } break;
    case 'y': {
        out.println(F("\t toggle DebugOut livesign Serial:"));
        liveSign_Serial_Enabled = !liveSign_Serial_Enabled;
        out.print(F("\t liveSign_Serial_Enabled:"));
        out.println(liveSign_Serial_Enabled);
    } break;
    case 'Y': {
        out.println(F("\t toggle DebugOut livesign LED:"));
        liveSign_LED_Enabled = !liveSign_LED_Enabled;
        out.print(F("\t liveSign_LED_Enabled:"));
        out.println(liveSign_LED_Enabled);
    } break;
    case 'x': {
        // get state
        out.println(F("__________"));
        out.println(F("Tests:"));

        out.println(F("__________"));
    } break;
    //---------------------------------------------------------------------
    case 'a': {
        out.println(F("\t setAutoconfig"));
        bool autoconfigEnable = atoi(&command[1]);
        cap.setAutoconfig(autoconfigEnable);
    } break;
    case 'A': {
        out.println(F("\t restart autoconfig."));
        bool autoconfigEnable = atoi(&command[1]);
        cap.setAutoconfig(autoconfigEnable);
    } break;
    case 'f': {
        out.print(F("\t DemoFadeTo "));
        // convert part of string to int
        // (up to first char that is not a number)
        uint8_t touch = atoi(&command[1]);
        // convert single character to int representation
        // uint8_t id = &command[1] - '0';
        out.print(touch);
        out.print(F(" : "));
        uint8_t release = touch / 2;

        // check if second value is given.
        char *delimiter = strchr(&command[1], ',');
        if (delimiter != NULL) {
            release = atoi(delimiter + 1);
            out.print(release);
            out.println();
        } else {
            out.print(release);
            out.println();
            out.print(
                F("\t   --> no second parameter given. using 'touch/2'."));
            out.println();
        }
        cap.setThresholds(touch, release);
    } break;
    //---------------------------------------------------------------------
    default: {
        if (strlen(command) > 0) {
            out.print(F("command '"));
            out.print(command);
            out.println(F("' not recognized. try again."));
        }
        pInstance->get_command_input_pointer()[0] = '?';
        pInstance->set_flag_EOC(true);
    }
    } // end switch

    // end Command Parser
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// live sign

void liveSign() {
    if ((millis() - liveSign_TimeStamp_LastAction) > liveSign_UpdateInterval) {
        liveSign_TimeStamp_LastAction = millis();

        if (liveSign_Serial_Enabled) {
            Serial.print(millis());
            Serial.println(F("ms"));
        }

        if (liveSign_LED_Enabled) {
            infoled_state = !infoled_state;
            if (infoled_state) {
                leds[0].green = 200;
                // leds[0] = CHSV(40, 255, 255);
                FastLED.show();
            } else {
                leds[0].green = 1;
                // leds[0] = CHSV(40, 255, 1);
                FastLED.show();
            }
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sensor
void setupSensor() {
    cap.setThresholds(4, 2);
    cap.setAutoconfig(true);
}

void plotSensor() {
    currtouched = cap.touched();

    for (uint8_t i = sensormin; i < sensormax; i++) {
        // if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
        //   Serial.print(i);
        //   Serial.println(" touched");
        // }
        // // if it *was* touched and now *isnt*, alert!
        // if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
        //   Serial.print(i);
        //   Serial.println(" released");
        // }
        if (!(currtouched & _BV(i))) {
            USBSer1.print(0);
        } else {
            USBSer1.print(plotTouched);
        }
        USBSer1.print(";");
    }

    for (uint8_t i = sensormin; i < sensormax; i++) {
        USBSer1.print(cap.filteredData(i));
        USBSer1.print(";");
    }
    //   USBSer1.println();
    //   USBSer1.print("Base: ");
    for (uint8_t i = sensormin; i < sensormax; i++) {
        USBSer1.print(cap.baselineData(i));
        USBSer1.print(";");
    }

    USBSer1.println();
    // reset our state
    lasttouched = currtouched;
}

void handleSensor() {
    for (uint8_t i = sensormin; i < sensormax; i++) {
        if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
            Serial.print(i);
            Serial.println(" touched");
            // leds[0] = CHSV(170, 0, 0);
            leds[0].blue = 200;
            FastLED.show();
        }
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
            Serial.print(i);
            Serial.println(" released");
            // leds[0] = CHSV(170, 255, 255);
            leds[0].blue = 0;
            FastLED.show();
        }
    }
}

void dump_regs() {
    // based on
    // https://github.com/adafruit/Adafruit_MPR121/pull/43#issuecomment-2970984674
    Serial.println("========================================");
    Serial.println("CHAN 00 01 02 03 04 05 06 07 08 09 10 11");
    Serial.println("     -- -- -- -- -- -- -- -- -- -- -- --");

    Serial.print("CDC: ");
    for (int chan = 0; chan < 12; chan++) {
        uint8_t reg = cap.readRegister8(0x5F + chan);
        if (reg < 10)
            Serial.print(" ");
        Serial.print(reg);
        Serial.print(" ");
    }
    Serial.println();

    Serial.print("CDT: ");
    for (int chan = 0; chan < 6; chan++) {
        uint8_t reg = cap.readRegister8(0x6C + chan);
        uint8_t cdtx = reg & 0b111;
        uint8_t cdty = (reg >> 4) & 0b111;
        if (cdtx < 10)
            Serial.print(" ");
        Serial.print(cdtx);
        Serial.print(" ");
        if (cdty < 10)
            Serial.print(" ");
        Serial.print(cdty);
        Serial.print(" ");
    }
    Serial.println();
    Serial.println("========================================");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// setup & loop
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup() {
    Serial.begin(115200);

    FastLED.addLeds<WS2811, PIN_NEOPIXEL, GRB>(leds, NUM_NEOPIXEL);

    // sketch setup `orange light`
    // for Hue-Color-Map see:
    // https://s-light.github.io/cp_magic_painter/docu/color_hue.svg
    // https://github.com/FastLED/FastLED/wiki/FastLED-HSV-Colors
    for (uint8_t i = 0; i < 5; i++) {
        leds[0] = CHSV(64, 255, 200);
        FastLED.show();
        delay(500);
        leds[0] = CHSV(64, 255, 50);
        FastLED.show();
        delay(500);
    }

    const uint32_t start = millis();
    while (!Serial && !((millis() - start) > 6000))
        yield();

    USBSer1.begin(115200);
    leds[0] = CHSV(140, 255, 255);
    FastLED.show();

    // If already enumerated, additional class driver begin() won't take effect
    // until re-enumeration
    // if (TinyUSBDevice.mounted()) {
    //     TinyUSBDevice.detach();
    //     delay(10);
    //     TinyUSBDevice.attach();
    // }

    // serial setup done.
    // start I2C and sensor setup
    // `purple light`
    leds[0] = CHSV(190, 255, 255);
    FastLED.show();

    sketchinfo_print(Serial);

    myDebugMenu.set_user_EOC_char(';');
    myDebugMenu.set_callback(handleMenu_Main);
    myDebugMenu.begin();

    Serial.println("startup `Wire`");
    Wire.begin();
    Serial.println("done.");
    delay(10);

    Serial.println("cap.begin..");
    if (!cap.begin(SENSOR_I2C_ADDRESS, &Wire)) {
        Serial.println("MPR121 not found, check wiring?");
        while (1)
            ;
    }
    Serial.println("MPR121 found!");

    // config sensor
    leds[0] = CHSV(215, 255, 255);
    FastLED.show();
    Serial.println("setupSensor()");
    setupSensor();

    // run. switch led off
    leds[0] = CHSV(160, 255, 0);
    FastLED.show();
    Serial.println(F("entering main loop.."));
}

void loop() {
    myDebugMenu.update();
    handleSensor();
    plotSensor();
    liveSign();
}
