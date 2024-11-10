//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "trellis_display.h"

#include "resources/minifont.h"

namespace tboard {
TrellisDisplay::TrellisDisplay(TrellisInterfacePtr trellis_interface) : interface_(std::move(trellis_interface)) { }

void TrellisDisplay::set_pixel_color(const int x, const int y, RGBA color, float brightness) {
    const RGBA adjusted_color = get_color_at_brightness(color, brightness);
    interface_->set_pixel_color(
        x, y, adjusted_color.colors.red, adjusted_color.colors.green, adjusted_color.colors.blue);
    pixel_colors_(x, y) = color;
}

void TrellisDisplay::set_pixel_on(int x, int y, float brightness) {
    set_pixel_color(x, y, {255, 255, 255}, brightness);
}

void TrellisDisplay::set_pixel_off(int x, int y) {
    set_pixel_color(x, y, {0, 0, 0});
}

void TrellisDisplay::toggle_pixel(int x, int y, tl::optional<RGBA> color, float brightness) {
    if (is_pixel_on(x, y)) {
        set_pixel_off(x, y);
    } else {
        if (color.has_value()) {
            set_pixel_color(x, y, color.value(), brightness);
        } else {
            set_pixel_on(x, y, brightness);
        }
    }
}

RGBA TrellisDisplay::get_pixel_color(int x, int y) const {
    return pixel_colors_(x, y);
}

bool TrellisDisplay::is_pixel_on(int x, int y) const {
    return get_pixel_color(x, y).colors.red > 0 || get_pixel_color(x, y).colors.green > 0 ||
           get_pixel_color(x, y).colors.blue > 0;
}

bool TrellisDisplay::are_all_pixels_off() const {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (is_pixel_on(x, y)) {
                return false;
            }
        }
    }
    return true;
}

void TrellisDisplay::clear() {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            set_pixel_off(x, y);
        }
    }
}



void TrellisDisplay::draw_character(int x, int y, char c, RGBA color, float brightness) {
    minifont::Bitmap bitmap = minifont::get_bitmap(c);
    for (int i = 0; i < minifont::CHAR_WIDTH; ++i) {
        for (int j = 0; j < minifont::CHAR_HEIGHT; ++j) {
            if (bitmap[j][i] > 0) {
                set_pixel_color(x + i, y + j, color, brightness);
            }
        }
    }
}

void TrellisDisplay::show() const {
    interface_->show();
}

RGBA TrellisDisplay::get_color_at_brightness(RGBA color, float brightness) {
    return {static_cast<uint8_t>(color.colors.red * brightness),
            static_cast<uint8_t>(color.colors.green * brightness),
            static_cast<uint8_t>(color.colors.blue * brightness),
            color.colors.alpha};
}
}  // namespace tboard