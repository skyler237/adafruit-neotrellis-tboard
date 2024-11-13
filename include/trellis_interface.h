//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TRELLIS_INTERFACE_H
#define TRELLIS_INTERFACE_H

#include <functional>
#include <memory>
#include <vector>

// Assume this is milliseconds
using Time = uint32_t;
using Duration = uint32_t;

struct Key {
    int x;
    int y;
};

// Used to register a callback for when a specific button is pressed
using OnEventCallback = std::function<void(const Time&)>;
// Used to register a callback for when any button is pressed/released/held -- provides the x, y coordinates of the
// button
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

    // Add iterator class to make the grid iterable in a range based loop
    template<typename OuterIteratorT, typename InnerIteratorT>
    class Iterator {
    public:
        Iterator(OuterIteratorT row, OuterIteratorT row_end) : row_(row), row_end_(row_end), col_(row->begin()) { }

        Iterator& operator++() {
            ++col_;
            ++col_index_;
            if (col_ == row_->end()) {
                ++row_;
                ++row_index_;
                col_index_ = 0;
                if (row_ != row_end_) {
                    col_ = row_->begin();
                }
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return row_ != other.row_ || (row_ != row_end_ && col_ != other.col_);
        }

        std::tuple<int, int, T> operator*() const { return std::make_tuple(row_index_, col_index_, *col_); }

        int row() const { return row_index_; }

        int col() const { return col_index_; }

    private:
        OuterIteratorT row_;
        OuterIteratorT row_end_;
        InnerIteratorT col_;
        int row_index_{0};
        int col_index_{0};
    };

    using Iter =
        Iterator<typename std::array<std::array<T, cols>, rows>::iterator, typename std::array<T, cols>::iterator>;
    using ConstIter = Iterator<typename std::array<std::array<T, cols>, rows>::const_iterator,
                               typename std::array<T, cols>::const_iterator>;

    Iter begin() { return Iter(grid_.begin(), grid_.end()); }

    Iter end() { return Iter(grid_.end(), grid_.end()); }

    ConstIter begin() const { return ConstIter(grid_.cbegin(), grid_.cend()); }

    ConstIter end() const { return ConstIter(grid_.cend(), grid_.cend()); }

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
    virtual void add_on_pressed_callback(int x, int y, OnEventCallback callback) = 0;
    virtual void add_on_released_callback(int x, int y, OnEventCallback callback) = 0;
    virtual void add_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) = 0;
    virtual void add_on_any_key_released_callback(OnAnyKeyEventCallback callback) = 0;
    virtual void add_on_any_key_held_callback(Duration hold_duration, OnAnyKeyEventCallback callback) = 0;
    // virtual void add_keys_held_callback(Duration hold_duration, std::vector<Key> keys, OnEventCallback callback) = 0;
    virtual void add_on_key_held_callback(Duration hold_duration, int x, int y, OnEventCallback callback) = 0;
    virtual void add_timer(int period_ms, OnTimerEventCallback callback) = 0;
    virtual void show() = 0;
    virtual void clear_callbacks() = 0;
};

using TrellisInterfacePtr = std::shared_ptr<TrellisInterface>;
#endif // TRELLIS_INTERFACE_H