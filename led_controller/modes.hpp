#pragma once

#include "strip.hpp"

class Context {
    uint16_t i = 0;
    const Segment *seg;

  public:
    uint32_t color;
    uint32_t time;
    uint16_t len;

    Context(const uint32_t color, const uint32_t time, const Segment &seg) {
        this->color = color;
        this->time = time;
        this->len = seg.count;
        this->seg = &seg;
    }

    void set(uint32_t color) {
        strip::set_pixel(this->seg->data[i++], color);
    }
};

// ID 0
void identity(Context &ctx) {
    for (uint16_t i = 0; i < ctx.len; i++) {
        ctx.set(ctx.color);
    }
}

void mode_func(Context &ctx, uint8_t mode) {
    switch (mode) {
    case 0:
        identity(ctx);
        break;
    default:
        identity(ctx);
        break;
    }
}