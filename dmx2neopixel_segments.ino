#include <Adafruit_NeoPixel.h>
#include <DMXSerial.h>

#define WS2812_PIN 7
#define PIXEL_COUNT 55
#define PIXEL_CONFIG NEO_RBG + NEO_KHZ800

#define DMX_SA 504  // DMX start address

// the length of the different segments on the LED strip
// the first length of LEDs in the list has index DMX_SA
// i have faith you can figure out the pattern from there
// list must add up to SEGMENTS
static uint16_t PARTITIONS[] = {
  7, 23, 25
};
const uint16_t PARTITION_N = sizeof(PARTITIONS) / sizeof(uint16_t);

Adafruit_NeoPixel strip(PIXEL_COUNT, WS2812_PIN, PIXEL_CONFIG);

uint32_t getColorAtIndex(uint8_t index) {
  return strip.Color(
    DMXSerial.read(DMX_SA + index * 3),
    DMXSerial.read(DMX_SA + index * 3 + 1),
    DMXSerial.read(DMX_SA + index * 3 + 2));
}

// something went wrong!
// make <blinks> short blinks
void panic(uint8_t blinks) {
  while (true) {
    for (uint8_t i = 0; i < blinks; i++) {
      digitalWrite(LED_BUILTIN, true);
      delay(50);
      digitalWrite(LED_BUILTIN, false);
      delay(50);
    }
    delay(1000 - blinks * 100);
  }
}

// turn partition list into partial sum list and check
// that it adds up to pixel count
// e.g {1, 2, 3} --> {1, 3, 6}
void check_partitions() {
  for (uint8_t i = 1; i < PARTITION_N; i++) {
    PARTITIONS[i] += PARTITIONS[i-1];
  }
  if (PARTITIONS[PARTITION_N - 1] != PIXEL_COUNT) panic(2);
}

void setup() {
  strip.begin();
  DMXSerial.init(DMXProbe);
  pinMode(LED_BUILTIN, OUTPUT);
  check_partitions();
}

void loop() {
  if (DMXSerial.receive(1000)) {
    uint16_t i = 0;
    uint32_t COLOR = getColorAtIndex(0);
    // read in colors from dmx
    for (uint16_t partition = 0; partition < PARTITION_N;) {
      strip.setPixelColor(i, COLOR);
      i++;
      if (i == PARTITIONS[partition]) {
        partition++;
        COLOR = getColorAtIndex(partition);
      }
    }
    strip.show();
  } else {
    panic(3);
  }
}
