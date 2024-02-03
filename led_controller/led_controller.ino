#include "test_config.hpp"

#include "modes.hpp"
#include "strip.hpp"

struct segment_t {
    uint32_t color;
    uint8_t mode;
    uint8_t parameter;
};

// // read segment information from dmx
// segment_t read_dmx(uint16_t i) {
//     const auto dmx_read = [](uint16_t n) {
//         return DMXSerial.read(DMX_START_ADDRESS + n);
//     };
//
//     uint8_t r = dmx_read(i * 5 + 0);
//     uint8_t g = dmx_read(i * 5 + 1);
//     uint8_t b = dmx_read(i * 5 + 2);
//     uint8_t m = dmx_read(i * 5 + 3);
//     uint8_t p = dmx_read(i * 5 + 4);
//
//     return {
//         .color = LEDStrip::Color(r, g, b),
//         .mode = m,
//         .parameter = p,
//     };
// }

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
    // DMXSerial.init(DMXProbe);
    Serial.begin(9600);
    strip::init();
    strip::show();
}

using modefunc_t = void (*)(const uint32_t &, uint16_t, uint16_t, uint32_t &);
using repfunc_t = void (*)(const uint32_t *, uint16_t, uint32_t &);
const uint16_t REP_LENGTH = 9;

void loop() {
    const SegmentMode &mode = strip::get_mode();
    const segment_t DUMMY_DATA[] = {
        {LEDStrip::Color(255, 0, 0), 0, 0},
        {LEDStrip::Color(0, 255, 0), 0, 0},
        {LEDStrip::Color(0, 0, 255), 0, 0},
    };
    uint32_t time = millis();

    for (uint16_t segn = 0; segn < mode.count; segn++) {
        const segment_t &data = DUMMY_DATA[segn];
        const Segment &seg = mode.data[segn];

        Context ctx = Context(data.color, time, seg);

        mode_func(ctx, data.mode);
    }

    strip::show();
    // } else {
    //     panic(3);
    // }
}
