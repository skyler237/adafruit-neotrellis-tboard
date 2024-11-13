//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "apps/coloring_pad.h"

#include "apps/application_ids.h"

namespace tboard::apps {
ColorPicker::ColorPicker(TrellisDisplayPtr display) : display_(std::move(display)) {
    /// Setup colors -- this is the brighest version, for when selected
    // TODO: maybe distinguish between selected, unselected, and paint colors
    // Red
    colors_[0] = RED;
    // Orange
    colors_[1] = ORANGE_RED;
    // Yellow
    colors_[2] = YELLOW;
    // Green
    colors_[3] = GREEN;
    // Blue
    colors_[4] = BLUE;
    // Magenta
    colors_[5] = PURPLE;
    // White
    colors_[6] = WHITE;
    // Eraser
    colors_[7] = BLACK;
}

void ColorPicker::handle_button_pressed(int x, int y) {
    if (y == COLOR_PICKER_ROW) {
        selected_color_ = colors_[x];
        selected_color_idx_ = x;
        draw();
    }
}

tl::optional<RGBA> ColorPicker::get_selected_color() const {
    return selected_color_;
}

RGBA ColorPicker::get_color_at(int x) const {
    return colors_[x];
}

void ColorPicker::draw() const {
    for (int i = 0; i < 8; ++i) {
        float brightness = DEFAULT_COLOR_PAD_BRIGHTNESS;
        if (selected_color_idx_ == i) {
            brightness = 1.0;
        }
        display_->set_pixel_color(i, COLOR_PICKER_ROW, colors_[i], brightness);
    }
    display_->show();
}

ColoringPad::~ColoringPad() = default;

ApplicationId ColoringPad::get_id() const {
    return COLORING_PAD_ID;
}

int ColoringPad::get_tick_period_ms() const {
    // No tick required since everything is event based
    return 0;
}

void ColoringPad::init() {
    // Clear the screen
    trellis_controller_->display()->clear();

    // Draw the color picker
    color_picker_ = std::make_shared<ColorPicker>(trellis_controller_->display());
    color_picker_->draw();

    trellis_controller_->display()->show();

    // Setup callbacks
    trellis_controller_->add_on_any_key_pressed_callback([this](int x, int y, const Time&) {
        if (y == ColorPicker::COLOR_PICKER_ROW) {
            color_picker_->handle_button_pressed(x, y);
        } else if (color_picker_->get_selected_color().has_value()) {
            trellis_controller_->display()->set_pixel_color(
                x, y, color_picker_->get_selected_color().value(), DEFAULT_COLOR_PAD_BRIGHTNESS);
            trellis_controller_->display()->show();
        }
    });

    // Add key held callback to fill screen if a color key is held
    constexpr Duration FILL_HOLD_DURATION_MS = 1500;
    trellis_controller_->add_on_any_key_held_callback(FILL_HOLD_DURATION_MS, [this](int x, int y, const Time&) {
        if (y == ColorPicker::COLOR_PICKER_ROW) {
            const RGBA color = color_picker_->get_color_at(x);
            trellis_controller_->display()->fill_region(color, 0, 0, 8, 7, DEFAULT_COLOR_PAD_BRIGHTNESS);
            trellis_controller_->display()->show();
        }
    });

    // Add key held callback to return to application picker if the top right button is held
    constexpr Duration EXIT_APP_HOLD_DURATION_MS = 2000;
    trellis_controller_->add_on_key_held_callback(
        EXIT_APP_HOLD_DURATION_MS, 7, 0, [this](const Time&) { switch_to_app(APPLICATION_PICKER_ID); });
}

void ColoringPad::exit() { }

tl::optional<ApplicationId> ColoringPad::tick(const Time&) {
    return tl::nullopt;
}
} // namespace tboard::apps