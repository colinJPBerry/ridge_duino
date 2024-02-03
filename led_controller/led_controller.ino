#include "test_config.hpp"

#include <Adafruit_NeoPixel.h>
#include <DMXSerial.h>

#define DMX_START_ADDRESS -1

using LEDStrip = Adafruit_NeoPixel;

namespace strip {

#if defined(LED_STRIP_CONTROL_PINS) && defined(LED_STRIP_CONTROL_PIN)
#error "LED_STRIP_CONTROL_PIN and LED_STRIP_CONTROL_PINS are both defined!"
#elif defined(LED_STRIP_CONTROL_PIN)

LEDStrip strip =
    LEDStrip(LED_STRIP_LENGTH, LED_STRIP_CONTROL_PIN, PIXEL_CONFIG);

inline void init() {
    strip.begin();
}
inline void set_pixel(uint16_t i, uint32_t color) {
    strip.setPixelColor(i, color);
}
inline void show() {
    strip.show();
}

#elif defined(LED_STRIP_CONTROL_PINS)

inline void init() {
}
inline void set_pixel(uint16_t i, uint32_t color) {
}
inline void show() {
}

#endif

} // namespace strip

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
        .color = strip::strip.Color(r, g, b),
        .mode = m,
        .parameter = p,
    };
}

// something went wrong!
// make <blinks> short blinks
template <typename... Blinks> void panic(Blinks... blinks) {
    uint8_t _blinks[] = {blinks...};
    while (1) {
        for (uint8_t blink : _blinks) {
            for (uint8_t i = 0; i < blink; i++) {
                digitalWrite(LED_BUILTIN, HIGH);
                delay(50);
                digitalWrite(LED_BUILTIN, LOW);
                delay(50);
            }
            delay(500);
        }
        delay(1000);
    }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    DMXSerial.init(DMXProbe);
    strip::init();
}

void loop() {
    if (DMXSerial.receive(1000)) {
        read_dmx(0);
    } else {
        panic(3);
    }
}