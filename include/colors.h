//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_COLORS_H
#define TBOARD_COLORS_H

// #include "seesaw_neopixel.h"

#include <algorithm>
#include <cstdint>

// TODO: add a color manager class/module
struct RGB {
    int r;
    int g;
    int b;

    RGB();

    RGB(int r, int g, int b);
};

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
// uint32_t Wheel(byte WheelPos) {
//     if (WheelPos < 85) {
//         return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//     } else if (WheelPos < 170) {
//         WheelPos -= 85;
//         return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
//     } else {
//         WheelPos -= 170;
//         return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
//     }
//     return 0;
// }

RGB wheel_rgb(uint8_t wheel_pos);
#endif // TBOARD_COLORS_H