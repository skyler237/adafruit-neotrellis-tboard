//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "trellis_controller.h"

#include <iostream>

namespace tboard {
TrellisDisplay::TrellisDisplay(TrellisInterfacePtr trellis_interface) : interface_(std::move(trellis_interface)) { }

void TrellisDisplay::set_pixel_color(const int x, const int y, RGBA color, float brightness) {
    const RGBA adjusted_color = set_brightness(color, brightness);
    interface_->set_pixel_color(
        x, y, adjusted_color.colors.red, adjusted_color.colors.green, adjusted_color.colors.blue);
    pixel_colors_(x, y) = color;
}

void TrellisDisplay::set_pixel_on(int x, int y) {
    set_pixel_color(x, y, {255, 255, 255});
}

void TrellisDisplay::set_pixel_off(int x, int y) {
    set_pixel_color(x, y, {0, 0, 0});
}

void TrellisDisplay::toggle_pixel(int x, int y, tl::optional<RGBA> color) {
    if (is_pixel_on(x, y)) {
        set_pixel_off(x, y);
    } else {
        if (color.has_value()) {
            set_pixel_color(x, y, color.value());
        } else {
            set_pixel_on(x, y);
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

void TrellisDisplay::clear() {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            set_pixel_off(x, y);
        }
    }
}

void TrellisDisplay::show() const {
    interface_->show();
}

RGBA TrellisDisplay::set_brightness(RGBA color, float brightness) {
    return {static_cast<uint8_t>(color.colors.red * brightness),
            static_cast<uint8_t>(color.colors.green * brightness),
            static_cast<uint8_t>(color.colors.blue * brightness),
            color.colors.alpha};
}

TrellisController::TrellisController(TrellisInterfacePtr trellis_interface)
    : interface_(std::move(trellis_interface)), trellis_display_(std::make_shared<TrellisDisplay>(interface_)) { }

void TrellisController::register_on_pressed_callback(int x, int y, OnEventCallback callback) const {
    interface_->register_on_pressed_callback(x, y, std::move(callback));
}

void TrellisController::register_on_released_callback(int x, int y, OnEventCallback callback) const {
    interface_->register_on_released_callback(x, y, std::move(callback));
}

void TrellisController::set_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) const {
    interface_->register_on_any_key_pressed_callback(std::move(callback));
}

void TrellisController::set_on_any_key_released_callback(OnAnyKeyEventCallback callback) const {
    interface_->register_on_any_key_released_callback(std::move(callback));
}

void TrellisController::add_timer_callback(const int period_ms, OnTimerEventCallback callback) const {
    interface_->register_timer_callback(period_ms, std::move(callback));
}

void TrellisController::clear_callbacks() const {
    interface_->clear_callbacks();
}

} // namespace tboard