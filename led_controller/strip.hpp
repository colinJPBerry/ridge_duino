#pragma once

#include "segment.hpp"
#include <Adafruit_NeoPixel.h>

using LEDStrip = Adafruit_NeoPixel;

namespace strip {

inline uint16_t conv_index(uint16_t i, const LEDStrip &strip) {
    return (i + strip.numPixels()) % strip.numPixels();
}

#if defined(LED_STRIP_CONTROL_PINS) && defined(LED_STRIP_CONTROL_PIN)
#error "LED_STRIP_CONTROL_PIN and LED_STRIP_CONTROL_PINS are both defined!"
#elif defined(LED_STRIP_CONTROL_PIN)

LEDStrip strip =
    LEDStrip(LED_STRIP_LENGTH, LED_STRIP_CONTROL_PIN, PIXEL_CONFIG);

inline void init() {
    strip.begin();
}
inline void set_pixel(uint16_t i, uint32_t color) {
    strip.setPixelColor(conv_index(i, strip), color);
}
inline uint32_t get_pixel(uint16_t i) {
    return strip.getPixelColor(conv_index(i, strip));
}
inline void show() {
    strip.show();
}
inline const SegmentMode &get_mode() {
    return MODE;
}

#elif defined(LED_STRIP_CONTROL_PINS)

inline void init() {
}
inline void set_pixel(uint16_t i, uint32_t color) {
}
inline uint32_t get_pixel(uint16_t i) {
}
inline void show() {
}
inline const SegmentMode &get_mode() {
}

#else

inline void init() {
}
inline void set_pixel(uint16_t i, uint32_t color) {
}
inline uint32_t get_pixel(uint16_t i) {
}
inline void show() {
}
inline const SegmentMode &get_mode() {
}

#endif

} // namespace strip