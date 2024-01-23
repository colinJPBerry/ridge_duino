#pragma once

#include "segment.hpp"

#define DMX_START_ADDRESS 400

const SegmentModes MODES = {
    {
        Segment::range(0, 4),
        Segment::range(5, 9),
    },
};