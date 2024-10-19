//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "colors.h"

namespace {
template<typename T>
const T& clamp(const T& v, const T& lo, const T& hi) {
    if ((hi < v)) {
        return (v < lo) ? lo : hi;
    }
    return (v < lo) ? lo : v;
}
} // namespace

RGB::RGB() = default;

RGB::RGB(int r, int g, int b) : r(clamp(r, 0, 255)), g(clamp(g, 0, 255)), b(clamp(b, 0, 255)) { }

RGB wheel_rgb(uint8_t wheel_pos) {
    if (wheel_pos < 85) {
        return {wheel_pos * 3, 255 - (wheel_pos * 3), 0};
    }
    if (wheel_pos < 170) {
        wheel_pos -= 85;
        return {255 - (wheel_pos * 3), 0, wheel_pos * 3};
    }
    wheel_pos -= 170;
    return {0, wheel_pos * 3, 255 - (wheel_pos * 3)};
}