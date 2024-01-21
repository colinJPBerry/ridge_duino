#pragma once
#include <Adafruit_NeoPixel.h>

struct Segment {
    uint16_t count;
    uint16_t *data;

    Segment(uint16_t count) : count(count) {
        this->data = new uint16_t[count];
    }

    Segment(uint16_t count, uint16_t *data) : count(count), data(data) {
    }

    ~Segment() {
        delete[] this->data;
    }

    template <typename... uint16_t>
    static Segment custom(uint16_t... segments) {
        const size_t n = sizeof...(uint16_t);
        Segment seg(n);
        size_t i = 0;
        for (auto &s : {segments...}) {
            seg[i++] = s;
        }
        return seg;
    }

    static Segment range(uint16_t from, uint16_t to, uint16_t step = 1) {
        if (from > to) {
            uint16_t tmp = from;
            from = to;
            to = tmp;
        }
        Segment seg((to - from + step) / step);
        for (uint16_t i = 0; i <= seg.count; i += 1) {
            seg[i] = from + i * step;
        }
        return seg;
    }

    operator uint16_t *() {
        return this->data;
    }

    uint16_t &operator[](uint16_t index) {
        return this->data[index];
    }

    Segment operator+(const Segment &other) {
        Segment seg(this->count + other.count);
        for (uint16_t i = 0; i < this->count; i++) {
            seg[i] = this->data[i];
        }
        for (uint16_t i = 0; i < other.count; i++) {
            seg[this->count + i] = other.data[i];
        }

        return seg;
    }
};