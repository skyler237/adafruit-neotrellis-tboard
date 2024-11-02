//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "apps/lights_out.h"

#include <USB/USBAPI.h>

#include <random>

#include "apps/application_ids.h"

namespace tboard::apps {
namespace {
constexpr float BRIGHTNESS = 0.05;
const auto COLOR = GREEN;
} // namespace

LightsOut::~LightsOut() = default;

LightsOut::LightsOut(const TrellisControllerPtr& trellis_controller, const ApplicationSwitcherPtr& application_switcher)
    : Application(trellis_controller, application_switcher), generator_(random_()), distribution_(0, 7) { }

ApplicationId LightsOut::get_id() const {
    return LIGHTS_OUT_ID;
}

int LightsOut::get_tick_period_ms() const {
    return 50;
}

void LightsOut::init() {
    game_state_ = GameState::INITIALIZING;
    trellis_controller_->display()->clear();
    trellis_controller_->display()->show();

    trellis_controller_->set_on_any_key_pressed_callback([this](int x, int y, const Time&) {
        // Toggle the pressed pixel and surrounding adjacent pixels
        trellis_controller_->display()->toggle_pixel(x, y, COLOR, BRIGHTNESS);
        if (x > 0) {
            trellis_controller_->display()->toggle_pixel(x - 1, y, COLOR, BRIGHTNESS);
        }
        if (x < 7) {
            trellis_controller_->display()->toggle_pixel(x + 1, y, COLOR, BRIGHTNESS);
        }
        if (y > 0) {
            trellis_controller_->display()->toggle_pixel(x, y - 1, COLOR, BRIGHTNESS);
        }
        if (y < 7) {
            trellis_controller_->display()->toggle_pixel(x, y + 1, COLOR, BRIGHTNESS);
        }
        trellis_controller_->display()->show();

        // Check for win condition
        if (trellis_controller_->display()->are_all_pixels_off()) {
            // Start a timer performa win animation/transition
            game_state_ = GameState::WON;
        }
    });
}

void LightsOut::exit() {
    trellis_controller_->clear_callbacks();
}

tl::optional<ApplicationId> LightsOut::tick(const Time& now) {
    Serial.println("LightsOut::tick()");
    switch (game_state_) {
    case GameState::INITIALIZING:
        if (initialization_toggle_counter_ > INITIALIZATION_TOGGLE_COUNT) {
            game_state_ = GameState::IN_PROGRESS;
            initialization_toggle_counter_ = 0;
            break;
        }

        // Randomly toggle a pixel
        trellis_controller_->display()->toggle_pixel(
            distribution_(generator_), distribution_(generator_), COLOR, BRIGHTNESS);
        trellis_controller_->display()->show();

        initialization_toggle_counter_++;
        Serial.printf("init counter: %d\n", initialization_toggle_counter_);
        break;
    case GameState::IN_PROGRESS:
        // Do nothing -- transition to WON will be handled in the key press callback
        break;
    case GameState::WON:
        constexpr int NUM_FRAMES = 5;
        static const std::array<RGBA, NUM_FRAMES> animation_colors = {BLACK, RED, WHITE, BLUE, GREEN};
        constexpr int NUM_ITERATIONS = 4;
        constexpr int SLOWDOWN_RATE = 3;

        // Light up only the ring corresponding to the counter, starting in the middle
        const int animation_idx = (win_animation_counter_ / SLOWDOWN_RATE) % NUM_FRAMES;
        if (animation_idx == 0) {
            // Clear display
            trellis_controller_->display()->clear();
        } else {
            int x_min = 4 - animation_idx;
            int x_max = 3 + animation_idx;
            int y_min = 4 - animation_idx;
            int y_max = 3 + animation_idx;

            for (int x = x_min; x <= x_max; x++) {
                for (int y = y_min; y <= y_max; y++) {
                    if (x == x_min || x == x_max || y == y_min || y == y_max) {
                        trellis_controller_->display()->set_pixel_color(x, y, animation_colors[animation_idx], BRIGHTNESS);
                    }
                }
            }
            if (win_animation_counter_ % SLOWDOWN_RATE == 0) {
                trellis_controller_->display()->show();
            }
        }

        win_animation_counter_++;

        if (win_animation_counter_ > NUM_FRAMES * NUM_ITERATIONS * SLOWDOWN_RATE) {
            game_state_ = GameState::INITIALIZING;
            win_animation_counter_ = 0;
        }
        break;
    }
    return tl::nullopt;
}

void LightsOut::initialize_game_board() {
    // Randomly light up some pixels
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            constexpr float INITIAL_LIGHT_ON_PROBABILITY = 0.7;
            if (distribution_(generator_) < INITIAL_LIGHT_ON_PROBABILITY) {
                trellis_controller_->display()->set_pixel_color(i, j, COLOR, BRIGHTNESS);
            }
        }
    }
    trellis_controller_->display()->show();
}

RGBA LightsOut::get_random_color() {
    return RGBA{static_cast<uint8_t>(distribution_(generator_)),
                static_cast<uint8_t>(distribution_(generator_)),
                static_cast<uint8_t>(distribution_(generator_))};
}
} // namespace tboard::apps