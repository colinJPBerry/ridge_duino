#include <Adafruit_NeoPixel.h>
#include <DMXSerial.h>

#define WS2812_PIN 7
#define PIXEL_COUNT 100
#define PIXEL_CONFIG NEO_RBG + NEO_KHZ800

#define DMX_SA 100  // DMX start address

#define SEGMENTS 10

Adafruit_NeoPixel strip(PIXEL_COUNT, WS2812_PIN, PIXEL_CONFIG);

uint32_t getColorAtIndex(uint8_t index) {
  return strip.Color(
    DMXSerial.read(DMX_SA + index * 3),
    DMXSerial.read(DMX_SA + index * 3 + 1),
    DMXSerial.read(DMX_SA + index * 3 + 2));
}

void setup() {
  strip.begin();
  DMXSerial.init(DMXProbe);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (DMXSerial.receive(1000)) {
    for (int i = 0; i < PIXEL_COUNT; i++) {
      strip.setPixelColor(i, getColorAtIndex(i*SEGMENTS/PIXEL_COUNT));
    }
    strip.show();
  } else {
    digitalWrite(LED_BUILTIN, true);
    delay(100);
    digitalWrite(LED_BUILTIN, false);
    delay(100);
  }
}
