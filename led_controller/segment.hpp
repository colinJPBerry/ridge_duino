#pragma once

struct Segment {
    uint16_t count;
    // does this leak memory? just dont use it outside of the very specific
    // constraints i lay out in the README and dont worry about it!
    //
    // maybe im not a c++ god but jesus christ do i wish i was doing this in
    // rust.
    uint16_t *data;

    template <typename... Leds> Segment(Leds... leds) {
        this->count = sizeof...(Leds);
        this->data = new uint16_t[this->count]{leds...};
    }

    static Segment range(uint16_t from, uint16_t to, uint16_t step = 1) {
        if (from > to) {
            uint16_t tmp = from;
            from = to;
            to = tmp;
        }
        uint16_t count = (to - from + step) / step;
        Segment seg;
        seg.count = count;
        seg.data = new uint16_t[count];
        for (uint16_t i = 0; i <= seg.count; i += 1) {
            seg[i] = from + i * step;
        }
        return seg;
    }

    uint16_t &operator[](uint16_t index) {
        return this->data[index];
    }

    Segment operator+(const Segment &other) {
        uint16_t count = this->count + other.count;
        Segment seg;
        seg.count = count;
        seg.data = new uint16_t[count];

        for (uint16_t i = 0; i < this->count; i++) {
            seg[i] = this->data[i];
        }
        for (uint16_t i = 0; i < other.count; i++) {
            seg[this->count + i] = other.data[i];
        }

        // yeah... so here is where they are being deleted.
        delete[] this->data;
        delete[] other.data;

        return seg;
    }
};

struct SegmentMode {
    uint16_t count;
    Segment *data;

    template <typename... Segments> SegmentMode(Segments... segments) {
        this->count = sizeof...(Segments);
        this->data = new Segment[this->count]{segments...};
    }
};

using SegmentModes = SegmentMode[];