//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "apps/random_color_toggle.h"

#include <iostream>
#include <random>

#include "apps/application_ids.h"
// #include "includes.h"

namespace tboard::apps {
RandomColorToggle::~RandomColorToggle() = default;

RandomColorToggle::RandomColorToggle(const TrellisControllerPtr& trellis_controller,
                                     const ApplicationSwitcherPtr& application_switcher)
    : Application(trellis_controller, application_switcher), generator_(random_()), distribution_(0, 255) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
}

ApplicationId RandomColorToggle::get_id() const { return RANDOM_COLOR_TOGGLE_ID; }

int RandomColorToggle::get_tick_period_ms() const { return 0; }

void RandomColorToggle::init() {
    trellis_controller_->display()->clear();
    trellis_controller_->display()->show();
    // trellis_controller_->clear_callbacks();
    trellis_controller_->set_on_any_key_pressed_callback([this](int x, int y, const Time&) {
        // Generate random color
        trellis_controller_->display()->toggle_pixel(x, y, get_random_color());;
        trellis_controller_->display()->show();
    });

    trellis_controller_->register_on_pressed_callback(0,
                                                      0,
                                                      [this](const Time&) {
                                                          switch_to_app(COLORING_PAD_ID);
                                                      });
}

void RandomColorToggle::exit() {
    /* do nothing */
}

tl::optional<ApplicationId> RandomColorToggle::tick(const Time& now) {
    return tl::nullopt;
}

RGBA RandomColorToggle::get_random_color() {
    return RGBA{static_cast<uint8_t>(distribution_(generator_)), static_cast<uint8_t>(distribution_(generator_)), static_cast<uint8_t>(distribution_(generator_))};
}
} // namespace tboard::apps