//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TRELLIS_INTERFACE_H
#define TRELLIS_INTERFACE_H

#include <functional>
#include <memory>

// Assume this is milliseconds
using Time = uint32_t;
using Duration = uint32_t;

// Used to register a callback for when a specific button is pressed
using OnEventCallback = std::function<void(const Time&)>;
// Used to register a callback for when any button is pressed -- provides the x, y coordinates of the button
using OnAnyKeyEventCallback = std::function<void(int, int, const Time&)>;
// Used to register a callback for a timer event -- returns the period for the next timer event
using OnTimerEventCallback = std::function<tl::optional<Duration>(const Time&)>;

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

    static bool check_bounds(int x, int y) { return x >= 0 && x < cols && y >= 0 && y < rows; }
};

template<typename T>
using Grid8x8 = Grid<T, 8, 8>;

class TrellisInterface {
public:
    virtual ~TrellisInterface() = default;

    virtual void set_pixel_color(int x, int y, int r, int g, int b) = 0;
    virtual void clear_pixel(int x, int y) = 0;
    virtual void register_on_pressed_callback(int x, int y, OnEventCallback callback) = 0;
    virtual void register_on_released_callback(int x, int y, OnEventCallback callback) = 0;
    virtual void register_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) = 0;
    virtual void register_on_any_key_released_callback(OnAnyKeyEventCallback callback) = 0;
    virtual void register_timer_callback(int period_ms, OnTimerEventCallback callback) = 0;
    virtual void show() = 0;
    virtual void clear_callbacks() = 0;
};

using TrellisInterfacePtr = std::shared_ptr<TrellisInterface>;
#endif // TRELLIS_INTERFACE_H