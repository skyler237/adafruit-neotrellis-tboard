//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_COLORS_H
#define TBOARD_COLORS_H

#include <cstdint>
#include <NColor.h>


RGBA wheel_rgb(uint8_t wheel_pos, uint8_t brightness = 255);
#endif // TBOARD_COLORS_H