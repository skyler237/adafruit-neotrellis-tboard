//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_APPS_SPLASH_SCREEN_H
#define TBOARD_APPS_SPLASH_SCREEN_H

#include "os/application.h"

// #include "includes.h"

namespace tboard::apps {

class SplashScreen : public Application {
public:
    SplashScreen(const TrellisControllerPtr& trellis_controller, const ApplicationSwitcherPtr& application_switcher)
        : Application(trellis_controller, application_switcher) { }

    ~SplashScreen() override;

    ApplicationId get_id() const override;
    int get_tick_period_ms() const override;
    void init() override;
    void exit() override;
    tl::optional<ApplicationId> tick(const Time& now) override;

private:
    int pixel_idx_{0};
};

} // namespace tboard::apps
#endif // TBOARD_APPS_SPLASH_SCREEN_H