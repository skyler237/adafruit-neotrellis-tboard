//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "trellis_controller.h"

#include <iostream>

namespace tboard {

TrellisController::TrellisController(TrellisInterfacePtr trellis_interface)
    : interface_(std::move(trellis_interface)), trellis_display_(std::make_shared<TrellisDisplay>(interface_)) { }

void TrellisController::register_on_pressed_callback(int x, int y, OnEventCallback callback) const {
    interface_->add_on_pressed_callback(x, y, std::move(callback));
}

void TrellisController::register_on_released_callback(int x, int y, OnEventCallback callback) const {
    interface_->add_on_released_callback(x, y, std::move(callback));
}

void TrellisController::set_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) const {
    interface_->add_on_any_key_pressed_callback(std::move(callback));
}

void TrellisController::set_on_any_key_released_callback(OnAnyKeyEventCallback callback) const {
    interface_->add_on_any_key_released_callback(std::move(callback));
}

void TrellisController::set_on_any_key_held_callback(Duration hold_duration_ms, OnAnyKeyEventCallback callback) const {

}

void TrellisController::add_timer_callback(const int period_ms, OnTimerEventCallback callback) const {
    interface_->add_timer(period_ms, std::move(callback));
}

void TrellisController::clear_callbacks() const {
    interface_->clear_callbacks();
}

} // namespace tboard