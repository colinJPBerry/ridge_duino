#include "src/neopixel.hpp"
#include "src/utilities.hpp"

#define WS2812_PIN 7
#define PIXEL_COUNT 55
#define PIXEL_CONFIG NEO_RBG + NEO_KHZ800

#define DMX_SA 504 // DMX start address

// the length of the different segments on the LED strip
// the first length of LEDs in the list has index DMX_SA
// i have faith you can figure out the pattern from there
// list must add up to SEGMENTS
static uint16_t PARTITIONS[] = {7, 23, 25};
const uint16_t PARTITION_N = sizeof(PARTITIONS) / sizeof(uint16_t);

Adafruit_NeoPixel strip(PIXEL_COUNT, WS2812_PIN, PIXEL_CONFIG);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 255, 0));
    strip.setPixelColor(2, strip.Color(0, 0, 255));
    strip.show();
}

void loop() {
    //
}