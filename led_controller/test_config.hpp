#pragma once

#include "segment.hpp"

#define PIXEL_CONFIG NEO_RGB + NEO_KHZ800
#define LED_STRIP_CONTROL_PIN 7
#define LED_STRIP_LENGTH 50

const SegmentMode MODE = {
    999,
    Segment::range(0, 4),
    Segment::range(5, 9),
    Segment::range(10, 14),
};