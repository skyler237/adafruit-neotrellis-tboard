//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#pragma once

#include "os/application.h"
#include "utils.h"

namespace tboard::apps {


constexpr float LPF_ALPHA = 0.05;

class BatteryBar {
public:
    explicit BatteryBar(TrellisControllerPtr trellis_controller);

    void display_battery_percentage(float battery_percentage) const;

private:
    TrellisControllerPtr trellis_controller_;
};

class Battery : public Application {
public:
    Battery(const TrellisControllerPtr& trellis_controller, const ApplicationSwitcherPtr& application_switcher);


    ~Battery() override;

    ApplicationId get_id() const override;
    int get_tick_period_ms() const override;
    void init() override;
    void exit() override;
    tl::optional<ApplicationId> tick(const Time& now) override;

private:
    BatteryBar battery_bar_;
    LowPassFilter lpf_{LPF_ALPHA};

    void display_percentage_text(float battery_percentage) const;
    void display_battery_percentage(float battery_percentage) const;
};

} // namespace tboard::apps