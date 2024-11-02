//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#pragma once

#include <random>

#include "os/application.h"

namespace tboard::apps {

class LightsOut final : public Application {
public:
    enum class GameState : uint8_t {
        INITIALIZING,
        IN_PROGRESS,
        WON
    };
    ~LightsOut() override;

    LightsOut(const TrellisControllerPtr& trellis_controller, const ApplicationSwitcherPtr& application_switcher);

    ApplicationId get_id() const override;
    int get_tick_period_ms() const override;
    void init() override;
    void exit() override;
    tl::optional<ApplicationId> tick(const Time& now) override;

private:
    std::random_device random_;
    std::mt19937 generator_;
    std::uniform_int_distribution<> distribution_;
    GameState game_state_{GameState::INITIALIZING};

    static constexpr int INITIALIZATION_TOGGLE_COUNT = 50;
    int initialization_toggle_counter_{0};

    int win_animation_counter_{0};

    void initialize_game_board();

    RGBA get_random_color();
};

} // namespace tboard::apps