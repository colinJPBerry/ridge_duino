#include "neopixel.hpp"
#include "utilities.hpp"

Adafruit_NeoPixel strip;
uint16_t dmx_start_address;

void init(uint16_t n) {
    strip = Adafruit_NeoPixel(n, WS2812_PIN, PIXEL_CONFIG);
    strip.begin();
    DMXSerial.init(DMXProbe);
}

uint32_t get_rgb_from_dmx(uint16_t i) {
    uint8_t r = DMXSerial.read(dmx_start_address + i * 3);
    uint8_t g = DMXSerial.read(dmx_start_address + i * 3 + 1);
    uint8_t b = DMXSerial.read(dmx_start_address + i * 3 + 2);
    return strip.Color(r, g, b);
}

void neopixel::neopixel(EqualSegments config) {
    init(config.pixel_count);
    dmx_start_address = config.dmx_start_address;

    if (config.pixel_count % config.segments != 0) {
        panic(2);
    }
    uint16_t segment_length = config.pixel_count / config.segments;

    if (DMXSerial.receive(100)) {
        for (uint16_t s = 0; s < config.segments; s++) {
            uint32_t color = get_rgb_from_dmx(s);
            for (uint16_t i = 0; i < segment_length; i++) {
                strip.setPixelColor(s * segment_length + i, color);
            }
        }
        strip.show();
    } else {
        panic(3);
    }
}

// void neopixel::neopixel(VariableSegments config) {
//     init(config.pixel_count);
//     dmx_start_address = config.dmx_start_address;

//     size_t segment_count = sizeof(config.segments);
//     uint16_t pixel_count = 0;
//     for (uint16_t i = 0; i < sizeof(config.segments) / sizeof(uint16_t); i++)
//     {
//         pixel_count += config.segments[i];
//     }
//     if (pixel_count != config.pixel_count) {
//         panic(2);
//     }

//     if (DMXSerial.receive(100)) {
//         uint16_t pixel_index = 0;
//         for (uint16_t s = 0; s < sizeof(config.segments) / sizeof(uint16_t);
//              s++) {
//             uint32_t color = get_rgb_from_dmx(s);
//             for (uint16_t i = 0; i < config.segments[s]; i++) {
//                 strip.setPixelColor(pixel_index, color);
//                 pixel_index++;
//             }
//         }
//         strip.show();
//     } else {
//         panic(3);
//     }
// }