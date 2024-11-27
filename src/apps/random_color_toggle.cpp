//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "apps/random_color_toggle.h"

#include <USB/USBAPI.h>

#include <ColorConverters.h>
#include <HSV.h>
#include <iostream>
#include <random>

#include "apps/application_ids.h"

// #include "includes.h"

namespace tboard::apps {
RandomColorToggle::~RandomColorToggle() = default;

RandomColorToggle::RandomColorToggle(const TrellisControllerPtr& trellis_controller,
                                     const ApplicationSwitcherPtr& application_switcher)
    : Application(trellis_controller, application_switcher), generator_(random_()), distribution_(0, 1) { }

ApplicationId RandomColorToggle::get_id() const {
    return RANDOM_COLOR_TOGGLE_ID;
}

int RandomColorToggle::get_tick_period_ms() const {
    return 0;
}

void RandomColorToggle::init() {
    generator_.seed(millis());
    trellis_controller_->display()->clear();
    trellis_controller_->display()->show();
    // trellis_controller_->clear_callbacks();
    trellis_controller_->add_on_any_key_pressed_callback([this](int x, int y, const Time&) {
        // Generate random color
        trellis_controller_->display()->toggle_pixel(x, y, get_random_color());
        ;
        trellis_controller_->display()->show();
    });
    // Add "exit" button by holding top right corner
    trellis_controller_->add_on_key_held_callback(
        1000, 7, 0, [this](const Time&) { switch_to_app(APPLICATION_PICKER_ID); });
}

void RandomColorToggle::exit() {
    /* do nothing */
}

tl::optional<ApplicationId> RandomColorToggle::tick(const Time& now) {
    return tl::nullopt;
}

RGBA RandomColorToggle::get_random_color() {
    constexpr float BRIGHTNESS_VALUE = 80;
    const HSV hsv{static_cast<float>(distribution_(generator_) * 360),
                  100 - static_cast<float>(distribution_(generator_) * 10),
                  BRIGHTNESS_VALUE};
    return convertToRGBA(hsv);
}
} // namespace tboard::apps