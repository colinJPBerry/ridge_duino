# LED Controller

Warning to anyone trying to edit this code: The things I have to do to get this configuration file syntax to work while not leaking memory are so hacky that it's best if you try not to touch it. If _I_ touch it it all falls apart and explodes. So I don't touch it.

For everyone else, welcome! This is a program for controlling LED strips via DMX.

## Config File

You can configure the LED controller by making a config file. You should end this file with `.hpp`, and it should be placed in the same directory as the main file.

If you would like to upload the code with a certain configuration, change the `#include` at the top of the main file to point to the config file you want to use.

For example: If you made a config file called `proscenium.hpp`, you would change the first line of `led_controller.ino`:

```diff
- #include "not_my_config.hpp"
+ #include "proscenium.hpp"
```

The general format for a config file is as follows:

```cpp
#pragma once

#include "segment.hpp"
#include <Adafruit_NeoPixel.h>

#define PIXEL_CONFIG NEO_RBG + NEO_KHZ800

// OPTION 1: multiple LED strips connected to the arduino:
#define LED_STRIP_CONTROL_PINS {6, 7, 8}
#define LED_STRIP_LENGTHS {50, 50, 50}
// OPTION 2: single LED strip connected to the arduino
#define LED_STRIP_CONTROL_PIN 7
#define LED_STRIP_LENGTH 50

// OPTION 1: a single LED segment mode
const SegmentMode MODE = {
    999,
    Segment::range(0, 4),
    Segment::range(5, 9) + Segment(11, 12),
},
// OPTION 2: multiple LED segment modes
#define MODE_SELECT_ADDRESS 0
const SegmentModes MODES = {
    {
        999,
        Segment::range(0, 4),
        Segment::range(5, 9) + Segment(11, 12),
    },
};
```

## Segment Modes

This program splits the LED strip into different segments, which are them controlled via DMX. Segment modes are a way of switching between different configurations of segments.

### Syntax

```cpp
const SegmentMode MODE = {
    999,                    // the address of the mode
    Segment::range(0, 4),   // the first segment (segment 0)
    Segment::range(5, 9),   // the second segment (segment 1)
    // ...etc
};

// if you need multiple modes:
const SegmentMode MODES = {
    {
        // segment mode 0
    },
    {
        // segment mode 1
    },
    // ...etc
};
```

REMEMBER! Like everything else, LED's and segments are indexed starting at 0.

Segments can be defined with the following expressions:

-   `Segment(x1, x2, x3, ..., xN)` - a list of LED's
-   `Segment::range(start, end)` - a range of LED's from `start` to `end` (inclusive)
-   `Segment::range(start, end, step)` - a range of LED's from `start` to `end` (inclusive) with a step size of `step`

These segments expressions can be added together with `+` to make more complicated segments.

## DMX Format

TODO

## Configuration Value Details

### `WS2812_PIN`

_default: `7`_

The pin number for interfacing with the LED strip.

### `PIXEL_CONFIG`

_default: `NEO_RBG + NEO_KHZ800`_

The configuration of the LED's on the LED strip.

A list of the definitions near the top of [this file](https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.h), currently at lines 90 to 139.

TODO

## LED Command Codes

TODO

## Blink Codes

If the arduino is not responding and you find it blinking it's little LED, that means something went wrong. Here's what the different blink codes mean:

TODO
