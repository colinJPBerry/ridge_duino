#include "test_config.hpp"

#include "modes.hpp"
#include "strip.hpp"

struct segment_t {
    uint32_t color;
    uint8_t mode;
    uint8_t parameter;
};

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
}

const uint32_t INTERVAL_LEN = 1000 / 30;
uint32_t time = millis();
uint32_t target_time = time + INTERVAL;

void loop() {
    time = millis();
    if (time > target_time) {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    while (time < target_time)
        time = millis();
    target_time = time + INTERVAL;

    const SegmentMode &mode = strip::get_mode();
    const segment_t DUMMY_DATA[] = {
        {LEDStrip::Color(255, 0, 0), 0, 0},
        {LEDStrip::Color(0, 255, 0), 0, 0},
        {LEDStrip::Color(0, 0, 255), 0, 0},
    };

    for (uint16_t segn = 0; segn < mode.count; segn++) {
        const segment_t &data = DUMMY_DATA[segn];
        const Segment &seg = mode.data[segn];

        Context ctx = Context(data.color, time, seg);

        mode_func(ctx, data.mode);
    }

    strip::show();
}
