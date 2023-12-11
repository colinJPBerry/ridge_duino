#pragma once

#include "Arduino.h"
#include "stdint.h"

namespace utilities {

// something went wrong!
// make <blinks> short blinks
void panic(uint8_t blinks) {
    while (1) {
        for (uint8_t i = 0; i < blinks; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(50);
            digitalWrite(LED_BUILTIN, LOW);
            delay(50);
        }
        delay(1000 - blinks * 100);
    }
}

} // namespace utilities
