//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "os/tboard_os.h"

#include <USB/USBAPI.h>

#include <iostream>

#include "trellis_controller.h"

namespace tboard {
TBoardOS::TBoardOS(TrellisInterfacePtr trellis_interface)
    : trellis_controller_(std::make_shared<TrellisController>(std::move(trellis_interface))),
      application_switcher_(std::make_shared<ApplicationSwitcher>()) {
    application_switcher_->register_on_application_change_request_callback(
        [this](const ApplicationId& to_app) { switch_to_app(to_app); });
}

void TBoardOS::set_startup_app(const tboard::ApplicationId& application_id) {
    current_app_ = apps_.at(application_id);
}

void TBoardOS::start() {
    current_app_->init();
    trellis_controller_->add_timer_callback(1, [this](const Time& now) { return run_timer_callback(now); });
}

void TBoardOS::switch_to_app(const ApplicationId& to_app) {
    current_app_->exit();
    auto new_app = apps_.at(to_app);
    new_app->init();
    // Reset next tick time
    next_tick_time_ = Time();
    current_app_ = std::move(new_app);
}

Duration TBoardOS::run_timer_callback(const Time& now) {
    tl::optional<ApplicationId> maybe_next_app;
    Serial.printf("TBoardOS::run_timer_callback: now=%lu, current_app=%s\n", now, current_app_->get_id().name.c_str());
    // Run the current app's tick function
    maybe_next_app = current_app_->tick(now);
    // Check for a request to switch to a new app
    if (maybe_next_app.has_value()) {
        // Switch to the next app
        switch_to_app(maybe_next_app.value());
    }
    return current_app_->get_tick_period_ms();
}
} // namespace tboard