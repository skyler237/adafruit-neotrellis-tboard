//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "os/application.h"

namespace tboard {
void ApplicationSwitcher::request_application_change(const ApplicationId& application_id) const {
        if (on_application_change_request_callback_) {
                on_application_change_request_callback_(application_id);
        }
}

void ApplicationSwitcher::register_on_application_change_request_callback(ApplicationChangeRequestCallback callback) {
    on_application_change_request_callback_ = std::move(callback);
}

Application::Application(TrellisControllerPtr trellis_controller, ApplicationSwitcherPtr application_switcher)
    : trellis_controller_(std::move(trellis_controller)), application_switcher_(std::move(application_switcher)) {}

void Application::switch_to_app(const ApplicationId& application_id) const {
    application_switcher_->request_application_change(application_id);
}
} // namespace tboard