//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "apps/application_picker.h"

#include <USB/USBAPI.h>

namespace tboard::apps {
ApplicationPicker::~ApplicationPicker() = default;

ApplicationId ApplicationPicker::get_id() const {
    return APPLICATION_PICKER_ID;
}

int ApplicationPicker::get_tick_period_ms() const {
    return 0;
}

void ApplicationPicker::init() {
    // Setup display
    trellis_controller_->display()->clear();
    int idx = 0;
    for (const auto& [app_id, color] : APPLICATION_LIST) {
        trellis_controller_->display()->set_pixel_color(idx % 8, idx / 8, color);
        idx++;
    }
    trellis_controller_->display()->show();

    // Setup callbacks
    trellis_controller_->add_on_any_key_pressed_callback([this](int x, int y, const Time&) {
        int idx = y * 8 + x;
        if (idx < APPLICATION_LIST.size()) {
            switch_to_app(APPLICATION_LIST[idx].first);
        }
    });
}

void ApplicationPicker::exit() {
    trellis_controller_->clear_callbacks();
}

tl::optional<ApplicationId> ApplicationPicker::tick(const Time& now) {
    return tl::nullopt;
}
} // namespace tboard::apps