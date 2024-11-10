//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "colors.h"

#include <USB/USBAPI.h>

#include <WMath.h>
#include <cmath>

#include "ColorConverters.h"
#include "HSV.h"

namespace {
float interpolate(float start, float end, float ratio) {
    if (std::fabs(end - start) < 1e-6) {
        return start;
    }
    return start + ((end - start) * ratio);
}
} // namespace

RGBA wheel_rgb(uint8_t wheel_pos, uint8_t brightness) {
    HSV hsv{
        static_cast<float>(map(wheel_pos, 0, 255, 0, 360)), 100, static_cast<float>(map(brightness, 0, 255, 0, 100))};
    return convertToRGBA(hsv);
}

RGBA interpolate_color(const RGBA& from_color, const RGBA& to_color, float ratio) {
    if (ratio > 1) {
        ratio = 1;
    } else if (ratio < 0) {
        ratio = 0;
    }

    HSV hsv1 = convertToHSV(from_color);
    HSV hsv2 = convertToHSV(to_color);

    // handle hue wrap-around
    if (std::fabs(hsv1.hue - hsv2.hue) > 180) {
        if (hsv1.hue > hsv2.hue) {
            hsv2.hue += 360;
        } else {
            hsv1.hue += 360;
        }
    }

    float interpolated_hue = interpolate(hsv1.hue, hsv2.hue, ratio);

    if (interpolated_hue > 360) {
        interpolated_hue -= 360;
    }

    // HSV interpolated_hsv{interpolated_hue,
    //                      interpolate(hsv1.saturation, hsv2.saturation, ratio),
    //                      interpolate(hsv1.value, hsv2.value, ratio)};

    HSV interpolated_hsv{interpolated_hue,
                         100,
                         100};

    return convertToRGBA(interpolated_hsv);
}
