#pragma once

// something went wrong!
// make <blinks> short blinks
void panic(uint8_t blinks) {
    while (true) {
        for (uint8_t i = 0; i < blinks; i++) {
            digitalWrite(LED_BUILTIN, true);
            delay(50);
            digitalWrite(LED_BUILTIN, false);
            delay(50);
        }
        delay(1000 - blinks * 100);
    }
}