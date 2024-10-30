//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_OS_TBOARD_OS_H
#define TBOARD_OS_TBOARD_OS_H
#include <map>

#include "application.h"
#include "trellis_controller.h"
#include "trellis_interface.h"

namespace tboard {

/// This is the top-level class for managing the trellis board and it's applications
class TBoardOS {
public:
    explicit TBoardOS(TrellisInterfacePtr trellis_interface);

    template<typename AppT>
//        requires std::derived_from<AppT, Application>
    void register_app();
    void set_startup_app(const tboard::ApplicationId& application_id);
    void run();

private:
    std::shared_ptr<tboard::TrellisController> trellis_controller_;
    std::map<ApplicationId, std::shared_ptr<Application>> apps_;
    ApplicationPtr current_app_;
    ApplicationSwitcherPtr application_switcher_;

    Time next_tick_time_{0};

    void run_timer_callback(const Time& now);
    void switch_to_app(const ApplicationId& to_app);
};

template<typename AppT>
void TBoardOS::register_app() {
    auto app = std::make_shared<AppT>(trellis_controller_, application_switcher_);
    const auto& app_id = app->get_id();
    const auto it = apps_.find(app_id);
    if (it != apps_.end()) {
      return;
    }
    apps_.emplace(app_id, app);
}

} // namespace tboard
#endif // TBOARD_OS_TBOARD_OS_H