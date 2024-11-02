//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_TRELLIS_CONTROLLER_H
#define TBOARD_TRELLIS_CONTROLLER_H
#include "colors.h"
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

class TrellisController {
public:
    explicit TrellisController(TrellisInterfacePtr trellis_interface);

    // Get the display
    const TrellisDisplayPtr& display() const { return trellis_display_; }

    // Register callbacks for button presses
    void register_on_pressed_callback(int x, int y, OnEventCallback callback) const;
    void register_on_released_callback(int x, int y, OnEventCallback callback) const;
    void set_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) const;
    void set_on_any_key_released_callback(OnAnyKeyEventCallback callback) const;
    void add_timer_callback(int period_ms, OnTimerEventCallback callback) const;
    void clear_callbacks() const;

private:
    TrellisInterfacePtr interface_;
    TrellisDisplayPtr trellis_display_;
};

using TrellisControllerPtr = std::shared_ptr<TrellisController>;

} // namespace tboard
#endif // TBOARD_TRELLIS_CONTROLLER_H