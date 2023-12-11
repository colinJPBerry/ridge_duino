#pragma once

#include "utilities.hpp"
#include <Adafruit_NeoPixel.h>
#include <DMXSerial.h>

#define WS2812_PIN 7
#define PIXEL_CONFIG NEO_RBG + NEO_KHZ800

namespace neopixel {

/**
 * Split the neopixel strip into `segments` equally-sized segments. Expose to
 * DMX as `segments` channels of RGB data, starting at `dmx_start_address`.
 *
 * @exception `2 short blinks`: The number of pixels is not divisible by the
 * number of segments.
 * @exception `3 short blinks`: `DMXSerial.recieve` timed out.
 */
struct EqualSegments {
    uint16_t pixel_count;
    uint16_t dmx_start_address;
    uint16_t segments;
};

template <EqualSegments const &cfg> inline void equal_segments();

/**
 * Split the neopixel strip into different length segments.
 *
 * @exception `2 short blinks`: The lengths in `segments` did not add up to
 * `pixel_count`
 * @exception `3 short blinks`: `DMXSerial.recieve` timed out.
 */
struct VariableSegments {
    uint16_t pixel_count;
    uint16_t dmx_start_address;
    uint16_t segments[];
};

template <VariableSegments const &cfg> inline void variable_segments();

// IMPLEMENTATION -------------------------------------------------------------

Adafruit_NeoPixel strip;
uint16_t dmx_start_address;

void init(uint16_t n, uint16_t _dmx_start_address) {
    strip = Adafruit_NeoPixel(n, WS2812_PIN, PIXEL_CONFIG);
    strip.begin();
    DMXSerial.init(DMXProbe);
    dmx_start_address = _dmx_start_address;
}

uint32_t get_rgb_from_dmx(uint16_t i) {
    uint8_t r = DMXSerial.read(dmx_start_address + i * 3);
    uint8_t g = DMXSerial.read(dmx_start_address + i * 3 + 1);
    uint8_t b = DMXSerial.read(dmx_start_address + i * 3 + 2);
    return strip.Color(r, g, b);
}

template <EqualSegments const &cfg> inline void equal_segments() {
    init(cfg.pixel_count, cfg.dmx_start_address);

    if (cfg.pixel_count % cfg.segments != 0)
        utilities::panic(2);
    const uint16_t SEGMENT_LEN = cfg.pixel_count / cfg.segments;

    while (1) {
        if (DMXSerial.receive(100)) {
            for (uint16_t i = 0; i < cfg.segments; i++) {
                uint32_t color = get_rgb_from_dmx(i);
                for (uint16_t j = 0; j < SEGMENT_LEN; j++) {
                    strip.setPixelColor(i * SEGMENT_LEN + j, color);
                }
            }
        } else {
            utilities::panic(3);
        }
    }
}

template <VariableSegments const &cfg> inline void variable_segments() {
    init(cfg.pixel_count, cfg.dmx_start_address);

    uint8_t segment_n = sizeof(cfg.segments) / sizeof(uint16_t);
    uint16_t segments[segment_n];

    segments[0] = cfg.segments[0];
    for (uint8_t i = 1; i < segment_n; i++) {
        segments[i] += segments[i - 1];
    }
    if (segments[segment_n - 1] != cfg.pixel_count)
        utilities::panic(2);

    while (1) {
        if (DMXSerial.receive(100)) {
            uint16_t i = 0;
            uint32_t color = get_rgb_from_dmx(0);

            for (uint16_t seg = 0; seg < segment_n;) {
                strip.setPixelColor(i, color);
                i++;
                if (i == segments[seg]) {
                    seg++;
                    color = get_rgb_from_dmx(seg);
                }
            }
            strip.show();
        } else {
            utilities::panic(3);
        }
    }
}

} // namespace neopixel