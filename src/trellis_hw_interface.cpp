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

TrellisHWInterface& TrellisHWInterface::get_instance() {
    static TrellisHWInterface instance;
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
    }
    else if (event.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
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
    } else {
        Serial.println("Trellis initialized");
    }

    // Setup events, etc. for all the keys
    for (int y = 0; y < Y_DIM; y++) {
        for (int x = 0; x < X_DIM; x++) {
            // activate rising and falling edges on all keys
            trellis_.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
            trellis_.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
            trellis_.registerCallback(x, y, [](keyEvent event) -> TrellisCallback {
                return get_instance().key_event_callback(event);
            });
        }
    }
}

void TrellisHWInterface::set_pixel_color(int x, int y, int r, int g, int b) {
    trellis_.setPixelColor(x, y, seesaw_NeoPixel::Color(r, g, b));
}

void TrellisHWInterface::set_pixel_color(int x, int y, RGB rgb) {
    set_pixel_color(x, y, rgb.r, rgb.g, rgb.b);
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
        current_period_ms_ = DEFAULT_TICK_PERIOD_MS;
    }
    else {
        current_period_ms_ = period_ms;
    }
    timer_callback_ = std::move(callback);
}

void TrellisHWInterface::clear_callbacks() {
    for (int x = 0; x < X_DIM; ++x) {
        for (int y = 0; y < Y_DIM; ++y) {
            register_on_pressed_callback(x, y, {});
        }
    }
    register_on_any_key_pressed_callback({});
}

void TrellisHWInterface::show() {
    trellis_.show();
}

void TrellisHWInterface::tick() {
    trellis_.read();
    if (timer_callback_) {
        timer_callback_(time_ms_.now());
    }
    delay(current_period_ms_);
    // if (any_key_callback_) {
    //     Serial.println("Has any key callback");
    //     any_key_callback_(0, 0, time_ms_.now());
    // }
    // else {
    //     Serial.println("No any key callback");
    // }
    // Serial.println("");
}