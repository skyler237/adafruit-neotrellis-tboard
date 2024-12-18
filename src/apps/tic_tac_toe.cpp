//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "apps/tic_tac_toe.h"

#include <USB/USBAPI.h>

#include "apps/application_ids.h"

namespace tboard::apps {

// Constant definitions
static const RGBA X_COLOR = RED;
static const RGBA O_COLOR = BLUE;
static const RGBA BOARD_COLOR = WHITE;

static constexpr float DEFAULT_BRIGHTNESS = 0.2;

TicTacToeBoard::TicTacToeBoard(TrellisDisplayPtr display) : display_(std::move(display)), game_state_() { }

GameState TicTacToeBoard::assess_game_state() {
    // Check for horizontal win
    for (int y = 0; y < 3; ++y) {
        if (board_[y][0].has_value() && board_[y][0] == board_[y][1] && board_[y][1] == board_[y][2]) {
            // Horizontal win
            return {
                .is_game_over = true,
                .win_state = {.winner = board_[y][0].value(), .winning_coords = {{{0, y}, {1, y}, {2, y}}}}
            };
        }
    }
    // Check for vertical win
    for (int x = 0; x < 3; ++x) {
        if (board_[0][x].has_value() && board_[0][x] == board_[1][x] && board_[1][x] == board_[2][x]) {
            // Vertical win
            return {
                .is_game_over = true,
                .win_state = {.winner = board_[0][x].value(), .winning_coords = {{{x, 0}, {x, 1}, {x, 2}}}}
            };
        }
    }
    // Check for diagonal win (top left to bottom right)
    if (board_[0][0].has_value() && board_[0][0] == board_[1][1] && board_[1][1] == board_[2][2]) {
        // Diagonal win
        return {
            .is_game_over = true,
            .win_state = {.winner = board_[0][0].value(), .winning_coords = {{{0, 0}, {1, 1}, {2, 2}}}}
        };
    }
    // Check for diagonal win (top right to bottom left)
    if (board_[0][2].has_value() && board_[0][2] == board_[1][1] && board_[1][1] == board_[2][0]) {
        // Diagonal win
        return {
            .is_game_over = true,
            .win_state = {.winner = board_[0][2].value(), .winning_coords = {{{2, 0}, {1, 1}, {0, 2}}}}
        };
    }
    // Check for draw
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            if (!board_[y][x].has_value()) {
                // There is an empty cell, game is not over
                return {.is_game_over = false};
            }
        }
    }
    return {.is_game_over = true, .win_state = {.winner = tl::nullopt}};
}

void TicTacToeBoard::handle_button_pressed(int x, int y) {
    Serial.println("Handling button press at x: " + String(x) + " y: " + String(y));
    // Try to get board coordinate
    const auto board_coord = to_board_coords(x, y);
    // This is a divider line, do nothing
    if (!board_coord) {
        Serial.println("Pressed on divider at x: " + String(x) + " y: " + String(y));
        return;
    }

    const auto& [board_x, board_y] = board_coord.value();
    auto& player_mark = board_[board_y][board_x];
    // If the cell is already filled, do nothing
    if (player_mark.has_value()) {
        Serial.println("Cell is already filled by player " + String(player_mark.value()));
        return;
    }

    // otherwise, fill the cell with the current player mark
    Serial.println("Marking cell at x: " + String(board_x) + " y: " + String(board_y) + " with player " +
                   String(current_player_));
    player_mark = current_player_;

    // Update the board
    draw();

    // Switch player
    current_player_ = current_player_ == PLAYER_X ? PLAYER_O : PLAYER_X;

    // check for game over
    game_state_ = assess_game_state();
    Serial.println("Game is over: " + String(game_state_.is_game_over));
}

void TicTacToeBoard::draw(const std::set<BoardCoordinates>& highlighted_squares) const {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const auto board_coord = to_board_coords(x, y);
            // Pixel is on divider line
            if (!board_coord) {
                display_->set_pixel_color(x, y, BOARD_COLOR, DEFAULT_BRIGHTNESS);
            } else {
                const auto& [board_x, board_y] = board_coord.value();
                const auto& maybe_player_mark = board_[board_y][board_x];
                // If there is a player mark, display it
                if (maybe_player_mark.has_value()) {
                    // Maybe highlight this square
                    const float brightness =
                        highlighted_squares.count(BoardCoordinates{board_x, board_y}) > 0 ? 1.0 : DEFAULT_BRIGHTNESS;
                    const RGBA color = maybe_player_mark.value() == PLAYER_X ? X_COLOR : O_COLOR;
                    display_->set_pixel_color(x, y, color, brightness);
                    // NO player mark, keep this cell empty
                } else {
                    display_->set_pixel_off(x, y);
                }
            }
        }
    }
    display_->show();
}

const GameState& TicTacToeBoard::get_game_state() const {
    return game_state_;
}

tl::optional<BoardCoordinates> TicTacToeBoard::to_board_coords(int x, int y) const {
    if (x == 2 || x == 5 || y == 2 || y == 5) {
        return tl::nullopt;
    }
    return BoardCoordinates(x / 3, y / 3);
}

TicTacToe::~TicTacToe() = default;

ApplicationId TicTacToe::get_id() const {
    return TIC_TAC_TOE_ID;
}

int TicTacToe::get_tick_period_ms() const {
    return 100;
}

void TicTacToe::init() {
    Serial.println("Init tic tac toe (begin)");
    // Clear the screen
    trellis_controller_->display()->clear();

    // Draw the board
    board_ = std::make_shared<TicTacToeBoard>(trellis_controller_->display());
    board_->draw();

    trellis_controller_->display()->show();

    // Setup callbacks
    trellis_controller_->add_on_any_key_pressed_callback(
        [this](int x, int y, const Time&) { board_->handle_button_pressed(x, y); });

    // Add key held callback to return to application picker if the top right button is held
    constexpr Duration EXIT_APP_HOLD_DURATION_MS = 2000;
    trellis_controller_->add_on_key_held_callback(
        EXIT_APP_HOLD_DURATION_MS, 7, 0, [this](const Time&) { switch_to_app(APPLICATION_PICKER_ID); });

    Serial.println("Init tic tac toe (end)");
}

void TicTacToe::exit() { }

tl::optional<ApplicationId> TicTacToe::tick(const Time& now) {
    Serial.println("Tic!");
    // Check for and handle game over
    // If the win animation is in progress, tick it
    if (win_animation_) {
        if (!win_animation_->tick(now)) {
            win_animation_ = tl::nullopt;
            // Start the game over!
            return TIC_TAC_TOE_ID;
        }
        // Still animating
        return tl::nullopt;
    }

    Serial.println("Tac!");
    if (board_->get_game_state().is_game_over) {
        // Disable any other key presses during animation
        trellis_controller_->clear_callbacks();

        // Define the win animation
        const auto win_state = board_->get_game_state().win_state;

        // Initialize the win animation
        Serial.println("Start win animation");
        win_animation_.emplace(trellis_controller_->display());

        if (win_state.winner.has_value()) {
            // Add the win animation frames
            win_animation_->add_frame({.display_duration_ms = 500,
                                       .draw_frame_callback = [win_state, this](const TrellisDisplayPtr& display) {
                                           board_->draw({win_state.winning_coords[0]});
                                       }});
            win_animation_->add_frame({.display_duration_ms = 500,
                                       .draw_frame_callback = [win_state, this](const TrellisDisplayPtr& display) {
                                           board_->draw({win_state.winning_coords[1]});
                                       }});
            win_animation_->add_frame({.display_duration_ms = 500,
                                       .draw_frame_callback = [win_state, this](const TrellisDisplayPtr& display) {
                                           board_->draw({win_state.winning_coords[2]});
                                       }});
            win_animation_->add_frame({.display_duration_ms = 2000,
                                       .draw_frame_callback = [win_state, this](const TrellisDisplayPtr& display) {
                                           // Highlight all winning squares
                                           board_->draw(
                                               {win_state.winning_coords.begin(), win_state.winning_coords.end()});
                                       }});
        } else {
            win_animation_->add_frame({.display_duration_ms = 2000,
                                       .draw_frame_callback = [win_state, this](const TrellisDisplayPtr& display) {
                                           // Just freeze the frame for a bit before resetting
                                           board_->draw();
                                       }});
        }
    }

    Serial.println("Toe!");
    return tl::nullopt;
}
} // namespace tboard::apps