#pragma once

#include "segment.hpp"

#define PIXEL_CONFIG NEO_RBG + NEO_KHZ800
#define LED_STRIP_CONTROL_PIN 2
#define LED_STRIP_LENGTH 50

const SegmentMode MODE = {
    999,
    Segment::range(0, 4),
    Segment::range(5, 9),
};