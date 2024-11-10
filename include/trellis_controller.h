//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_TRELLIS_CONTROLLER_H
#define TBOARD_TRELLIS_CONTROLLER_H
#include "colors.h"
#include "trellis_display.h"
#include "trellis_interface.h"

namespace tboard {

class TrellisController {
public:
    explicit TrellisController(TrellisInterfacePtr trellis_interface);

    // Get the display
    const TrellisDisplayPtr& display() const { return trellis_display_; }

    // Register callbacks for button presses
    void register_on_pressed_callback(int x, int y, OnEventCallback callback) const;
    void register_on_released_callback(int x, int y, OnEventCallback callback) const;
    void set_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) const;
    void set_on_any_key_released_callback(OnAnyKeyEventCallback callback) const;
    void set_on_any_key_held_callback(Duration hold_duration_ms, OnAnyKeyEventCallback callback) const;
    void add_timer_callback(int period_ms, OnTimerEventCallback callback) const;
    void clear_callbacks() const;

private:
    TrellisInterfacePtr interface_;
    TrellisDisplayPtr trellis_display_;
};

using TrellisControllerPtr = std::shared_ptr<TrellisController>;

} // namespace tboard
#endif // TBOARD_TRELLIS_CONTROLLER_H