//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#pragma once

#include <animation.h>

#include "os/application.h"

namespace tboard::apps {

enum Player : uint8_t { PLAYER_X, PLAYER_O };

struct GameState {
    bool is_game_over;
    // nullopt if the game is a draw
    tl::optional<Player> winner;
};

class TicTacToeBoard {
public:
    explicit TicTacToeBoard(TrellisDisplayPtr display);

    void handle_button_pressed(int x, int y);

    void draw() const;

    const GameState& get_game_state() const;

private:
    TrellisDisplayPtr display_;
    std::array<std::array<tl::optional<Player>, 3>, 3> board_;
    GameState game_state_;

    Player current_player_{PLAYER_X};

    GameState assess_game_state();

    // Convert from 8x8 pixel grid to 3x3 tic tac toe grid -- return optional if the pixel is on a divider line
    tl::optional<std::pair<int, int>> to_board_coords(int x, int y) const;
};

class TicTacToe : public Application {
public:
    using Application::Application;
    ~TicTacToe() override;

    ApplicationId get_id() const override;
    int get_tick_period_ms() const override;
    void init() override;
    void exit() override;
    tl::optional<ApplicationId> tick(const Time& now) override;

private:
    std::shared_ptr<TicTacToeBoard> board_;
    tl::optional<MultiFrameAnimation> win_animation_;
};

} // namespace tboard::apps
