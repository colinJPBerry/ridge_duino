#include <Adafruit_NeoPixel.h>
#include <DMXSerial.h>

#define WS2812_PIN 7
#define PIXEL_CONFIG NEO_RBG + NEO_KHZ800
#define LED_COUNT 50

#define DMX_START_ADDRESS 400

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(LED_COUNT, WS2812_PIN, PIXEL_CONFIG);

struct segment_t {
    uint32_t color;
    uint8_t mode;
    uint8_t parameter;
};

// read segment information from dmx
segment_t read_dmx(uint16_t i) {
    const auto dmx_read = [](uint16_t n) {
        return DMXSerial.read(DMX_START_ADDRESS + n);
    };

    uint8_t r = dmx_read(i * 5 + 0);
    uint8_t g = dmx_read(i * 5 + 1);
    uint8_t b = dmx_read(i * 5 + 2);
    uint8_t m = dmx_read(i * 5 + 3);
    uint8_t p = dmx_read(i * 5 + 4);

    return {
        .color = strip.Color(r, g, b),
        .mode = m,
        .parameter = p,
    };
}

// something went wrong!
// make <blinks> short blinks
void panic(uint8_t blinks) {
    while (1) {
        for (uint8_t i = 0; i < blinks; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(50);
            digitalWrite(LED_BUILTIN, LOW);
            delay(50);
        }
        delay(max(1000 - blinks * 100, 300));
    }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    DMXSerial.init(DMXProbe);
}

void loop() {
    if (DMXSerial.receive(1000)) {
        read_dmx(0);
    } else {
        panic(3);
    }
}