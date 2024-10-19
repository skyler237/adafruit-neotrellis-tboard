//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TRELLIS_INTERFACE_H
#define TRELLIS_INTERFACE_H

// #include "includes.h"
#include <cmath>
#include <functional>
#include <memory>
// #include <chrono>

// #include "colors.h"


struct Duration {
    int seconds;
    int milliseconds;
};

struct Time {
    int seconds;
    int milliseconds;

    friend bool operator<(const Time& lhs, const Time& rhs) {
        if (lhs.seconds < rhs.seconds) {
            return true;
        }
        if (rhs.seconds < lhs.seconds) {
            return false;
        }
        return lhs.milliseconds < rhs.milliseconds;
    }

    friend bool operator<=(const Time& lhs, const Time& rhs) { return !(rhs < lhs); }

    friend bool operator>(const Time& lhs, const Time& rhs) { return rhs < lhs; }

    friend bool operator>=(const Time& lhs, const Time& rhs) { return !(lhs < rhs); }

    Time operator+(const Duration & duration) const {
    Time result = {.seconds = seconds + duration.seconds, .milliseconds = milliseconds + duration.milliseconds};
    if (result.milliseconds >= 1000) {
        result.seconds += 1;
        result.milliseconds -= 1000;
    }
    return result;
};
};


struct Clock {
    static Time now() {
        return {.seconds=0, .milliseconds=0};
    }
};

// Used to register a callback for when a specific button is pressed
using OnPressedCallback = std::function<void(const Time&)>;
// Used to register a callback for when any button is pressed -- provides the x, y coordinates of the button
using OnAnyKeyPressedCallback = std::function<void(int, int, const Time&)>;
// Used to register a callback for a timer event
using OnTimerEventCallback = std::function<void(const Time&)>;


template<typename T, int rows, int cols>
struct Grid {
    Grid() = default;

    T& operator()(int x, int y) {
        check_bounds(x, y);
        return grid_[y][x];
    }

    const T& operator()(int x, int y) const {
        check_bounds(x, y);
        return grid_[y][x];
    }

private:
    std::array<std::array<T, cols>, rows> grid_;

    static bool check_bounds(int x, int y) {
        if (x < 0 || x >= cols || y < 0 || y >= rows) {
            return false;
        }
        return true;
    }
};

template<typename T>
using Grid8x8 = Grid<T, 8, 8>;


class TrellisInterface {
public:
    virtual ~TrellisInterface() = default;

    virtual void set_pixel_color(int x, int y, int r, int g, int b) = 0;
    virtual void clear_pixel(int x, int y) = 0;
    virtual void register_on_pressed_callback(int x, int y, OnPressedCallback callback) = 0;
    virtual void register_on_any_key_pressed_callback(OnAnyKeyPressedCallback callback) = 0;
    virtual void register_timer_callback(int period_ms, OnTimerEventCallback callback) = 0;
    virtual void show() = 0;
    virtual void clear_callbacks() = 0;
};


using TrellisInterfacePtr = std::shared_ptr<TrellisInterface>;
#endif // TRELLIS_INTERFACE_H