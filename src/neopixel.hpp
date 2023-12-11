#pragma once

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

inline void neopixel(const EqualSegments config);
inline void neopixel(const VariableSegments config);

} // namespace neopixel