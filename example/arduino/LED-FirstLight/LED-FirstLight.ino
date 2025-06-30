#include <FastLED.h>

// simple rainbow for on-board led

#define NUM_LEDS NUM_NEOPIXEL
#define DATA_PIN PIN_NEOPIXEL
#define BRIGHTNESS 255
#define SATURATION 255

CRGB leds[NUM_LEDS];

void setup() {
    // sanity check delay
    delay(2000);
    FastLED.addLeds<WS2811, PIN_NEOPIXEL, RGB>(leds, NUM_LEDS);
}

void loop() {
    for (int j = 0; j < 255; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS);
            // The higher the value 4 the less fade there is and vice versa
        }
        FastLED.show();
        delay(25);
    }
}
