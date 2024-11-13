//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "trellis_hw_interface.h"

Timer::Timer(const uint16_t period_ms, OnTimerEventCallback callback)
    : timer_callback_(std::move(callback)), timer_period_ms_(period_ms) {
    // Treat 0 period as no timer
    if (period_ms == 0) {
        next_timer_time_ = tl::nullopt;
    } else {
        // Set timer to trigger right away
        next_timer_time_ = 0;
    }
}

void Timer::tick(const Time now) {
    if (next_timer_time_.has_value() && now >= next_timer_time_) {
        next_timer_time_ = now + timer_period_ms_;
        tl::optional<Duration> maybe_new_period = timer_callback_(now);
        if (maybe_new_period.has_value()) {
            const Duration delta_duration = maybe_new_period.value() - timer_period_ms_;
            timer_period_ms_ = maybe_new_period.value();
            next_timer_time_.value() += delta_duration;
        }
    }
}

TrellisHWInterface::TrellisHWInterface()
    : trellis_(reinterpret_cast<Adafruit_NeoTrellis*>(new Adafruit_NeoTrellis[Y_DIM / 4][X_DIM / 4]{
                   {Adafruit_NeoTrellis(0x2F), Adafruit_NeoTrellis(0x2E)},
                   {Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x30)}
}),
               Y_DIM / 4,
               X_DIM / 4) { }

TrellisHWInterfacePtr TrellisHWInterface::get_instance() {
    static auto instance = std::make_shared<TrellisHWInterface>();
    return instance;
}

TrellisCallback TrellisHWInterface::key_event_callback(keyEvent event) {
    const int x = event.bit.NUM % X_DIM;
    const int y = event.bit.NUM / X_DIM;
    if (event.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        for (const auto& cb : on_pressed_callbacks_(x, y)) {
            cb(time_ms_.now());
        }
        for (const auto& cb : any_key_pressed_callbacks_) {
            cb(x, y, time_ms_.now());
        }
        key_pressed_times_(x, y) = time_ms_.now();
    } else if (event.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
        for (const auto& cb : on_released_callbacks_(x, y)) {
            cb(time_ms_.now());
        }
        for (const auto& cb : any_key_released_callbacks_) {
            cb(x, y, time_ms_.now());
        }
        key_pressed_times_(x, y) = tl::nullopt;
    }
    return nullptr;
}

Adafruit_MultiTrellis& TrellisHWInterface::get_trellis() {
    return trellis_;
}

void TrellisHWInterface::begin() {
    // Setup serial interface
    Serial.begin(115200);

    // Make sure the trellis began ok...
    if (!trellis_.begin()) {
        Serial.println("failed to begin trellis");
        // while (1) delay(1);
    }

    // Setup events, etc. for all the keys
    for (int y = 0; y < Y_DIM; y++) {
        for (int x = 0; x < X_DIM; x++) {
            // activate rising and falling edges on all keys
            trellis_.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
            trellis_.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
            trellis_.registerCallback(
                x, y, [](keyEvent event) -> TrellisCallback { return get_instance()->key_event_callback(event); });
        }
    }
}

void TrellisHWInterface::set_pixel_color(int x, int y, int r, int g, int b) {
    trellis_.setPixelColor(x, y, seesaw_NeoPixel::Color(r, g, b));
}

void TrellisHWInterface::set_pixel_color(int x, int y, RGBA rgb) {
    set_pixel_color(x, y, rgb.colors.red, rgb.colors.green, rgb.colors.blue);
}

void TrellisHWInterface::clear_pixel(int x, int y) {
    trellis_.setPixelColor(x, y, 0);
}

void TrellisHWInterface::add_on_pressed_callback(int x, int y, OnEventCallback callback) {
    on_pressed_callbacks_(x, y).emplace_back(std::move(callback));
}

void TrellisHWInterface::add_on_released_callback(int x, int y, OnEventCallback callback) {
    on_released_callbacks_(x, y).emplace_back(std::move(callback));
}

void TrellisHWInterface::add_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) {
    any_key_pressed_callbacks_.emplace_back(std::move(callback));
}

void TrellisHWInterface::add_on_any_key_released_callback(OnAnyKeyEventCallback callback) {
    any_key_released_callbacks_.emplace_back(std::move(callback));
}

void TrellisHWInterface::add_on_any_key_held_callback(Duration hold_duration, OnAnyKeyEventCallback callback) {
    any_key_held_callbacks_.emplace(hold_duration, std::move(callback));
}

void TrellisHWInterface::add_on_key_held_callback(Duration hold_duration,
                                               const int x,
                                               const int y,
                                               OnEventCallback callback) {
    key_held_callbacks_(x, y).emplace(hold_duration, std::move(callback));
}

void TrellisHWInterface::add_timer(const int period_ms, OnTimerEventCallback callback) {
    timers_.emplace_back(period_ms, std::move(callback));
}

void TrellisHWInterface::clear_callbacks() {
    for (int x = 0; x < X_DIM; ++x) {
        for (int y = 0; y < Y_DIM; ++y) {
            on_pressed_callbacks_(x, y).clear();
            on_released_callbacks_(x, y).clear();
            key_held_callbacks_(x, y).clear();
        }
    }
    any_key_pressed_callbacks_.clear();
    any_key_released_callbacks_.clear();
    any_key_held_callbacks_.clear();
}

void TrellisHWInterface::show() {
    trellis_.show();
}

void TrellisHWInterface::tick() {
    trellis_.read();
    // Tick each timer (will call callbacks if time has elapsed)
    for (auto& timer : timers_) {
        timer.tick(time_ms_.now());
    }
    // Check for held keys
    for (const auto& key_hold_time : get_key_hold_times(time_ms_.now())) {
        Serial.printf("key held: %d, %d, %d\n", key_hold_time.x, key_hold_time.y, key_hold_time.hold_duration);
        // Check if any key held callbacks should be called
        for (const auto& [hold_duration, callback] : any_key_held_callbacks_) {
            // Since durations are in order, we can break early
            if (key_hold_time.hold_duration < hold_duration) {
                break;
            }
            // Otherwise, call the callback and check the next one
            callback(key_hold_time.x, key_hold_time.y, time_ms_.now());
        }

        // Check if specific key held callbacks should be called
        for (const auto& [hold_duration, callback] : key_held_callbacks_(key_hold_time.x, key_hold_time.y)) {
            // Same as above, if we find a duration that is too long, we can break
            if (key_hold_time.hold_duration < hold_duration) {
                break;
            }
            callback(time_ms_.now());
        }
    }

    delayMicroseconds(DEFAULT_TICK_PERIOD_US);
}

std::vector<KeyHoldTime> TrellisHWInterface::get_key_hold_times(const Time& now) const {
    std::vector<KeyHoldTime> key_hold_times;
    for (const auto& [row, col, time] : key_pressed_times_) {
        if (time.has_value()) {
            key_hold_times.emplace_back(KeyHoldTime{col, row, now - time.value()});
        }
    }
    return key_hold_times;
}
