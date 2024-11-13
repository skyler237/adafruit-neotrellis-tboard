//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#define VBATPIN PIN_A6

#include "apps/battery.h"

#include <USB/USBAPI.h>
#include <resources/minifont.h>

#include <utility>
#include <variant.h>
#include <wiring_analog.h>

#include "apps/application_ids.h"

namespace tboard::apps {

BatteryBar::BatteryBar(TrellisControllerPtr trellis_controller) : trellis_controller_(std::move(trellis_controller)) { }

void BatteryBar::display_battery_percentage(float battery_percentage) const {
    constexpr int X_DIM = 8;
    constexpr int BAR_HEIGHT = 2;
    constexpr int BAR_Y = 6;
    constexpr float PERCENT_PER_PIXEL = 100.0 / X_DIM;
    constexpr float MAX_BRIGHTNESS = 0.8;

    const int num_pixels_on = static_cast<int>(battery_percentage / PERCENT_PER_PIXEL) + 1;

    for (int i = 0; i < X_DIM; ++i) {
        float ratio = static_cast<float>(i) / 7.0;
        const RGBA color = i < num_pixels_on ? interpolate_color(RED, GREEN, ratio) : RGBA(0, 0, 0);
        // Vary brightness of last pixel based on battery percentage
        const float remaining_percent = fmod(battery_percentage, PERCENT_PER_PIXEL);
        Serial.print("Extra percentage: ");
        Serial.println(remaining_percent);
        const float brightness_ratio = i > num_pixels_on ? remaining_percent / PERCENT_PER_PIXEL : 1.0;
        Serial.print("Brightness ratio: ");
        Serial.println(brightness_ratio);
        const float brightness = MAX_BRIGHTNESS * brightness_ratio;
        for (int j = 0; j < BAR_HEIGHT; ++j) {
            trellis_controller_->display()->set_pixel_color(i, BAR_Y + j, color, brightness);
        }
    }
}

Battery::Battery(const TrellisControllerPtr& trellis_controller, const ApplicationSwitcherPtr& application_switcher)
    : Application(trellis_controller, application_switcher), battery_bar_(trellis_controller_) { }

Battery::~Battery() = default;

ApplicationId Battery::get_id() const {
    return BATTERY_ID;
}

int Battery::get_tick_period_ms() const {
    return 20;
}

void Battery::init() {
    // Clear screen
    trellis_controller_->display()->clear();
    trellis_controller_->display()->show();

    // Add "exit button" in top right corner
    trellis_controller_->display()->set_pixel_color(7, 0, RGBA(255, 0, 0));
    trellis_controller_->display()->show();
    trellis_controller_->add_on_pressed_callback(
        7, 0, [this](const Time&) { switch_to_app(APPLICATION_PICKER_ID); });
}

void Battery::exit() {
    /* do nothing */
}

tl::optional<ApplicationId> Battery::tick(const Time& now) {
    // Get battery level

    float measuredvbat = static_cast<float>(analogRead(VBATPIN));
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage
    Serial.print("VBat: ");
    Serial.println(measuredvbat);

    constexpr float MAX_VOLTAGE = 4.2;
    constexpr float MIN_VOLTAGE = 3.2;
    const float battery_percentage = (measuredvbat - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE) * 100;
    Serial.print("Battery percentage: ");
    Serial.println(battery_percentage);

    float filtered_percentage = lpf_.filter(battery_percentage);
    Serial.print("Filtered percentage: ");
    Serial.println(filtered_percentage);

    display_battery_percentage(filtered_percentage);

    // Display battery level
    return tl::nullopt;
}

void Battery::display_percentage_text(float battery_percentage) const {
    int integer_percent = static_cast<int>(battery_percentage);
    constexpr int TEXT_Y = 1;
    constexpr int TEXT_X1 = 1;
    constexpr int TEXT_X2 = 5;

    trellis_controller_->display()->clear_region(0, TEXT_Y, 8, minifont::CHAR_HEIGHT);

    // Single digit
    if (integer_percent >= 100) {
        // Draw "100"
        trellis_controller_->display()->draw_character(0, TEXT_Y, '1', RGBA(255, 255, 255));
        trellis_controller_->display()->draw_character(2, TEXT_Y, '0', RGBA(255, 255, 255));
        trellis_controller_->display()->draw_character(5, TEXT_Y, '0', RGBA(255, 255, 255));
        return;
    }
    // Double digit
    int tens_digit = integer_percent / 10;
    int ones_digit = integer_percent % 10;
    trellis_controller_->display()->draw_character(TEXT_X1, TEXT_Y, '0' + tens_digit, RGBA(255, 255, 255));
    trellis_controller_->display()->draw_character(TEXT_X2, TEXT_Y, '0' + ones_digit, RGBA(255, 255, 255));
}

void Battery::display_battery_percentage(float battery_percentage) const {

    battery_bar_.display_battery_percentage(battery_percentage);

    display_percentage_text(battery_percentage);

    trellis_controller_->display()->show();
}
} // namespace tboard::apps