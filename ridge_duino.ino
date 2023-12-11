#include "src/neopixel.hpp"
#include "src/utilities.hpp"

neopixel::EqualSegments constexpr cfg = {
    .pixel_count = 10, .dmx_start_address = 0, .segments = 2};

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    neopixel::equal_segments<cfg>();
}

void loop() {
    //
}