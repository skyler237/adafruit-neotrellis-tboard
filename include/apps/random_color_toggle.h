//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_APPS_RANDOM_COLOR_TOGGLE_H
#define TBOARD_APPS_RANDOM_COLOR_TOGGLE_H

#include <random>

#include "os/application.h"

namespace tboard::apps {

class RandomColorToggle final : public Application {
public:
    ~RandomColorToggle() override;

    RandomColorToggle(const TrellisControllerPtr& trellis_controller,
                      const ApplicationSwitcherPtr& application_switcher);

    ApplicationId get_id() const override;
    int get_tick_period_ms() const override;
    void init() override;
    void exit() override;
    tl::optional<ApplicationId> tick(const Time& now) override;

private:
    std::random_device random_;
    std::mt19937 generator_;
    std::uniform_real_distribution<> distribution_;

    RGBA get_random_color();
};

} // namespace tboard::apps
#endif // TBOARD_APPS_RANDOM_COLOR_TOGGLE_H