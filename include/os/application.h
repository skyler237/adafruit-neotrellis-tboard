//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_OS_APPLICATION_H
#define TBOARD_OS_APPLICATION_H

#include "trellis_controller.h"

namespace tboard {

struct ApplicationId {
    int id;
    std::string name;

    friend bool operator<(const ApplicationId& lhs, const ApplicationId& rhs) {
        if (lhs.id < rhs.id)
            return true;
        if (rhs.id < lhs.id)
            return false;
        return lhs.name < rhs.name;
    }
};

using ApplicationChangeRequestCallback = std::function<void(const ApplicationId&)>;

/// Responsible for managing signals to switch between applications
class ApplicationSwitcher {
public:
    void request_application_change(const ApplicationId& application_id) const;

    void register_on_application_change_request_callback(ApplicationChangeRequestCallback callback);

private:
    ApplicationChangeRequestCallback on_application_change_request_callback_;
};
using ApplicationSwitcherPtr = std::shared_ptr<ApplicationSwitcher>;

/// Base class for all applications
class Application {
public:
    explicit Application(TrellisControllerPtr trellis_controller, ApplicationSwitcherPtr application_switcher);
    virtual ~Application() = default;

    // Get the application's ID
    virtual ApplicationId get_id() const = 0;

    // Get the desired period between ticks in milliseconds for the application
    virtual int get_tick_period_ms() const = 0;

    // Perform any initialization required by the application
    virtual void init() = 0;

    // Perform any cleanup required by the application
    virtual void exit() = 0;

    // Perform any processing required by the application
    virtual tl::optional<ApplicationId> tick(const Time& now) = 0;

protected:
    void switch_to_app(const ApplicationId& application_id) const;

    const TrellisControllerPtr trellis_controller_;

private:
    const ApplicationSwitcherPtr application_switcher_;
};
using ApplicationPtr = std::shared_ptr<Application>;

} // namespace tboard
#endif // TBOARD_OS_APPLICATION_H