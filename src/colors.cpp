//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "colors.h"

RGBA wheel_rgb(uint8_t wheel_pos, uint8_t brightness) {
    HSV hsv{
        static_cast<float>(map(wheel_pos, 0, 255, 0, 360)), 100, static_cast<float>(map(brightness, 0, 255, 0, 100))};
    return convertToRGBA(hsv);
}