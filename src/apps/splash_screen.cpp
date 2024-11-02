//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #include "includes.h"
#include "apps/splash_screen.h"

#include <WMath.h>
#include <iostream>

#include "apps/application_ids.h"
//#include "apps/coloring_pad.h"
//#include "apps/random_color_toggle.h"

namespace tboard::apps {
SplashScreen::~SplashScreen() = default;

ApplicationId SplashScreen::get_id() const {
    return SPLASHSCREEN_ID;
}

int SplashScreen::get_tick_period_ms() const {
    return 20;
}

void SplashScreen::init() {
    pixel_idx_ = 0;
    trellis_controller_->display()->clear();
}

void SplashScreen::exit() { }

tl::optional<ApplicationId> SplashScreen::tick(const Time& now) {
    if (pixel_idx_ < 64) {
    const int x = pixel_idx_ % 8;
    const int y = pixel_idx_ / 8;

        // Set pixel color
        trellis_controller_->display()->set_pixel_color(x, y, wheel_rgb(map(pixel_idx_, 0, 8 * 8, 0, 255), 50)); // addressed
        //
        // trellis_controller_->display()->set_pixel_color(pixel_idx_ % 8,
        //                                                 pixel_idx_ / 8,
        //                                                 // Sweep from red to blue
        //                                                 RGBA(255 - (pixel_idx_ * 4), 0, pixel_idx_ * 4));
        pixel_idx_++;
        trellis_controller_->display()->show();
    } else {
        return APPLICATION_PICKER_ID;
    }
    return tl::nullopt;
}
} // namespace tboard::apps