//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TRELLIS_HW_INTERFACE_H
#define TRELLIS_HW_INTERFACE_H

// #include "includes.h"
#include <Adafruit_NeoTrellis.h>
#include <timing.h>
#include "optional.h"

#include "colors.h"
#include "trellis_interface.h"


constexpr int X_DIM = 8;
constexpr int Y_DIM = 8;
static constexpr uint16_t DEFAULT_TICK_PERIOD_MS = 10;
class TrellisHWInterface : public TrellisInterface
{
public:

    static TrellisHWInterface& get_instance();

    TrellisCallback key_event_callback(keyEvent event);

    Adafruit_MultiTrellis& get_trellis();

    void begin();

    void set_pixel_color(int x, int y, int r, int g, int b) override;

    void set_pixel_color(int x, int y, RGB rgb);

    void clear_pixel(int x, int y) override;

    void register_on_pressed_callback(int x, int y, OnEventCallback callback) override;
    void register_on_released_callback(int x, int y, OnEventCallback callback) override;

    void register_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) override;
    void register_on_any_key_released_callback(OnAnyKeyEventCallback callback) override;

    void register_timer_callback(int period_ms, OnTimerEventCallback callback) override;

    void clear_callbacks() override;

    void show() override;

    void tick();



private:
    explicit TrellisHWInterface();
    Adafruit_MultiTrellis trellis_;
    Grid8x8<tl::optional<OnEventCallback>> on_pressed_callbacks_;
    Grid8x8<tl::optional<OnEventCallback>> on_released_callbacks_;
    OnAnyKeyEventCallback any_key_pressed_callback_;
    OnAnyKeyEventCallback any_key_released_callback_;
    OnTimerEventCallback timer_callback_;
    uint16_t current_period_ms_{DEFAULT_TICK_PERIOD_MS};
    milliSeconds time_ms_;
};
#endif // TRELLIS_HW_INTERFACE_H
