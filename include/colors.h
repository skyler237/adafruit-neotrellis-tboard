//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_COLORS_H
#define TBOARD_COLORS_H

#include <cstdint>

#include "RGBA.h"

RGBA wheel_rgb(uint8_t wheel_pos, uint8_t brightness = 255);

// Named colors
static const RGBA BLACK = {0, 0, 0, 255};
static const RGBA WHITE = {255, 255, 255, 255};
static const RGBA RED = {255, 0, 0, 255};
static const RGBA LIME = {0, 255, 0, 255};
static const RGBA BLUE = {0, 0, 255, 255};
static const RGBA YELLOW = {255, 255, 0, 255};
static const RGBA CYAN = {0, 255, 255, 255};
static const RGBA MAGENTA = {255, 0, 255, 255};
static const RGBA SILVER = {192, 192, 192, 255};
static const RGBA GRAY = {128, 128, 128, 255};
static const RGBA MAROON = {128, 0, 0, 255};
static const RGBA OLIVE = {128, 128, 0, 255};
static const RGBA GREEN = {0, 128, 0, 255};
static const RGBA PURPLE = {128, 0, 128, 255};
static const RGBA TEAL = {0, 128, 128, 255};
static const RGBA NAVY = {0, 0, 128, 255};

static const RGBA DARK_RED = {139, 0, 0, 255};
static const RGBA BROWN = {165, 42, 42, 255};
static const RGBA FIREBRICK = {178, 34, 34, 255};
static const RGBA CRIMSON = {220, 20, 60, 255};
static const RGBA TOMATO = {255, 99, 71, 255};
static const RGBA CORAL = {255, 127, 80, 255};
static const RGBA INDIAN_RED = {205, 92, 92, 255};
static const RGBA LIGHT_CORAL = {240, 128, 128, 255};
static const RGBA DARK_SALMON = {233, 150, 122, 255};
static const RGBA SALMON = {250, 128, 114, 255};
static const RGBA LIGHT_SALMON = {255, 160, 122, 255};
static const RGBA ORANGE_RED = {255, 69, 0, 255};
static const RGBA DARK_ORANGE = {255, 140, 0, 255};
static const RGBA ORANGE = {255, 165, 0, 255};
static const RGBA GOLD = {255, 215, 0, 255};

static const RGBA DARK_GOLDEN_ROD = {184, 134, 11, 255};
static const RGBA GOLDEN_ROD = {218, 165, 32, 255};
static const RGBA PALE_GOLDEN_ROD = {238, 232, 170, 255};
static const RGBA DARK_KHAKI = {189, 183, 107, 255};
static const RGBA KHAKI = {240, 230, 140, 255};
static const RGBA YELLOW_GREEN = {154, 205, 50, 255};
static const RGBA DARK_OLIVE_GREEN = {85, 107, 47, 255};
static const RGBA OLIVE_DRAB = {107, 142, 35, 255};
static const RGBA LAWN_GREEN = {124, 252, 0, 255};
static const RGBA CHARTREUSE = {127, 255, 0, 255};
static const RGBA GREEN_YELLOW = {173, 255, 47, 255};
static const RGBA DARK_GREEN = {0, 100, 0, 255};
static const RGBA FOREST_GREEN = {34, 139, 34, 255};
static const RGBA LIME_GREEN = {50, 205, 50, 255};
static const RGBA LIGHT_GREEN = {144, 238, 144, 255};
static const RGBA PALE_GREEN = {152, 251, 152, 255};

static const RGBA DARK_SEA_GREEN = {143, 188, 143, 255};
static const RGBA MEDIUM_SPRING_GREEN = {0, 250, 154, 255};
static const RGBA SPRING_GREEN = {0, 255, 127, 255};
static const RGBA SEA_GREEN = {46, 139, 87, 255};
static const RGBA MEDIUM_AQUAMARINE = {102, 205, 170, 255};
static const RGBA MEDIUM_SEA_GREEN = {60, 179, 113, 255};
static const RGBA LIGHT_SEA_GREEN = {32, 178, 170, 255};
static const RGBA DARK_SLATE_GRAY = {47, 79, 79, 255};
static const RGBA DARK_CYAN = {0, 139, 139, 255};
static const RGBA AQUA = {0, 255, 255, 255};
static const RGBA LIGHT_CYAN = {224, 255, 255, 255};
static const RGBA DARK_TURQUOISE = {0, 206, 209, 255};
static const RGBA TURQUOISE = {64, 224, 208, 255};
static const RGBA MEDIUM_TURQUOISE = {72, 209, 204, 255};
static const RGBA PALE_TURQUOISE = {175, 238, 238, 255};
static const RGBA LIGHT_BLUE = {173, 216, 230, 255};
#endif // TBOARD_COLORS_H