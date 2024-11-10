//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#pragma once

#include <array>
#include <map>

/// Note that the last row is typically blank because it is reserved for the descender.

namespace minifont {
constexpr int CHAR_WIDTH = 3;
constexpr int CHAR_HEIGHT = 5;
using Bitmap = std::array<std::array<uint8_t, CHAR_WIDTH>, CHAR_HEIGHT>;
static constexpr Bitmap A_UPPER = {
    {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {0, 0, 0}}
};

static constexpr Bitmap B_UPPER = {
    {{1, 1, 0}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap C_UPPER = {
    {{1, 1, 1}, {1, 0, 0}, {1, 0, 0}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap D_UPPER = {
    {{1, 1, 0}, {1, 0, 1}, {1, 0, 1}, {1, 1, 0}, {0, 0, 0}}
};

static constexpr Bitmap E_UPPER = {
    {{1, 1, 1}, {1, 1, 0}, {1, 0, 0}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap F_UPPER = {
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 0}, {1, 0, 0}, {0, 0, 0}}
};

// ...

/// Numbers
static constexpr Bitmap ZERO = {
    {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap ONE = {
    {{1, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 0, 0}}
};

static constexpr Bitmap TWO = {
    {{1, 1, 1}, {0, 0, 1}, {1, 1, 0}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap THREE = {
    {{1, 1, 1}, {0, 1, 1}, {0, 0, 1}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap FOUR = {
    {{1, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 0}}
};

static constexpr Bitmap FIVE = {
    {{1, 1, 1}, {1, 1, 0}, {0, 0, 1}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap SIX = {
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap SEVEN = {
    {{1, 1, 1}, {0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {0, 0, 0}}
};

static constexpr Bitmap EIGHT = {
    {{1, 1, 1}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 0}}
};

static constexpr Bitmap NINE = {
    {{1, 1, 1}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {0, 0, 0}}
};

// Map of characters to their bitmaps
static const std::map<char, Bitmap> CHAR_MAP = {
  // Uppercase
    {'A', A_UPPER},
    {'B', B_UPPER},
    {'C', C_UPPER},
    {'D', D_UPPER},
    {'E', E_UPPER},
    {'F', F_UPPER},
 // ...
  // Lowercase
  // ...
  // Numbers
    {'0',    ZERO},
    {'1',     ONE},
    {'2',     TWO},
    {'3',   THREE},
    {'4',    FOUR},
    {'5',    FIVE},
    {'6',     SIX},
    {'7',   SEVEN},
    {'8',   EIGHT},
    {'9',    NINE}
};

static Bitmap get_bitmap(const char c) {
    const auto it = CHAR_MAP.find(c);
    if (it == CHAR_MAP.end()) {
        return {};
    }
    return it->second;
}

} // namespace minifont