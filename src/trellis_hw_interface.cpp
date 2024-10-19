//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "trellis_hw_interface.h"

TrellisHWInterface::TrellisHWInterface() {
    Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

        {Adafruit_NeoTrellis(0x2F), Adafruit_NeoTrellis(0x2E)},

        {Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x30)}
    };
    // Create underlying neotrellis object
    trellis_ =
        std::make_shared<Adafruit_MultiTrellis>(reinterpret_cast<Adafruit_NeoTrellis*>(t_array), Y_DIM / 4, X_DIM / 4);
}

TrellisCallback TrellisHWInterface::key_event_callback(keyEvent event) {
    const int x = event.bit.NUM % X_DIM;
    const int y = event.bit.NUM / X_DIM;
    if (event.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        const auto& cb = callbacks_(x, y);
        if (cb.has_value()) {
            cb.value()(Clock::now());
        }
        if (any_key_callback_) {
            any_key_callback_(x, y, Clock::now());
        }
    }
    return nullptr;
}

void TrellisHWInterface::begin() const {
    // Setup serial interface
    Serial.begin(9600);

    // Make sure the trellis began ok...
    if (!trellis_->begin()) {
        Serial.println("failed to begin trellis");
        // while (1) delay(1);
    }

    // Setup events, etc. for all the keys
    for (int y = 0; y < Y_DIM; y++) {
        for (int x = 0; x < X_DIM; x++) {
            // activate rising and falling edges on all keys
            trellis_->activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
            trellis_->activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
            // trellis_->registerCallback(x, y, std::bind(&TrellisHWInterface::key_event_callback, this,
            // std::placeholders::_1));
        }
    }
}

void TrellisHWInterface::set_pixel_color(int x, int y, int r, int g, int b) {
    trellis_->setPixelColor(x, y, seesaw_NeoPixel::Color(r, g, b));
}

void TrellisHWInterface::set_pixel_color(int x, int y, RGB rgb) {
    set_pixel_color(x, y, rgb.r, rgb.g, rgb.b);
}

void TrellisHWInterface::clear_pixel(int x, int y) {
    trellis_->setPixelColor(x, y, 0);
}

void TrellisHWInterface::register_on_pressed_callback(int x, int y, OnPressedCallback callback) {
    // TODO: Register local callbacks to emplace/update here, similar to virtual interface
    callbacks_(x, y) = std::move(callback);
}

void TrellisHWInterface::register_on_any_key_pressed_callback(OnAnyKeyPressedCallback callback) {
    any_key_callback_ = std::move(callback);
}

void TrellisHWInterface::register_timer_callback(int period_ms, OnTimerEventCallback callback) {
    // todo
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
    trellis_->show();
}