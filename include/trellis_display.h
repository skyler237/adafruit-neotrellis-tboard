//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#pragma once
#include "RGBA.h"
#include "optional.h"
#include "trellis_interface.h"

namespace tboard {
/// Handles maintaining and modifying the state of the Trellis display
class TrellisDisplay {
public:
    explicit TrellisDisplay(TrellisInterfacePtr trellis_interface);

    void set_pixel_color(int x, int y, RGBA color, float brightness = 1.0);

    // Sets the pixel to fully on (white)
    void set_pixel_on(int x, int y, float brightness = 1.0);

    // Clears the pixel
    void set_pixel_off(int x, int y);

    // Toggle the pixel on or off
    void toggle_pixel(int x, int y, tl::optional<RGBA> color = tl::nullopt, float brightness = 1.0);

    RGBA get_pixel_color(int x, int y) const;

    bool is_pixel_on(int x, int y) const;

    bool are_all_pixels_on() const;

    bool are_all_pixels_off() const;

    // Turn all pixels off
    void clear();

    void fill(RGBA color);

    // Text handling
    void draw_character(int x, int y, char c, RGBA color, float brightness = 1.0);

    // Render pixels to the display
    void show() const;

private:
    TrellisInterfacePtr interface_;
    Grid8x8<RGBA> pixel_colors_;

    static RGBA get_color_at_brightness(RGBA color, float brightness);
};

inline bool TrellisDisplay::are_all_pixels_on() const {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (!is_pixel_on(x, y)) {
                return false;
            }
        }
    }
    return true;
}

inline void TrellisDisplay::fill(RGBA color) {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            set_pixel_color(x, y, color);
        }
    }
}

using TrellisDisplayPtr = std::shared_ptr<TrellisDisplay>;

} // namespace tboard