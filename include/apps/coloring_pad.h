//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #pragma once
#ifndef TBOARD_APPS_COLORING_PAD_H_
#define TBOARD_APPS_COLORING_PAD_H_
#include "os/application.h"

namespace tboard::apps {

class ColorPicker {
public:
    // This is the row where the color picker is located
    static constexpr int COLOR_PICKER_ROW = 7;

    explicit ColorPicker(TrellisDisplayPtr display);

    void handle_button_pressed(int x, int y);

    tl::optional<RGBA> get_selected_color() const;

    void draw() const;

private:
    TrellisDisplayPtr display_;
    tl::optional<RGBA> selected_color_;
    tl::optional<int> selected_color_idx_;
    std::array<RGBA, 8> colors_;
};

class ColoringPad : public Application {
public:
    using Application::Application;
    ~ColoringPad() override;

    ApplicationId get_id() const override;
    int get_tick_period_ms() const override;
    void init() override;
    void exit() override;
    tl::optional<ApplicationId> tick(const Time& now) override;

private:
    std::shared_ptr<ColorPicker> color_picker_;

};

} // namespace tboard::apps
#endif // TBOARD_APPS_COLORING_PAD_H_