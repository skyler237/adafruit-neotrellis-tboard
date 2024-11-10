//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_APPS_APPLICATION_PICKER_H
#define TBOARD_APPS_APPLICATION_PICKER_H

#include <map>

#include "apps/application_ids.h"
#include "os/application.h"

namespace tboard::apps {

// Define the order and color for each application that will show up
static const std::vector<std::pair<ApplicationId, RGBA>> APPLICATION_LIST = {
    {            BATTERY_ID,    RED},
    {       COLORING_PAD_ID,   BLUE},
    {RANDOM_COLOR_TOGGLE_ID, PURPLE},
    {         LIGHTS_OUT_ID,  GREEN},
};

class ApplicationPicker : public Application {
public:
    ApplicationPicker(const TrellisControllerPtr& trellis_controller,
                      const ApplicationSwitcherPtr& application_switcher)
        : Application(trellis_controller, application_switcher) { }

    ~ApplicationPicker() override;

    ApplicationId get_id() const override;
    int get_tick_period_ms() const override;
    void init() override;
    void exit() override;
    tl::optional<ApplicationId> tick(const Time& now) override;
};

} // namespace tboard::apps
#endif // TBOARD_APPS_APPLICATION_PICKER_H