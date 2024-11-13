//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TRELLIS_HW_INTERFACE_H
#define TRELLIS_HW_INTERFACE_H

// #include "includes.h"
#include <Adafruit_NeoTrellis.h>
#include <map>
#include <timing.h>
#include <utility>
#include <vector>

#include "colors.h"
#include "optional.h"
#include "trellis_interface.h"

constexpr int X_DIM = 8;
constexpr int Y_DIM = 8;
static constexpr uint32_t DEFAULT_TICK_PERIOD_US = 1000;
class TrellisHWInterface;
using TrellisHWInterfacePtr = std::shared_ptr<TrellisHWInterface>;

class Timer {
public:
    Timer(uint16_t period_ms, OnTimerEventCallback callback);

    void tick(Time now);

private:
    OnTimerEventCallback timer_callback_;
    tl::optional<Time> next_timer_time_ = tl::nullopt;
    uint16_t timer_period_ms_;
};

struct KeyHoldTime {
    int x;
    int y;
    Duration hold_duration;
};

class TrellisHWInterface : public TrellisInterface {
public:
    TrellisHWInterface();

    static TrellisHWInterfacePtr get_instance();

    TrellisCallback key_event_callback(keyEvent event);

    Adafruit_MultiTrellis& get_trellis();

    void begin();

    void set_pixel_color(int x, int y, int r, int g, int b) override;

    void set_pixel_color(int x, int y, RGBA rgba);

    void clear_pixel(int x, int y) override;

    void add_on_pressed_callback(int x, int y, OnEventCallback callback) override;
    void add_on_released_callback(int x, int y, OnEventCallback callback) override;

    void add_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) override;
    void add_on_any_key_released_callback(OnAnyKeyEventCallback callback) override;

    void add_on_any_key_held_callback(Duration hold_duration, OnAnyKeyEventCallback callback) override;
    // void add_keys_held_callback(Duration hold_duration, std::vector<Key> keys, OnEventCallback callback) override;
    void add_on_key_held_callback(Duration hold_duration, int x, int y, OnEventCallback callback) override;

    void add_timer(int period_ms, OnTimerEventCallback callback) override;

    void clear_callbacks() override;

    void show() override;

    void tick();

private:
    Adafruit_MultiTrellis trellis_;
    Grid8x8<std::vector<OnEventCallback>> on_pressed_callbacks_;
    Grid8x8<std::vector<OnEventCallback>> on_released_callbacks_;
    std::vector<OnAnyKeyEventCallback> any_key_pressed_callbacks_;
    std::vector<OnAnyKeyEventCallback> any_key_released_callbacks_;
    std::vector<Timer> timers_;
    milliSeconds time_ms_;

    // Used for determining how long each key has been held -- nullopt if key was released
    Grid8x8<tl::optional<Time>> key_pressed_times_;
    // Store these in a map (in ascending duration order) so we can quit the search after the first one that is not met
    std::map<Duration, OnAnyKeyEventCallback> any_key_held_callbacks_;
    Grid8x8<std::map<Duration, OnEventCallback>> key_held_callbacks_;

    std::vector<KeyHoldTime> get_key_hold_times(const Time& now) const;
};
#endif // TRELLIS_HW_INTERFACE_H
