#include <FastLED.h>

// simple rainbow for on-board led

#define NUM_LEDS NUM_NEOPIXEL
#define DATA_PIN PIN_NEOPIXEL
#define BRIGHTNESS 100
#define SATURATION 255
uint8_t hueSpeed = 10;

CRGB leds[NUM_LEDS];

void setup() {
    // sanity check delay
    delay(2000);
    FastLED.addLeds<WS2811, PIN_NEOPIXEL, RGB>(leds, NUM_LEDS);
}

void loop() {
    leds[0] = CHSV(beat8(hueSpeed, 255), SATURATION, BRIGHTNESS);
    FastLED.show();
    // alternative to the beat8 we can use a classic
    // You can change the pattern speed here
    // EVERY_N_MILLISECONDS(15) { hue++; }
}
