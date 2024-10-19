//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TRELLIS_HW_INTERFACE_H
#define TRELLIS_HW_INTERFACE_H

// #include "includes.h"
#include <Adafruit_NeoTrellis.h>
#include "optional.h"

#include "colors.h"
#include "trellis_interface.h"

constexpr int X_DIM = 8;
constexpr int Y_DIM = 8;
class TrellisHWInterface : public TrellisInterface
{
public:
    explicit TrellisHWInterface();

    TrellisCallback key_event_callback(keyEvent event);

    Adafruit_MultiTrellis& get_trellis();

    void begin();

    void set_pixel_color(int x, int y, int r, int g, int b) override;

    void set_pixel_color(int x, int y, RGB rgb);

    void clear_pixel(int x, int y) override;

    void register_on_pressed_callback(int x, int y, OnPressedCallback callback) override;

    void register_on_any_key_pressed_callback(OnAnyKeyPressedCallback callback) override;

    void register_timer_callback(int period_ms, OnTimerEventCallback callback) override;

    void clear_callbacks() override;

    void show() override;

private:
    Adafruit_MultiTrellis trellis_;
    Grid8x8<tl::optional<OnPressedCallback>> callbacks_;
    OnAnyKeyPressedCallback any_key_callback_;
};
#endif // TRELLIS_HW_INTERFACE_H
