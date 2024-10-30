//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "os/tboard_os.h"

#include <iostream>

#include "trellis_controller.h"

namespace tboard {
TBoardOS::TBoardOS(TrellisInterfacePtr trellis_interface)
    : trellis_controller_(std::make_shared<TrellisController>(std::move(trellis_interface))),
      application_switcher_(std::make_shared<ApplicationSwitcher>()) {
    application_switcher_->register_on_application_change_request_callback(
        [this](const ApplicationId& to_app) {
            switch_to_app(to_app);
        });
}

void TBoardOS::set_startup_app(const tboard::ApplicationId& application_id) {
    current_app_ = apps_.at(application_id);
}

void TBoardOS::run() {
    current_app_->init();
    trellis_controller_->add_timer_callback(1,
                                            [this](const Time& now) { run_timer_callback(now); return 1;});
}

void TBoardOS::switch_to_app(const ApplicationId& to_app) {
    current_app_->exit();
    auto new_app = apps_.at(to_app);
    new_app->init();
    // Reset next tick time
    next_tick_time_ = Time();
    current_app_ = std::move(new_app);
}

void TBoardOS::run_timer_callback(const Time& now) {
    tl::optional<ApplicationId> maybe_next_app;
    // Check current app's tick rate
    int tick_period_ms = current_app_->get_tick_period_ms();
    if (tick_period_ms > 0 && now >= next_tick_time_) {
        // Run the current app's tick function
        // TODO: fix this
        maybe_next_app = current_app_->tick(0);
        next_tick_time_ = now + Duration{0};
    }
    // Check for a request to switch to a new app
    if (maybe_next_app.has_value()) {
        // Switch to the next app
        switch_to_app(maybe_next_app.value());
    }
}
} // namespace tboard