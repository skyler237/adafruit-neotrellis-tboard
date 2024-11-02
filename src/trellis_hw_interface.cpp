//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "trellis_hw_interface.h"

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
        const auto& cb = on_pressed_callbacks_(x, y);
        if (cb.has_value()) {
            cb.value()(time_ms_.now());
        }
        if (any_key_pressed_callback_) {
            any_key_pressed_callback_(x, y, time_ms_.now());
        }
    } else if (event.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
        const auto& cb = on_released_callbacks_(x, y);
        if (cb.has_value()) {
            cb.value()(time_ms_.now());
        }
        if (any_key_released_callback_) {
            any_key_released_callback_(x, y, time_ms_.now());
        }
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

void TrellisHWInterface::register_on_pressed_callback(int x, int y, OnEventCallback callback) {
    on_pressed_callbacks_(x, y) = std::move(callback);
}

void TrellisHWInterface::register_on_released_callback(int x, int y, OnEventCallback callback) {
    on_released_callbacks_(x, y) = std::move(callback);
}

void TrellisHWInterface::register_on_any_key_pressed_callback(OnAnyKeyEventCallback callback) {
    any_key_pressed_callback_ = std::move(callback);
}

void TrellisHWInterface::register_on_any_key_released_callback(OnAnyKeyEventCallback callback) {
    any_key_released_callback_ = std::move(callback);
}

void TrellisHWInterface::register_timer_callback(const int period_ms, OnTimerEventCallback callback) {
    if (period_ms == 0) {
        next_timer_time_ = 0;
    } else {
        // Set timer to trigger right away
        next_timer_time_ = time_ms_.now();
    }
    timer_callback_ = std::move(callback);
    timer_period_ms_ = period_ms;
}

void TrellisHWInterface::clear_callbacks() {
    for (int x = 0; x < X_DIM; ++x) {
        for (int y = 0; y < Y_DIM; ++y) {
            on_pressed_callbacks_(x, y) = tl::nullopt;
            on_released_callbacks_(x, y) = tl::nullopt;
        }
    }
    any_key_pressed_callback_ = {};
    any_key_released_callback_ = {};
}

void TrellisHWInterface::show() {
    trellis_.show();
}

void TrellisHWInterface::tick() {
    trellis_.read();
    if (timer_callback_ && next_timer_time_ > 0 && time_ms_.now() >= next_timer_time_) {
        next_timer_time_ = time_ms_.now() + timer_period_ms_;
        tl::optional<Duration> maybe_new_period = timer_callback_(time_ms_.now());
        if (maybe_new_period.has_value()) {
            const Duration delta_duration = maybe_new_period.value() - timer_period_ms_;
            timer_period_ms_ = maybe_new_period.value();
            next_timer_time_ += delta_duration;
        }
    }
    delayMicroseconds(DEFAULT_TICK_PERIOD_US);
    // if (any_key_callback_) {
    //     Serial.println("Has any key callback");
    //     any_key_callback_(0, 0, time_ms_.now());
    // }
    // else {
    //     Serial.println("No any key callback");
    // }
    // Serial.println("");
}

void TrellisHWInterface::set_timer_period(uint16_t period_ms) {
    timer_period_ms_ = period_ms;
}
